#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

class doubleVec2 {
public:
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
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
	doubleVec2 CreatePentagon(int topX, int topY, int sideLength);

private:
	bool debug;
	b2World *world;
	b2Vec2 gravity;
};