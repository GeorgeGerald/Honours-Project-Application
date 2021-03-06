#include "DNA.h"

#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <direct.h>

DNA::DNA() :
	coh_mult_			(&data_[0]),
	coh_div_mult_		(&data_[1]),
	ali_mult_			(&data_[2]),
	ali_div_mult_		(&data_[3]),
	sep_mult_			(&data_[4]),
	sep_neigh_mult_		(&data_[5]),
	sep_clo_neigh_mult_	(&data_[6]),
	food_mult_1_		(&data_[7]),
	food_mult_2_		(&data_[8]),
	food_div_mult_		(&data_[9]),
	floav_mult_			(&data_[10]),
	floav_div_mult_		(&data_[11])
{
}

DNA::~DNA()
{
}
void DNA::CleanUp()
{
	coh_mult_ = nullptr;
	coh_div_mult_ = nullptr;
	ali_mult_ = nullptr;
	ali_div_mult_ = nullptr;
	sep_mult_ = nullptr;
	sep_neigh_mult_ = nullptr;
	sep_clo_neigh_mult_ = nullptr;
	food_mult_1_ = nullptr;
	food_mult_2_ = nullptr;
	food_div_mult_ = nullptr;
	floav_mult_ = nullptr;
	floav_div_mult_ = nullptr;
}

void DNA::StoreData(int generation, std::string txt_file, std::string csv_data)
{
	if ((txt_file.substr(txt_file.find_last_of(".") + 1) == ".txt")
		|| (csv_data.substr(csv_data.find_last_of(".") + 1) == ".csv"))
	{
		float whyohwhy = 0;
	}

	std::ofstream stored_data;
	stored_data.open(txt_file);
	if (stored_data.fail())
	{
		float whyohwhy = 0;
	}
	for (int i = 0; i < 12; i++)
	{
		stored_data << data_[i] << "\n";
	}
	stored_data.close();

	std::ofstream excel_data;
	excel_data.open(csv_data);
	if (excel_data.fail())
	{
		float whyohwhy = 0;
	}
	for (int i = 0; i < 12; i++)
	{
		excel_data[generation] << data_[i] << ",\n";
	}
	excel_data.close();
}

void DNA::ReadData(std::string data_file)
{
	if (data_file.substr(data_file.find_last_of(".") + 1) == ".txt")
	{
		float whyohwhy = 0;
	}

	std::ifstream stored_data;
	stored_data.open(data_file);
	if (stored_data.fail())
	{
		float whyohwhy = 0;
	}

	if (stored_data.is_open())
	{
		for (int i = 0; i < 12; i++)
		{
			stored_data >> data_[i];
		}
	}
}


