#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !
#define RAD_PER_DEG ((float)180/b2_pi)

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)
#define RAD_TO_DEG(r) ((float) RAD_PER_DEG * r)

//class b2World;
//class b2Body;
//class b2Vec2;
//enum b2BodyType;


// TODO 6: Create a small class that keeps a pointer to tghe b2Body
// and has a method to request the position
// then write the implementation in the .cpp
// Then make your circle creation function to return a pointer to that class
class PhysBody {
public:
	PhysBody() : body(NULL) {};
	void PhysBody::GetPosition(int& x, int& y) const;
	float GetRotation() const;
	b2Body* body;
};

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

	// TODO 4: Move body creation to 3 functions to create circles, rectangles and chains
	PhysBody* CreateCircle(int x, int y, int r, b2BodyType bodyType);
	PhysBody* CreateRectangle(int x, int y, int w, int h, b2BodyType bodyType);
	PhysBody* CreateChain(int x, int y, int points[], b2Vec2 vertices[], int n, b2BodyType bodyType);
	pentaFloat CalculatePentagon(int sideLength);
	PhysBody* CreatePentagon(int x, int y, int sideLength, b2BodyType bodyType);

private:

	bool debug;
	b2World* world;
};