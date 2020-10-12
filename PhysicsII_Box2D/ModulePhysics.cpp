#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "math.h"

//#include "Box2D/Box2D/Box2D.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));

	// big static circle as "ground" in the middle of the screen
	int x = SCREEN_WIDTH / 2;
	int y = SCREEN_HEIGHT / 1.5f;
	int diameter = SCREEN_WIDTH / 2;
	CreateCircle(x, y, (diameter / 2), b2_staticBody);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	return UPDATE_CONTINUE;
}

// 
update_status ModulePhysics::PostUpdate()
{
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
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
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

void PhysBody::GetPosition(int& x, int& y) const {
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x);
	y = METERS_TO_PIXELS(pos.y);
}

float PhysBody::GetRotation() const {
	return RAD_TO_DEG(body->GetAngle());
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int r, b2BodyType bodyType) {
	b2BodyDef body;
	body.type = bodyType;
	float radius = PIXEL_TO_METERS(r);
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	b->CreateFixture(&fixture);

	PhysBody *physBody = new PhysBody;
	physBody->body = b;
	return physBody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int w, int h, b2BodyType bodyType) {
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	b2Vec2 verticesPoly[4];
	verticesPoly[0].Set(PIXEL_TO_METERS((-w / 2)), PIXEL_TO_METERS((-h / 2)));
	verticesPoly[1].Set(PIXEL_TO_METERS((-w / 2)), PIXEL_TO_METERS((h / 2)));
	verticesPoly[2].Set(PIXEL_TO_METERS((w / 2)), PIXEL_TO_METERS((h / 2)));
	verticesPoly[3].Set(PIXEL_TO_METERS((w / 2)), PIXEL_TO_METERS((-h / 2)));
	int32 countPoly = 4;
	shape.Set(verticesPoly, countPoly);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	b->CreateFixture(&fixture);

	PhysBody* physBody = new PhysBody;
	physBody->body = b;
	return physBody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int points[], b2Vec2 vertices[], int n, b2BodyType bodyType) {
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	for (int k = 0; k < n; k++) {
		vertices[k].Set(PIXEL_TO_METERS((points[k * 2])), PIXEL_TO_METERS((points[k * 2 + 1])));
	}
	shape.CreateLoop(vertices, n);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	b->CreateFixture(&fixture);

	PhysBody* physBody = new PhysBody;
	physBody->body = b;
	return physBody;
}

pentaFloat ModulePhysics::CalculatePentagon(int sideLength) {
	float radius = sideLength / 2 * (1 / sin(b2_pi / 5));
	float X1 = sideLength * sqrt(1 - 1 / ((1 / sin(b2_pi / 5)) * (1 / sin(b2_pi / 5))));
	float Y1 = sqrt((radius * radius) - (X1 * X1));
	float A = (4 * Y1 * Y1) + (4 * X1 * X1);
	float n = ((Y1 * Y1) + (X1 * X1) + (radius * radius) - (sideLength * sideLength));
	float B = -4 * n * Y1;
	float C = (n * n) - (4 * X1 * X1 * radius * radius);
	float discriminant = (B * B) - (4 * A * C);
	float sqrtDisc = sqrt(discriminant);
	float Y2 = (-B - sqrtDisc) / (2 * A);
	float X2 = sqrt((radius * radius) - (Y2 * Y2));
	return { radius, X1, Y1, X2, Y2 };
}

PhysBody* ModulePhysics::CreatePentagon(int x, int y, int sideLength, b2BodyType bodyType) {
	b2BodyDef body;
	body.type = bodyType;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape shape;
	b2Vec2 vertices[5];
	pentaFloat pentagon = CalculatePentagon(sideLength);
	vertices[0].Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS((- pentagon.r)));
	vertices[1].Set(PIXEL_TO_METERS((- pentagon.x1)), PIXEL_TO_METERS((- pentagon.y1)));
	vertices[2].Set(PIXEL_TO_METERS((- pentagon.x2)), PIXEL_TO_METERS((- pentagon.y2)));
	vertices[3].Set(PIXEL_TO_METERS((pentagon.x2)), PIXEL_TO_METERS((- pentagon.y2)));
	vertices[4].Set(PIXEL_TO_METERS((pentagon.x1)), PIXEL_TO_METERS((- pentagon.y1)));
	int32 countPoly = 5;
	shape.Set(vertices, countPoly);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	b->CreateFixture(&fixture);

	PhysBody* physBody = new PhysBody;
	physBody->body = b;
	return physBody;
}