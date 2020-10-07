#include "Globals.h"
#include "Application.h"
#include "ModulePhysics.h"
#include "math.h"
#include <stdlib.h>
#include <time.h>

// TODO 1: Include Box 2 header and library
#ifdef _DEBUG
#pragma comment(lib, "Box2D/libx86/Debug/Box2D.lib")
#else
#pragma comment(lib, "Box2D/libx86/Release/Box2D.lib")
#endif // _DEBUG

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	debug = true;
	b2Vec2 gravity(0.0f, 10.0f);
	world = new b2World(gravity);
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	// TODO 2: Create a private variable for the world
	// - You need to send it a default gravity
	// - You need init the world in the constructor
	// - Remember to destroy the world after using it


	// TODO 4: Create a a big static circle as "ground"
	//GIANT CIRCLE
	//b2BodyDef circleBodyDef;
	//circleBodyDef.type = b2_staticBody;
	//circleBodyDef.position.Set(PIXELS_TO_METERS(512), PIXELS_TO_METERS(380));
	//b2Body *circleBody = world->CreateBody(&circleBodyDef);

	//b2CircleShape circle;
	//circle.m_radius = PIXELS_TO_METERS(300);

	//b2FixtureDef fixtureCircle;
	//fixtureCircle.shape = &circle;
	//circleBody->CreateFixture(&fixtureCircle);

	//FLOOR
	b2BodyDef floorBodyDef;
	floorBodyDef.type = b2_staticBody;
	floorBodyDef.position.Set(PIXELS_TO_METERS(0), PIXELS_TO_METERS(0));
	b2Body *floorBody = world->CreateBody(&floorBodyDef);

	b2PolygonShape floor;
	b2Vec2 verticesFloor[4];
	verticesFloor[0].Set(PIXELS_TO_METERS(0), PIXELS_TO_METERS(730));
	verticesFloor[1].Set(PIXELS_TO_METERS(0), PIXELS_TO_METERS(756));
	verticesFloor[2].Set(PIXELS_TO_METERS(1022), PIXELS_TO_METERS(756));
	verticesFloor[3].Set(PIXELS_TO_METERS(1022), PIXELS_TO_METERS(730));
	int32 countFloor = 4;
	floor.Set(verticesFloor, countFloor);
	
	b2FixtureDef fixtureFloor;
	fixtureFloor.shape = &floor;
	floorBody->CreateFixture(&fixtureFloor);

	//POLYGON
	b2BodyDef polyBodyDef;
	polyBodyDef.type = b2_staticBody;
	polyBodyDef.position.Set(PIXELS_TO_METERS(0), PIXELS_TO_METERS(0));
	b2Body* polyBody = world->CreateBody(&polyBodyDef);

	b2PolygonShape poly;
	b2Vec2 verticesPoly[5];
	//verticesPoly[0].Set(PIXELS_TO_METERS(412), PIXELS_TO_METERS(579));
	//verticesPoly[1].Set(PIXELS_TO_METERS(412), PIXELS_TO_METERS(479));
	//verticesPoly[2].Set(PIXELS_TO_METERS(612), PIXELS_TO_METERS(479));
	//verticesPoly[3].Set(PIXELS_TO_METERS(612), PIXELS_TO_METERS(579));
	doubleVec2 pentagon = CreatePentagon(512,379,50);
	verticesPoly[0].Set(PIXELS_TO_METERS(512), PIXELS_TO_METERS((379 - 42.53)));
	verticesPoly[1].Set(PIXELS_TO_METERS((512-40.45)), PIXELS_TO_METERS((379-13.14)));
	verticesPoly[2].Set(PIXELS_TO_METERS((512-22.87)), PIXELS_TO_METERS((379+35.86)));
	verticesPoly[3].Set(PIXELS_TO_METERS((512+22.87)), PIXELS_TO_METERS((379+35.86)));
	verticesPoly[4].Set(PIXELS_TO_METERS((512+40.45)), PIXELS_TO_METERS((379-13.14)));

	int32 countPoly = 5;
	poly.Set(verticesPoly, countPoly);

	b2FixtureDef fixturePoly;
	fixturePoly.shape = &poly;
	polyBody->CreateFixture(&fixturePoly);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	// TODO 3: Update the simulation ("step" the world)
	world->Step(1.0f / 60.0f, 8, 3);
	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
	// TODO 5: On space bar press, create a circle on mouse position
	// - You need to transform the position / radius
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		b2BodyDef dynamicBodyDef;
		dynamicBodyDef.type = b2_dynamicBody;
		dynamicBodyDef.position.Set(PIXELS_TO_METERS(App->input->GetMouseX()), PIXELS_TO_METERS(App->input->GetMouseY()));
		b2Body* dynamicBody = world->CreateBody(&dynamicBodyDef);

		srand(time(NULL));
		int r = rand() % 40 + 11;
		b2CircleShape circle;
		circle.m_radius = PIXELS_TO_METERS(r); //20

		b2FixtureDef fixture;
		fixture.shape = &circle;
		dynamicBody->CreateFixture(&fixture);
	}

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	
	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;
				case b2Shape::e_polygon:
				{
					b2PolygonShape* shape = (b2PolygonShape*)f->GetShape();
					for (int k = 0; k < shape->m_count; k++) {
						b2Vec2 ver1 = shape->m_vertices[k];
						b2Vec2 ver2;
						if ((k+1) == shape->m_count) {
							LOG("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
							ver2 = shape->m_vertices[0];
						}
						else {
							ver2 = shape->m_vertices[k + 1];
						}
						App->renderer->DrawLine(METERS_TO_PIXELS(ver1.x), METERS_TO_PIXELS(ver1.y), METERS_TO_PIXELS(ver2.x), METERS_TO_PIXELS(ver2.y), 255, 255, 255);
					}
				}
				break;
				// You will have to add more cases to draw boxes, edges, and polygons ...
			}
		}
	}

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

doubleVec2 ModulePhysics::CreatePentagon( int topX, int topY, int sideLength) {
	float radius = /*(1 / 10 *sqrt(25 + 10*sqrt(5))*sideLength)*/ 34.40954801;
	int rightTopX = /*sin(2 * PI / 5)*/ 0.9510565163 * radius;
	int rightTopY = /*cos(2 * PI / 5)*/ 0.3090169944 * radius;
	int rightBotX = /*sin(4 * PI / 5)*/ 0.5877852523 * radius;
	int rightBotY = /*cos(PI / 5)*/ 0.8090169944 * radius;
	return { rightTopX, rightTopY, rightBotX, rightBotY };
}