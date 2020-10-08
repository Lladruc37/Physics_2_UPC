#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

class pentaFloat {
public:
	float r = 0;
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;
};

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();
	//The length of one of its sides
	pentaFloat CalculatePentagon(int sideLength);
	//The (x,y) coordinates of the center of the pentagon
	//The length of one of its sides
	//The type of body (static, kinematic, dynamic)
	void CreatePentagon(int x, int y, int sideLength, b2BodyType bodyType);

private:
	bool debug;
	b2World *world;
	b2Vec2 gravity;
};