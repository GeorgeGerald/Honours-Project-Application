#pragma once

#include <vector>
#include <math.h>
#include <thread>
#include <maths/vector2.h>

#include "Globals.h"

#include "boid.h"
#include "food.h"
#include "genetic_algorithm.h"
#include "DNA.h"

class flock
{
public:
	flock(gef::Platform&, bool);
	~flock();

	void Initialise(gef::Vector2);
	void Update(std::vector<boid>*, std::vector<resource>*, float);
	void CleanUp();

	void Reset(int);
	void GAReset(float, int);

	void RunBoidsAlgorithm(float);

	void PhysicsCalculations(std::vector<boid>::iterator, gef::Vector2, float);

	int GetFlockSize() { return flock_size_; }
	float GetFlockHealth() { return flock_health_; }

	std::vector<boid> boids_;
	

private:
	// References to other entities:
	std::vector<resource> *resources_;
	std::vector<boid> *enemy_boids_;

	gef::Vector2 Cohesion(gef::Vector2, gef::Vector2, int);
	gef::Vector2 Alignment(gef::Vector2, gef::Vector2, gef::Vector2, int);
	gef::Vector2 Separation(gef::Vector2, gef::Vector2, int);
	gef::Vector2 FoodAttraction(std::vector<boid>::iterator);
	gef::Vector2 FlockAvoidance(std::vector<boid>::iterator);
	gef::Vector2 Boundary(std::vector<boid>::iterator);

	gef::Vector2 GACohesion(DNA, gef::Vector2, gef::Vector2, int);
	gef::Vector2 GAAlignment(DNA, gef::Vector2, gef::Vector2, gef::Vector2, int);
	gef::Vector2 GASeparation(DNA, gef::Vector2, gef::Vector2, int);
	gef::Vector2 GAFoodAttraction(std::vector<boid>::iterator);
	gef::Vector2 GAFlockAvoidance(std::vector<boid>::iterator);
	gef::Vector2 GABoundary(std::vector<boid>::iterator);

	bool CollisionDetection(float, float);

	int flock_size_;

	float flock_health_;

	static float food_detection_sqr_;
	static float interaction_distance_sqr_;

	// Limits
	static float max_force_, max_speed_;
	static float bound_x_, bound_y_;

	gef::Platform& platform_;

	// Genetic Algorithm Specific Area:
	bool GA_enabled_;
	genetic_algorithm* genetic_algorithm_;

	gef::Vector2 reset_pos_;
};

