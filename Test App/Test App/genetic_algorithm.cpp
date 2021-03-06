#include "genetic_algorithm.h"

#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <direct.h>

genetic_algorithm::genetic_algorithm() :
	generation_(0),
	population_(glo_flock_size),
	num_of_genotypes_(glo_initial_num_of_genotypes),
	mutation_rate_(10)
{
}
genetic_algorithm::~genetic_algorithm()
{
}

void genetic_algorithm::Initialise(std::vector<boid> *boids)
{
	int file_num = 0;

	// The end result will be five different populations of equal size in the flock:
	float genotypes[glo_initial_num_of_genotypes][12];

	// Intrduce a random amount of variation into the genetic data: (genotype)
	for (int genotype = 0; genotype < num_of_genotypes_; genotype++)
	{
		// This is the randomised method: (produces 5 populations of 10 within the current flock)
		for (int gene = 0; gene < 12; gene++)
		{
			genotypes[genotype][gene] = rand() % 1000;
		}

		// This would be a heuristic method: (can switch between them using comments)
		//if (genotype < (num_of_genotypes_ - 1))
		//{
		//	for (int gene = 0; gene < 12; gene++)
		//	{
		//		genotypes[genotype][gene] = rand() % 1000;
		//	}
		//}
		//else
		//{
		//	float data[12];
		//	Heuristic(data);
		//	for (int gene = 0; gene < 12; gene++)
		//	{
		//		genotypes[genotype][gene] = data[gene];
		//	}
		//}
	}

	int genotype = 0;
	int iteration_counter = 0;
	float population_size = 0.2f * glo_flock_size;

	// For each boid:
	for (std::vector<boid>::iterator iterator = boids->begin(); iterator != boids->end(); iterator++)
	{
		// populate the genetic data:
		DNA dna = DNA();
		float data[12];

		for (int gene = 0; gene < 12; gene++)
		{
			data[gene] = genotypes[genotype][gene];
		}
		dna.UpdateDataSet(data);

		// This kind of needs some changing later to record the data properly:
		std::string txt_directory("GeneticDatatxt/Generation_1");
		std::string txt_file_name("/boid_data_");
		std::string txt_base(".txt");

		std::string csv_directory("GeneticDataCSV/Simulation_");
		std::string csv_file_name("/boid_data_");
		std::string csv_base(".csv");

		int check_txt;
		const char* txt_dir = "GeneticDatatxt/Generation_1";
		check_txt = _mkdir(txt_dir);

		int check_csv;
		const char* csv_dir = "GeneticDataCSV/Simulation_";
		check_csv = _mkdir( (csv_dir + std::to_string(glo_simulation_number)).c_str() );

		dna.StoreData(1, txt_directory+txt_file_name+std::to_string(file_num)+txt_base, csv_directory+ std::to_string(glo_simulation_number)+csv_file_name+std::to_string(file_num)+csv_base);
		file_num++;

		iterator->SetDNA(dna);
		iterator->SetPopID(std::to_string(genotype));

		iteration_counter++;
		if (iteration_counter > population_size)
		{
			iteration_counter = 0;
			genotype++;
		}
	}
}
void genetic_algorithm::Heuristic(float* data)
{
	// Cohesion Variables
	data[0] = 1.0f;			// coh_mult_
	data[1] = 30.0f;		// coh_div_mult_
	// Alignment Variables
	data[2] = 1.0f;			// ali_mult_
	data[3] = 10.0f;		// ali_div_mult_
	// Separation Variables
	data[4] = 0.025f;		// sep_mult_
	data[5] = 1.0f;			// sep_neigh_mult_
	data[6] = 1.0f;			// sep_clo_neigh_mult_
	// Food Attraction Variables
	data[7] = 0.0025f;		// food_mult_1_
	data[8] = 1.0f;			// food_mult_2_
	data[9] = 1.0f;			// food_div_mult_
	// Flock Avoidance Variables
	data[10] = 300.0f;		// floav_mult_
	data[11] = 1.0f;		// floav_div_mult_
}


void genetic_algorithm::Update(std::vector<boid>* boids, float flock_health, float enemy_flock_health, int generation)
{
	Evaluation	(boids, flock_health, enemy_flock_health);
	Selection	(boids, generation);
//	susSelection(boids, generation);
}


// Maybe also collect data here too, this will be useful in terms of getting useful data:
void genetic_algorithm::Evaluation(std::vector<boid>* boids, float flock_health, float enemy_flock_health)
{
	bool first_iteration = true;
	sum_fitness_ = 0.0f;
	populations_.clear();

	for (std::vector<boid>::iterator iterator = boids->begin(); iterator != boids->end(); iterator++)
	{
		// Evaluate the boid against the fitness function:
		float fitfunc = iterator->GetHealth() + flock_health - enemy_flock_health;
		iterator->SetFitness(fitfunc);

		// Produce the roulette wheel:
		sum_fitness_ += fitfunc;

		Population pop;
		pop.sum_fitness_ = 0.0f;

		if (first_iteration)
		{
			for (int i = 0; i < 12; i++)
			{
				pop.data[i] = iterator->GetDNA().GetData(i);
			}
			pop.id_ = iterator->GetPopID();
			pop.sum_fitness_ += fitfunc;
			populations_.push_back(pop);
			first_iteration = false;
		}
		else
		{
			bool isNewPop = true;
			for (std::vector<Population>::iterator pop_it = populations_.begin(); pop_it != populations_.end(); pop_it++)
			{
				if (pop_it->id_ == iterator->GetPopID())
				{
					for (int i = 0; i < 12; i++)
					{
						pop.data[i] = iterator->GetDNA().GetData(i);
					}
					pop.sum_fitness_ += fitfunc;
					isNewPop = false;
				}
			}
			if (isNewPop)
			{
				for (int i = 0; i < 12; i++)
				{
					pop.data[i] = iterator->GetDNA().GetData(i);
				}
				pop.id_ = iterator->GetPopID();
				pop.sum_fitness_ += fitfunc;
				populations_.push_back(pop);
			}
		}
	}
}

void genetic_algorithm::Selection(std::vector<boid>* boids, int generation)
{
	int file_num = 0;
	int additional_id = 0;

	for (std::vector<boid>::iterator iterator = boids->begin(); iterator != boids->end(); iterator++)
	{
		// Pick a number of the roulette wheel:
		float roulette = rand() % (int)sum_fitness_;

		float segment = 0.0f;
		bool isSegmentFound = false;
		for (std::vector<Population>::iterator pop_it = populations_.begin(); pop_it != populations_.end(); pop_it++)
		{
			segment += pop_it->sum_fitness_;
			// If the random number is in this segment:
			if (roulette <= (int)segment && !isSegmentFound)
			{
				// Crossover:
				int crossover_point = rand() % 12;

				// Combine the parents DNA:
				if (crossover_point != 0)
				{
					float data[12];
					for (int gene = 0; gene < crossover_point; gene++)
					{
						// Pass on the new data up to the crossover point:
						iterator->GetDNA().SetData(gene, pop_it->data[gene]);
					}
				}

				// Mutation: (0.1% chance of mutation)
				if (rand() % 1000 < mutation_rate_)
				{
					int rand_gene = rand() % 12;

					// Produce a + or - 10% change to each gene value:
					if (rand() % 2)
					{
						iterator->GetDNA().SetData(rand_gene, (iterator->GetDNA().GetData(rand_gene) + (iterator->GetDNA().GetData(rand_gene) * 0.1f)));
					}
					else
					{
						iterator->GetDNA().SetData(rand_gene, (iterator->GetDNA().GetData(rand_gene) - (iterator->GetDNA().GetData(rand_gene) * 0.1f)));
					}

					if (iterator->GetDNA().GetData(rand_gene) == 0.0f)
					{
						iterator->GetDNA().SetData(rand_gene, iterator->GetDNA().GetData(rand_gene) + 0.1f);
					}
				}
				iterator->SetPopID(iterator->GetPopID() + "_" + std::to_string(additional_id));
				additional_id++;
				isSegmentFound = true;
			}
		}
		if (!isSegmentFound)
		{	// Fun breakpoints are fun! Meeeeoooowwwww!!
			float just_keep_it = 0.5f;
		}

		// Every tenth generation check up on the data:
		if (generation % 10 == 0)
		{
			std::string txt_directory("GeneticDatatxt/Generation_");
			std::string txt_file_name("/boid_data_");
			std::string txt_base(".txt");

			std::string csv_directory("GeneticDataCSV/Simulation_");
			std::string csv_file_name("/boid_data_");
			std::string csv_base(".csv");


			int check_txt;
			const char* txt_dir = "GeneticDatatxt/Generation_";
			check_txt = _mkdir((txt_dir + std::to_string(generation)).c_str());


			iterator->GetDNA().StoreData(generation,
				txt_directory + std::to_string(generation) + txt_file_name + std::to_string(file_num) + txt_base,
				csv_directory + std::to_string(glo_simulation_number) + csv_file_name + std::to_string(file_num) + csv_base);
			file_num++;
		}
	}
}

void genetic_algorithm::susSelection(std::vector<boid>* boids, int generation)
{
	int file_num = 0;
	int additional_id = 0;

	for (std::vector<boid>::iterator iterator = boids->begin(); iterator != boids->end(); iterator++)
	{
		// Pick a number of the roulette wheel:
		float roulette_1 = rand() % (int)sum_fitness_;
		float roulette_2 = rand() % (int)sum_fitness_;

		float segment1 = 0.0f;
		float segment2 = 0.0f;
		bool isSegmentFound = false;
		for (std::vector<Population>::iterator pop_it = populations_.begin(); pop_it != populations_.end(); pop_it++)
		{
			segment1 += pop_it->sum_fitness_;
			// If the random number is in this segment:
			if (roulette_1 <= (int)segment1 && !isSegmentFound)
			{
				for (std::vector<Population>::iterator pop_it2 = populations_.begin(); pop_it2 != populations_.end(); pop_it2++)
				{
					segment2 += pop_it->sum_fitness_;
					if (roulette_2 <= (int)segment2 && !isSegmentFound)
					{
						// Crossover:
						int crossover_point = rand() % 12;

						// Combine the parents DNA:
						if (crossover_point != 0)
						{
							float data[12];
							for (int gene = 0; gene < crossover_point; gene++)
							{
								// Pass on the new data up to the crossover point:
								pop_it->data[gene] = pop_it2->data[gene];
								
							}
							for (int i = 0; i < 12; i++)
							{
								iterator->GetDNA().SetData(i, pop_it->data[i]);
							}
						}

						// Mutation: (0.1% chance of mutation)
						if (rand() % 1000 < mutation_rate_)
						{
							int rand_gene = rand() % 12;

							// Produce a + or - 10% change to each gene value:
							if (rand() % 2)
							{
								iterator->GetDNA().SetData(rand_gene, (iterator->GetDNA().GetData(rand_gene) + (iterator->GetDNA().GetData(rand_gene) * 0.1f)));
							}
							else
							{
								iterator->GetDNA().SetData(rand_gene, (iterator->GetDNA().GetData(rand_gene) - (iterator->GetDNA().GetData(rand_gene) * 0.1f)));
							}

							if (iterator->GetDNA().GetData(rand_gene) == 0.0f)
							{
								iterator->GetDNA().SetData(rand_gene, iterator->GetDNA().GetData(rand_gene) + 0.1f);
							}
						}
						iterator->SetPopID(iterator->GetPopID() + "_" + std::to_string(additional_id));
						additional_id++;
						isSegmentFound = true;
					}
				}
				
			}
		}
		if (!isSegmentFound)
		{	// Fun breakpoints are fun! Meeeeoooowwwww!!
			float just_keep_it = 0.5f;
		}

		std::string txt_directory("GeneticDatatxt/Generation_");
		std::string txt_file_name("/boid_data_");
		std::string txt_base(".txt");

		std::string csv_directory("GeneticDataCSV/Simulation_");
		std::string csv_file_name("/boid_data_");
		std::string csv_base(".csv");

		int check_txt;
		const char* txt_dir = "GeneticDatatxt/Generation_";
		check_txt = _mkdir((txt_dir + std::to_string(generation)).c_str());

		iterator->GetDNA().StoreData(generation,
			txt_directory + std::to_string(generation) + txt_file_name + std::to_string(file_num) + txt_base,
			csv_directory + std::to_string(glo_simulation_number) + csv_file_name + std::to_string(file_num) + csv_base);
		file_num++;
	}
}
