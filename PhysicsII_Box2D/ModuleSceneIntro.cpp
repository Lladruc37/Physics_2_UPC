#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = box = rick = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png"); 
	box = App->textures->Load("pinball/crate.png");
	rick = App->textures->Load("pinball/rick_head.png");
	dragon = App->textures->Load("pinball/dragon.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	// TODO 5: Move all creation of bodies on 1,2,3 key press here in the scene
		// On space bar press, create a circle on mouse position
	if (App->input->GetKey(SDL_SCANCODE_1) /*== KEY_DOWN*/)
	{
		for (int k = 0; k < MAX_CIRCLES; k++) {
			if (circles[k] == NULL) {
				circles[k] = App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25, b2_dynamicBody);
				break;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_2) /*== KEY_DOWN*/)
	{
		// TODO 1: When pressing 2, create a box on the mouse position
		// TODO 2: To have the box behave normally, set fixture's density to 1.0f
		for (int k = 0; k < MAX_RECTANGLES; k++) {
			if (rectangles[k] == NULL) {
				rectangles[k] = App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50, b2_dynamicBody);
				break;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_3) /*== KEY_DOWN*/)
	{
		// TODO 3: Create a chain shape using those vertices
		// remember to convert them from pixels to meters!
		int rick_head[84] = {
			46, 133,
			54, 143,
			62, 148,
			73, 149,
			85, 148,
			93, 142,
			97, 134,
			99, 127,
			105, 126,
			106, 121,
			100, 117,
			102, 105,
			111, 100,
			105, 93,
			109, 89,
			110, 82,
			109, 75,
			117, 67,
			106, 64,
			103, 59,
			112, 35,
			94, 41,
			94, 24,
			88, 4,
			82, 20,
			75, 32,
			59, 15,
			41, 0,
			44, 20,
			43, 40,
			28, 39,
			14, 36,
			31, 64,
			24, 66,
			1, 77,
			30, 91,
			22, 97,
			10, 103,
			33, 115,
			23, 125,
			42, 125,
			36, 137
		};
		b2Vec2 vertices[42];
		for (int k = 0; k < MAX_RICKS; k++) {
			if (ricks[k] == NULL) {
				ricks[k] = App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, vertices, 42, b2_dynamicBody);
				break;
			}
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_4) /*== KEY_DOWN*/)
	{
		for (int k = 0; k < MAX_DRAGONS; k++) {
			if (dragons[k] == NULL) {
				dragons[k] = App->physics->CreatePentagon(App->input->GetMouseX(), App->input->GetMouseY(), 50, b2_dynamicBody);
				break;
			}
		}
	}
	// TODO 7: Draw all the circles using "circle" texture
	for (int k = 0; circles[k] != NULL; k++) {
		int x, y;
		circles[k]->GetPosition(x, y);
		b2CircleShape* circleShape = (b2CircleShape*)circles[k]->body->GetFixtureList()->GetShape();
		int r = METERS_TO_PIXELS(circleShape->m_radius);
		App->renderer->Blit(circle, x - r, y - r, NULL, 1.0f, circles[k]->GetRotation());
	}
	for (int k = 0; rectangles[k] != NULL; k++) {
		int x, y;
		rectangles[k]->GetPosition(x, y);
		b2PolygonShape* rectangleShape = (b2PolygonShape*)rectangles[k]->body->GetFixtureList()->GetShape();
		b2Vec2* vertices = rectangleShape->m_vertices;
		App->renderer->Blit(box,(x - METERS_TO_PIXELS(vertices[0].x)),(y + METERS_TO_PIXELS(vertices[0].y)), NULL, 1.0f, rectangles[k]->GetRotation());
	}
	for (int k = 0; ricks[k] != NULL; k++) {
		int x, y;
		ricks[k]->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, ricks[k]->GetRotation());
	}
	for (int k = 0; dragons[k] != NULL; k++) {
		int x, y;
		dragons[k]->GetPosition(x, y);
		b2PolygonShape* pentagonShape = (b2PolygonShape*)dragons[k]->body->GetFixtureList()->GetShape();
		b2Vec2* vertices = pentagonShape->m_vertices;
		App->renderer->Blit(dragon, (x - METERS_TO_PIXELS(vertices[0].x)), (y + METERS_TO_PIXELS(vertices[4].y)), NULL, 1.0f, dragons[k]->GetRotation());
	}

	return UPDATE_CONTINUE;
}
