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

	ground = Cube(2000, 0, 2000);
	ground.SetPos(0, 0, 0);
	ground.color = Green;
	pb_ground = App->physics->AddBody(ground, 0);

	road1 = Cube(20, 0.2, 300); //10,150
	road1.SetPos(0, 0, 0);
	road1.color = {0.5f,0.5f,0.5f };
	pb_road1 = App->physics->AddBody(road1, 0);

	road2 = Cube(50, 0.2, 50);
	road2.SetPos(-25 + 10, 0, 150+25); //-15,175
	road2.color = { 0.5f,0.5f,0.5f };
	pb_road2 = App->physics->AddBody(road2, 0);

	road3 = Cube(100, 0.2, 20);
	road3.SetPos(-15 - 75, 0, 175 + 15); // -125, 210
	road3.color = { 0.5f,0.5f,0.5f };
	pb_road3 = App->physics->AddBody(road3, 0);

	road4 = Cube(50, 0.2, 50);
	road4.SetPos(-125 - 40, 0,210 - 5); // -180,230
	road4.color = { 0.5f,0.5f,0.5f };
	pb_road4 = App->physics->AddBody(road4, 0);

	road5 = Cube(20, 0.2, 50);
	road5.SetPos(-180, 0, 230 + 25); // -170,255
	road5.color = { 0.5f,0.5f,0.5f };
	pb_road5 = App->physics->AddBody(road5, 0);

	road6 = Cube(20, 50, 100);
	road6.SetPos(-180, -10, 255+50);
	road6.color = { 0.5f,0.5f,0.5f };
	road6.SetRotation(-20, { 1,0,0 });
	pb_road6 = App->physics->AddBody(road6, 0);


	//AddCheckPoint({ 0, 0, 100 }, 90, 20, White, 2, false); // meta

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

update_status ModuleSceneIntro::Update(float dt)
{
	//Change Cam view
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_STATE::KEY_DOWN) camView = CAM_VIEW::BEHIND_PLAYER;
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_STATE::KEY_DOWN) camView = CAM_VIEW::LOCK_PLAYER;
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_STATE::KEY_DOWN) camView = CAM_VIEW::FREE_CAM;
		
	switch (camView)
// Update
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
	road1.Render();
	road2.Render();
	road3.Render();
	road4.Render();
	road5.Render();
	road6.Render();
	///*road7.Render();
	//road8.Render();
	//road9.Render();
	//road10.Render();*/



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

