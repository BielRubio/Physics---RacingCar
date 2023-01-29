#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	VehicleInfo car;

	// Car properties ----------------------------------------
	car.chassis_size.Set(1.2, 0.5, 2.0f);
	car.chassis_offset.Set(0, 0.7, 0);
	car.mass = 250.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.5f;
	float wheel_radius = 0.3f;
	float wheel_width = 0.4f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.5f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width + 0.6f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width - 0.6f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width + 0.6f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width - 0.6f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;


	

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(0, 0.3, 0);

	vehicle->collision_listeners.add(this);
	vehicle->SetId(1);

	countdown = 3;
	loseCondition = 20;
	currentTime = 0;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	if (App->scene_intro->state == GAMESTATE::GAMEPLAY) {
		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			float aux[16] = { 1,0,0,0,0,1,0,0.3,0,0,1,0,0,0,0,1 };

			vehicle->SetTransform(aux);
			vehicle->SetLinearVelocity({ 0,0,0 });
		}
		if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
			Respawn();
		}

		turn = acceleration = brake = 0.0f;

		//Initial countdown
		if (countdown >= 0) {
			countdown -= dt;
		}
		else {
			App->scene_intro->currentLap = LAPS::FIRST;
		}		
		if (App->scene_intro->currentLap != LAPS::START) {
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && vehicle->GetKmh() <= 200)
			{
				acceleration = MAX_ACCELERATION;
			}
			App->physics->DragForce(vehicle, 25);

			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				if (vehicle->GetKmh() > 110) {
					if (turn < TURN_DEGREES)
						turn += TURN_DEGREES / 2;
				}
				else if (vehicle->GetKmh() > 90) {
					if (turn < TURN_DEGREES)
						turn += TURN_DEGREES / 1.75;
				}
				else if (vehicle->GetKmh() > 70) {
					if (turn < TURN_DEGREES)
						turn += TURN_DEGREES / 1.5;
				}
				else {
					if (turn < TURN_DEGREES)
						turn += TURN_DEGREES;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				if (vehicle->GetKmh() > 110) {
					if (turn < TURN_DEGREES)
						turn -= TURN_DEGREES / 2;
				}
				else if (vehicle->GetKmh() > 90) {
					if (turn < TURN_DEGREES)
						turn -= TURN_DEGREES / 1.75;
				}
				else if (vehicle->GetKmh() > 70) {
					if (turn < TURN_DEGREES)
						turn -= TURN_DEGREES / 1.5;
				}
				else {
					if (turn < TURN_DEGREES)
						turn -= TURN_DEGREES;
				}
			}
			//Go backwards
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && vehicle->GetKmh() >= -90)
			{
				acceleration = -MAX_ACCELERATION;
			}

			if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			{
				brake = BRAKE_POWER;
			}

			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
				acceleration = MAX_ACCELERATION * 5;
			}
			//Apply friction (in case no acceleration or in case it goes too quick, the vehicle starts loosing speed)

			if (App->input->GetKey(SDL_SCANCODE_UP) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LSHIFT) != KEY_REPEAT) {
				brake = BRAKE_POWER / 100;

				if (vehicle->GetKmh() > 100) {
					brake = BRAKE_POWER / 20;
				}
			}
			vehicle->ApplyEngineForce(acceleration);
			vehicle->Turn(turn);
			vehicle->Brake(brake);

		

			if (loseCondition >= 0) {
				loseCondition -= dt;
			}
			else {
				App->scene_intro->race = RACESTATE::LOSE;
				App->scene_intro->state = GAMESTATE::END;
			}
			currentTime += dt;
		}
		pos = { vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getX(),
				vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getY(),
				vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().getZ(), };

		fwVec = { vehicle->vehicle->getForwardVector().getX(),
				vehicle->vehicle->getForwardVector().getY(),
				vehicle->vehicle->getForwardVector().getZ(), };
		 
	}
	vehicle->Render();

	char title[200];
	if (App->scene_intro->state == GAMESTATE::TITLESCREEN) {
		sprintf_s(title, "Press Space/Enter to start");
	}
	else {
		if (App->scene_intro->currentLap == LAPS::START) {
			if (countdown > 0) {
				sprintf_s(title, "The race will start in %.f seconds", countdown);
			}
			else {
				if (App->scene_intro->race == RACESTATE::LOSE) {
					sprintf_s(title, "YOU LOSE");
				}
				else {
					sprintf_s(title, "RACE STARTED");
				}
			}
		}
		else {
			int cLap = (int)App->scene_intro->currentLap;
			if (cLap == -1) cLap = 0;
			sprintf_s(title, "Speed: %.1f Km/h | Lap: %i/3 | Last lap time: %.2f | Finish time: %.2f| Press F3 to Respawn", vehicle->GetKmh(), cLap, currentTime, loseCondition);
			if (App->scene_intro->currentLap == LAPS::LAST) {
				if (App->scene_intro->race == RACESTATE::WIN) {
					sprintf_s(title, "Speed: %.1f Km/h | Lap: %i/3 | Last lap time: | Finish time: | YOU WIN", vehicle->GetKmh(), cLap);
				}
			}
			if (App->scene_intro->race == RACESTATE::LOSE) {
				sprintf_s(title, "Speed: %.1f Km/h | Lap: %i/3 | Last lap time: | Finish time: | YOU LOSE", vehicle->GetKmh(), cLap);
			}
		}
	}
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}
void ModulePlayer::Respawn() {
	if (lastCheckPoint == 0) {
		vehicle->ApplyEngineForce(0);
		vehicle->Turn(0);
		vehicle->Brake(0);

		vehicle->SetPos(0, 0.3, 0);
		vehicle->Rotate(90);
		vehicle->SetLinearVelocity({ 0,0,0 });
		vehicle->vehicle->getRigidBody()->setAngularVelocity({0,0,0});
		
	}
	else {
		p2List_item<CheckPoint>* checklist = App->scene_intro->checkPoints.getFirst();
		while (checklist != NULL) {
			if (checklist->data.body->id == lastCheckPoint) {
				vehicle->SetPos(checklist->data.body->GetPos().getX(), checklist->data.body->GetPos().getY(), checklist->data.body->GetPos().getZ());
				vehicle->Rotate(checklist->data.angle);
				vehicle->ApplyEngineForce(0);
				vehicle->Turn(0);
				vehicle->Brake(0);
				vehicle->SetLinearVelocity({ 0, 0, 0 });
				vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0,0,0 });
			}
			checklist = checklist->next;
		}
	}
	
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {
	//Collide with the checkpoint added
	if (body2->id == 2) {
		if (App->scene_intro->checkPoints.getFirst()->data.checked == false) {
			//Update of laps
			switch (App->scene_intro->currentLap) {
			case LAPS::FIRST:
				App->scene_intro->currentLap = LAPS::SECOND;
				LOG("second lap start");
				break;
			case LAPS::SECOND:
				App->scene_intro->currentLap = LAPS::LAST;
				LOG("third lap start");
				break;
			case LAPS::LAST:
				App->scene_intro->race = RACESTATE::WIN;
				break;
			}
		}
		App->scene_intro->checkPoints.getFirst()->data.checked = true;
		if (App->scene_intro->checkPoints.getFirst()->next != NULL) {
			App->scene_intro->checkPoints.getFirst()->next->data.checked = false;
		}
		lastCheckPoint = body2->id; 
	}
	else if (body2->id > 2 && body2->id < lastCP) {
		p2List_item<CheckPoint>* checklist = App->scene_intro->checkPoints.getFirst();
		while (checklist != NULL) {
			if (checklist->data.body->id == body2->id) {
				if (lastCheckPoint == checklist->prev->data.body->id) {
					if (checklist->data.checked == false) {
						checklist->data.checked = true;
						if (checklist->next != NULL) {
							checklist->next->data.checked = false;
						}
						lastCheckPoint = body2->id;
						loseCondition += 5;
					}
				}
				
			}
			checklist = checklist->next;
		}	
	}
	else if (body2->id == lastCP) {
		p2List_item<CheckPoint>* checklist = App->scene_intro->checkPoints.getFirst();
		while (checklist != NULL) {
			if (checklist->data.body->id == body2->id) {
				if (lastCheckPoint == checklist->prev->data.body->id) {
					if (checklist->data.checked == false) {
						checklist->data.checked = true;
						App->scene_intro->checkPoints.getFirst()->data.checked = false;
						lastCheckPoint = body2->id;
						loseCondition += 5;
					}
				}
			}
			checklist = checklist->next;
		}
	}
	else if (body2->id == -1) {
		Respawn();
	}
}


