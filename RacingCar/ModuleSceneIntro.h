#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

#define IMMOVABLE 100000

enum class CAM_VIEW{
	BEHIND_PLAYER = 0,
	LOCK_PLAYER,
	FREE_CAM
};

struct PhysBody3D;
struct PhysMotor3D;

struct CheckPoint {
	PhysBody3D* body;
	Cylinder leftC, rightC;
	Cube colorBody;
	float angle;
	bool checked;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void AddCheckPoint(vec3 position, float angle, float width, Color color, int id, bool startChecked);

public:

	p2List<CheckPoint> checkPoints;
	p2List<Cylinder> Cylinders;
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;
	
	//Cam settings

	CAM_VIEW camView;

	//Map
	PhysBody3D* pb_ground;
	Cube ground;

	PhysBody3D* pb_road1;
	Cube road1;
	PhysBody3D* pb_road2;
	Cube road2;
	PhysBody3D* pb_road3;
	Cube road3;
	PhysBody3D* pb_road4;
	Cube road4;
	PhysBody3D* pb_road5;
	Cube road5;
	PhysBody3D* pb_road6;
	Cube road6;
	PhysBody3D* pb_road7;
	Cube road7;
	PhysBody3D* pb_road8;
	Cube road8;
	PhysBody3D* pb_road9;
	Cube road9;
	PhysBody3D* pb_road10;
	Cube road10;

};
