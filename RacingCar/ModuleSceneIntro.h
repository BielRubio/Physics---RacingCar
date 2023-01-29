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

enum class LAPS {
	START = -1,
	FIRST,
	SECOND,
	LAST,
	COMPLETE
};

enum class RACESTATE {
	WIN,
	LOOSE,
	DEFAULT
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

	//MAP
	PhysBody3D* pb_ground;
	Cube ground;

	//road
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
	//^^^
	PhysBody3D* pb_joint1;
	Cylinder joint1;
	//vvv
	PhysBody3D* pb_road7;
	Cube road7;
	PhysBody3D* pb_road8;
	Cube road8;
	PhysBody3D* pb_road9;
	Cube road9;
	PhysBody3D* pb_road10;
	Cube road10;
	PhysBody3D* pb_road11;
	Cube road11;
	PhysBody3D* pb_road12;
	Cube road12;
	PhysBody3D* pb_road13;
	Cube road13;
	PhysBody3D* pb_road14;
	Cube road14;
	PhysBody3D* pb_road15;
	Cube road15;
	PhysBody3D* pb_road16;
	Cube road16;
	PhysBody3D* pb_road17;
	Cube road17;
	PhysBody3D* pb_road18;
	Cube road18;
	PhysBody3D* pb_road19;
	Cube road19;
	PhysBody3D* pb_road20;
	Cube road20;
	PhysBody3D* pb_road21;
	Cube road21;
	PhysBody3D* pb_road22;
	Cube road22;
	PhysBody3D* pb_road23;
	Cube road23;
	PhysBody3D* pb_road24;
	Cube road24;
	//^^^
	PhysBody3D* pb_roundab1;
	Cylinder roundab1;
	PhysBody3D* pb_barrier1;
	Cube barrier1;
	//vvv
	PhysBody3D* pb_road25;
	Cube road25;
	PhysBody3D* pb_road26;
	Cube road26;
	PhysBody3D* pb_road27;
	Cube road27;
	PhysBody3D* pb_road28;
	Cube road28;
	PhysBody3D* pb_road29;
	Cube road29;
	PhysBody3D* pb_road30;
	Cube road30;
	PhysBody3D* pb_road31;
	Cube road31;
	PhysBody3D* pb_road32;
	Cube road32;

	//Finish
	PhysBody3D* pb_fpoleL;
	Cylinder fpoleL;
	PhysBody3D* pb_fpoleR;
	Cylinder fpoleR;

	PhysBody3D* pb_flag;
	Cube flag;


	RACESTATE race;

	LAPS currentLap;

	Timer timer;


};
