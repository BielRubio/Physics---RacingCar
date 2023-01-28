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


	AddCheckPoint({ 0, 0, 100 }, 90, 20, White, 2, false); // meta
	AddCheckPoint({ 25, 0, 200 }, 0, 30, Red, 3, false); // meta
	AddCheckPoint({ -175, 0, 375 }, 180, 20, Red, 4, false); // meta
	AddCheckPoint({ -300, 0, 300 }, -45, 20, Red, 5, false); // meta

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

	p2List_item<CheckPoint>* c = checkPoints.getFirst();
	while (c != NULL) {
		if (c->data.checked == false) {
			c->data.colorBody.Render();
		}
		c = c->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	LOG("COLLIDE WITH CHECKPOINT");
}

void ModuleSceneIntro::AddCheckPoint(vec3 position, float angle, float width, Color color, int id, bool startChecked) {
	//Sensor
	Cube sensor;
	sensor.size = { 5,5,width };
	sensor.SetPos(position.x, position.y + 3, position.z);
	sensor.SetRotation(angle, { 0, 1, 0 });

	float radius = width / 2;
	vec3 positionLeftFlag(0, position.y + 2.9, radius);
	vec3 positionRightFlag(0, position.y + 2.9, -radius);
	float theta = angle * M_PI / 180;
	positionLeftFlag.x += radius * sin(theta); positionLeftFlag.z = positionLeftFlag.z * cos(theta);
	positionRightFlag.x -= radius * sin(theta); positionRightFlag.z = positionRightFlag.z * cos(theta);

	// Sensor left mark
	Cylinder leftFlag;
	leftFlag.radius = 2;
	leftFlag.height = 5;
	leftFlag.color = color;
	leftFlag.SetPos(positionLeftFlag.x + position.x, positionLeftFlag.y - 1, positionLeftFlag.z + position.z);
	leftFlag.SetRotation(90, { 0, 0, 1 });

	// Sensor right mark
	Cylinder rightFlag;
	rightFlag.radius = 2;
	rightFlag.height = 5;
	rightFlag.color = color;
	rightFlag.SetPos(positionRightFlag.x + position.x, positionRightFlag.y - 1, positionRightFlag.z + position.z);
	rightFlag.SetRotation(90, { 0, 0, 1 });
	// Create Checkpoint
	CheckPoint sensorCP;
	sensorCP.body = App->physics->AddBody(sensor, 0.0f);
	sensorCP.body->SetAsSensor(true);
	sensorCP.body->SetId(id);
	sensorCP.angle = angle;
	sensorCP.checked = startChecked;
	sensorCP.leftC = leftFlag;
	sensorCP.rightC = rightFlag;
	sensorCP.colorBody = sensor;
	sensorCP.colorBody.color = color;

	Cylinders.add(sensorCP.leftC);
	App->physics->AddBody(sensorCP.leftC, 0);
	Cylinders.add(sensorCP.rightC);
	App->physics->AddBody(sensorCP.rightC, 0);

	checkPoints.add(sensorCP);
}

