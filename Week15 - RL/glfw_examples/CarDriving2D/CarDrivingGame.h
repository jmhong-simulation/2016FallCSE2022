/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once
#pragma once

#include "GLSquare.h"
#include "GLLineSegments.h"
#include "SelfDrivingCar.h"
#include "VectorND.h"

class CarDrivingGame
{
public:
	SelfDrivingCar my_car;
	std::vector<std::unique_ptr<GLObject>> obj_list;
	int prev_action_;

	//TODO: move these to RLGame mother class
	bool compat_state_ = true; // false: image state for conv net
	VectorND<float> state_buffer_;

	void init()
	{
		my_car.init();

		// world (-1.0, -0.5) x (2.0, 1.5)
		//obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(0.5f, 0.5f, 0.0f), 1.5f, 1.0f)))); // world
		//obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(1.2f, 0.5f, 0.0f), 0.1f, 0.05f))));
		//obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(-0.3f, 0.5f, 0.0f), 0.1f, 0.2f))));
		//obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(0.3f, 1.1f, 0.0f), 0.3f, 0.05f))));
		//obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(0.6f, -0.25f, 0.0f), 0.3f, 0.25f))));
		//
		//obj_list.push_back(std::move(std::unique_ptr<GLSquare>(new GLSquare(glm::vec3(0.5f, 0.5f, 0.0f), 1.2f, 0.7f))));

		{
			GLObject *temp = new GLObject;
			temp->initCircle(glm::vec3(0.5f, 0.5f, 0.0f), 1.0f, 30);
			obj_list.push_back(std::move(std::unique_ptr<GLObject>(temp)));
		}

		{
			GLObject *temp = new GLObject;
			temp->initCircle(glm::vec3(0.5f, 0.5f, 0.0f), 0.65f, 30);
			obj_list.push_back(std::move(std::unique_ptr<GLObject>(temp)));
		}

		//TODO: move to RLGame
		state_buffer_.initialize(getNumStateVariables(), true);
		state_buffer_.assignAllValues(1.0f);
	}

	void processInput(const int& action)
	{
		prev_action_ = action;

		switch (action)
		{
		case 0:
			my_car.turnLeft(); // right
			break;
		case 1:
			my_car.turnRight(); // left
			break;
		case 2:
			// stay
			break;
		default:
			std::cout << "Wrong action " << std::endl;
			exit(1);
			break;
		}
		// always accel in this example
		my_car.accel();

		/*switch (action)
		{
		case 0:
			my_car.turnLeft();
			break;
		case 1:
			my_car.turnRight();
			break;
		case 2:
			my_car.accel();
			break;
		case 3:
			my_car.decel();
			break;
		default:
			std::cout << "Wrong action " << std::endl;
			exit(1);
			break;
		}*/
	}

	void update(const bool& update_render_data, float& reward, int& flag) // flag = 0 : continue, 1 : terminal
	{
		flag = 0;
		reward = 0.0f;
		
		my_car.update();
		my_car.updateSensor(obj_list, update_render_data);

		// velocity reward
		const float speed = glm::dot(my_car.vel_, my_car.dir_);
		const float max_speed = 0.01f;
		//reward = speed / max_speed;
		reward = 0.1f; // constant reward for this example

		// collision check
		glm::vec3 col_line_center;
		if (my_car.car_body.checkCollisionLoop(obj_list, col_line_center) == true)
		{
			static int count = 0;

			//std::cout << "Collision " << count++ << std::endl;

			// reset car
			// collision response
			/*glm::vec3 ref_nor = glm::normalize(my_car.car_body.center_ - col_line_center);
			const glm::vec3 dp = ref_nor * 0.001f;
			my_car.car_body.center_ += dp;
			my_car.car_body.model_matrix_ = glm::translate(dp) * my_car.car_body.model_matrix_;
			my_car.vel_ = glm::vec3(0);*/

			// reset car status
			my_car.init();
			my_car.car_body.model_matrix_ = glm::mat4();

			reward = 0.0f;	// no reward
			flag = 1;		// terminal //TODO: use enum 
		}

		// car-world escape check
		//for (auto itr : my_car.car_body.vertices)
		//{
		//	const glm::vec4 v4 = my_car.car_body.model_matrix_ * glm::vec4(itr.x, itr.y, itr.z, 1.0f);

		//	if (world_bb.isInside(v4.x, v4.y) == false) std::cout << "World outside " << v4.x << " " << v4.y << std::endl;
		//}
	}

	//TODO: move these to RLGame mother class
	int getNumStateVariables()
	{
		if (compat_state_ == true) return my_car.distances_from_sensors_.size(); // sensor inputs
		else
		{
			std::cout << "getNumStateVariables" << std::endl;
			exit(1);
			return 0;
		}
	}

	int getNumActions()
	{
		return 3; // left, right, stay
	}

	const VectorND<float>& getStateBuffer() // update state buffer and return it
	{
		if (compat_state_ == true)
		{
			for (int i = 0; i < my_car.distances_from_sensors_.size(); i++)
			{
				//state_buffer_[i] = CLAMP(1.0 - my_car.distances_from_sensors_[i], 0.0f, 1.0f);
				state_buffer_[i] = my_car.distances_from_sensors_[i]; //Note: most of distances are larger than 1. Don't clamp.
				//state_buffer_[i] = my_car.distances_from_sensors_[i] > 0.7 ? 1.0f : 0.0f;

				//std::cout << state_buffer_[i] << " ";
			}

			//std::cout << std::endl;
		}
		else
		{
			std::cout << "getStateBuffer()" << std::endl;
			exit(1);
			//for (int j = 0; j < SCR_HEIGHT; j++)
			//	for (int i = 0; i < SCR_WIDTH; i++)
			//	{
			//		float value = 0.0;

			//		if (renderer_.front_buffer[j][i] == '*') value = 1.0;
			//		else if (renderer_.front_buffer[j][i] == '-') value = 0.5;

			//		state_buffer_[i + SCR_WIDTH*j] = value;
			//	}
		}

		return state_buffer_;
	}
};