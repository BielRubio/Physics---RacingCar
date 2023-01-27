#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	//Initialize variables

	camView = (CAM_VIEW)0;

	//Initialize map

	ground = Cube(100, 0, 100);
	ground.SetPos(0, 0, 0);
	ground.color = Green;


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	//Change Cam view
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_STATE::KEY_DOWN) camView = CAM_VIEW::BEHIND_PLAYER;
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_STATE::KEY_DOWN) camView = CAM_VIEW::LOCK_PLAYER;
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_STATE::KEY_DOWN) camView = CAM_VIEW::FREE_CAM;
		
	switch (camView)
	{
	case CAM_VIEW::BEHIND_PLAYER:
		App->camera->LookAt({ App->player->pos.x + App->player->fwVec.x * 10, App->player->pos.y, App->player->pos.z + App->player->fwVec.z * 10 });
		App->camera->Position = { App->player->pos.x - App->player->fwVec.x * 12,App->player->pos.y + 7,App->player->pos.z - App->player->fwVec.z * 12 };
		break;
	case CAM_VIEW::LOCK_PLAYER:
		App->camera->LookAt(App->player->pos);
		break;
	}
	

	

	//Render map

	ground.Render();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

