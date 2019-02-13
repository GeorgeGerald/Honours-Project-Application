#pragma once

#include <vector>
#include <math.h>
#include <maths/vector2.h>

#include "boid.h"

class flock
{
public:
	flock(gef::Platform& platform);
	~flock();

	void Initialise(gef::Vector2 flock_centre, int flock_size);
	void Update(float frame_time);
	void CleanUp();

	void RunBoidsAlgorithm(float frame_time);

	void PhysicsCalculations(std::vector<boid>::iterator iterator_, gef::Vector2 accel, float frame_time);
	gef::Vector2 AvoidBoundary(std::vector<boid>::iterator iterator_, gef::Vector2 accel, float frame_time);

	int GetFlockSize() { return flock_size_; }

	std::vector<boid> boids_;

private:
	int flock_size_;

	float desired_separation_;
	float interaction_distance_;
	// Limits
	static float max_force_, max_speed_;
	// Reynolds Weights
	float sep_wgt_, coh_wgt_, ali_wgt_;

	gef::Platform& platform_;
};

