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

	road6 = Cube(20, 0.2, 100);
	road6.SetPos(-180, 5.3, 255+50.6);
	road6.color = { 0.5f,0.5f,0.5f };
	road6.SetRotation(-10, { 1,0,0 });
	pb_road6 = App->physics->AddBody(road6, 0);

	joint1 = Cylinder(0.7, 20);
	joint1.SetPos(-180, 13.4, 355);
	joint1.color = { 0.5f,0.5f,0.5f };
	pb_joint1 = App->physics->AddBody(joint1, 0);

	road7 = Cube(20, 0.2, 150);
	road7.SetPos(-180, 14, 355 + 75);
	road7.color = { 0.5f,0.5f,0.5f };
	pb_road7 = App->physics->AddBody(road7, 0);

	road8 = Cube(50, 0.2, 20);
	road8.SetPos(-180 - 35, 14, 495);
	road8.color = { 0.5f,0.5f,0.5f };
	pb_road7 = App->physics->AddBody(road8, 0);

	road9 = Cube(20, 0.2, 50);
	road9.SetPos(-180 - 50, 14, 495- 35);
	road9.color = { 0.5f,0.5f,0.5f };
	pb_road9 = App->physics->AddBody(road9, 0);

	road10 = Cube(100, 0.2, 20);
	road10.SetPos(-280 - 10, 14, 495 - 50);
	road10.color = { 0.5f,0.5f,0.5f };
	pb_road10 = App->physics->AddBody(road10, 0);

	road11 = Cube(50, 0.2, 50);
	road11.SetPos(-290 - 75, 14, 445 - 15);
	road11.color = { 0.5f,0.5f,0.5f };
	pb_road11 = App->physics->AddBody(road11, 0);

	road12 = Cube(10, 0.2, 50);
	road12.SetPos(-390 + 5, 14, 445 - 40 - 25);
	road12.color = { 0.5f,0.5f,0.5f };
	pb_road12 = App->physics->AddBody(road12, 0);

	road13 = Cube(50, 0.2, 50);
	road13.SetPos(-290 - 75, 14, 330);
	road13.color = { 0.5f,0.5f,0.5f };
	pb_road13 = App->physics->AddBody(road13, 0);

	road14 = Cube(10, 0.2, 50);
	road14.SetPos(-390 + 45, 14, 330 - 50);
	road14.color = { 0.5f,0.5f,0.5f };
	pb_road14 = App->physics->AddBody(road14, 0);

	road15 = Cube(50, 0.2, 50);
	road15.SetPos(-290 - 75, 14, 330 - 100);
	road15.color = { 0.5f,0.5f,0.5f };
	pb_road15 = App->physics->AddBody(road15, 0);

	road16 = Cube(10, 0.2, 20);
	road16.SetPos(-290 - 75, 14, 230 - 35);
	road16.color = { 0.5f,0.5f,0.5f };
	pb_road16 = App->physics->AddBody(road16, 0);

	road17 = Cube(70, 0.2, 70);
	road17.SetPos(-290 - 75, 0, 140);
	road17.color = { 0.5f,0.5f,0.5f };
	pb_road17 = App->physics->AddBody(road17, 0);

	//AddCheckPoint({ 0, 0, 100 }, 90, 20, White, 2, false); // meta
	//AddCheckPoint({ 25, 0, 200 }, 0, 30, Red, 3, false); // meta
	//AddCheckPoint({ -175, 0, 375 }, 180, 20, Red, 4, false); // meta
	//AddCheckPoint({ -300, 0, 300 }, -45, 20, Red, 5, false); // meta

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
	joint1.Render();
	road7.Render();
	road8.Render();
	road9.Render();
	road10.Render();
	road11.Render();
	road12.Render();
	road13.Render();
	road14.Render();
	road15.Render();
	road16.Render();
	road17.Render();
	//road18.Render();
	//road19.Render();



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

