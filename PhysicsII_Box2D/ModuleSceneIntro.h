#pragma once
#include "Module.h"
#include "p2List.h"
#include "Globals.h"
//#include "ModulePhysics.h"

#define MAX_CIRCLES 200
#define MAX_RECTANGLES 200
#define MAX_RICKS 200
#define MAX_DRAGONS 200

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	PhysBody* circles[MAX_CIRCLES];
	PhysBody* rectangles[MAX_RECTANGLES];
	PhysBody* ricks[MAX_RICKS];
	PhysBody* dragons[MAX_DRAGONS];
	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* dragon;
};
