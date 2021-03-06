#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include <system/application.h>
#include <graphics/sprite.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include <vector>
#include <graphics/mesh_instance.h>

#include "genetic_algorithm.h"
#include "flock.h"
#include "food.h"
#include "app_camera.h"

namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
}

class flocking_app : public gef::Application
{
public:
	flocking_app(gef::Platform& platform);
	~flocking_app();

	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;

	float fps_;

	class gef::Renderer3D* renderer_3d_;

	gef::Sprite* environment_;

	flock* flock_1_;
	gef::Vector2 flock_1_pos_;
	flock* flock_2_;
	gef::Vector2 flock_2_pos_;

	food* food_;
	int resource_count_;

	genetic_algorithm* genetic_algorithm_;
	bool genetic_improvement_;
	int generation_;
	// Identify how long it takes to run the GA:
	typedef std::chrono::high_resolution_clock clock_;
	bool ga_timecheck_;

	app_camera cam_1_;
};

