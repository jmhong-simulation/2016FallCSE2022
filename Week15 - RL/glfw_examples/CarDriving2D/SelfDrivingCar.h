/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

class SelfDrivingCar
{
public:
	GLSquare car_body;
	GLLineSegments sensing_lines;
	
	glm::vec3 dir_, vel_;

	float turn_coeff_ = 2.0;
	float accel_coeff_ = 0.0001;
	float fric = 0.01;
	float sensing_radius = 1.0;

	std::vector<float> distances_from_sensors_;

	int sensor_min = -40, sensor_max = 40, sensor_di = 20;

	SelfDrivingCar()
	{
	}

	void init()
	{
		car_body.update(glm::vec3(0.6, -0.3, 0.0f), 0.1f, 0.05f);
		//car_body.update(glm::vec3(0.5, 0.5, 0.0f), 0.1f, 0.05f);

		dir_ = glm::vec3(1.0f, 0.0f, 0.0f);
		vel_ = glm::vec3(0.01f, 0.0f, 0.0f);

		int count = 0;
		for (int i = sensor_min; i <= sensor_max; i += sensor_di)
			count++;

		distances_from_sensors_.resize(count);
	}

	void turnLeft()
	{
		const glm::mat4 rot_mat = glm::rotate(glm::mat4(), glm::radians(turn_coeff_), glm::vec3(0, 0, 1));

		glm::vec4 temp(dir_.x, dir_.y, dir_.z, 0.0f);

		temp = rot_mat * temp;

		dir_.x = temp.x;
		dir_.y = temp.y;

		car_body.rotateCenteredZAxis(turn_coeff_);

		float x = 1.0;

		if (glm::dot(vel_, dir_) < 0.0) x = -1.0;

		vel_ = dir_ * sqrt(glm::dot(vel_, vel_)) * x;
	}

	void turnRight()
	{
		const glm::mat4 rot_mat = glm::rotate(glm::mat4(), glm::radians(-turn_coeff_), glm::vec3(0, 0, 1));

		glm::vec4 temp(dir_.x, dir_.y, dir_.z, 0.0f);

		temp = rot_mat * temp;

		dir_.x = temp.x;
		dir_.y = temp.y;

		car_body.rotateCenteredZAxis(-turn_coeff_);

		float x = 1.0;

		if (glm::dot(vel_, dir_) < 0.0) x = -1.0;

		vel_ = dir_ * sqrt(glm::dot(vel_, vel_)) * x;
	}

	void accel()
	{
		vel_ += accel_coeff_ * dir_;
	}

	void decel()
	{
		vel_ -= accel_coeff_ * dir_;
	}

	void update()
	{
		vel_ *= (1.0f - fric);

		car_body.model_matrix_ = glm::translate(vel_) * car_body.model_matrix_;

		car_body.center_ += vel_; //TODO: update model_matrix AND center?
	}
	
	void updateSensor(const std::vector<std::unique_ptr<GLObject>>& obj_list, const bool& update_gl_obj = true)// parameter -> object list
	{
		// sensor sensing_lines (distance from car view point)
		std::vector<glm::vec3> sensor_lines;
		const glm::vec3 center = car_body.center_;
		const float radius = sensing_radius;
		
		for (int i = sensor_min, count = 0; i <= sensor_max; i += sensor_di, count ++)
		{
			glm::vec4 end_pt = glm::vec4(radius*cos(glm::radians((float)i)), radius*-sin(glm::radians((float)i)), 0.0f, 0.0f);
			end_pt = car_body.model_matrix_ * end_pt;

			const glm::vec3 r = center + glm::vec3(end_pt.x, end_pt.y, end_pt.z);

			int flag = 0;
			glm::vec3 col_pt;
			float t;

			// find closest collision pt
			{
				float min_t = 1e8;

				for (int o = 0; o < obj_list.size(); o++)
				{
					int flag_temp;
					float t_temp;
					glm::vec3 col_pt_temp;

					obj_list[o]->checkCollisionLoop(center, r, flag_temp, t_temp, col_pt_temp);

					if (flag_temp == 1 && t_temp < min_t)
					{
						t = t_temp;
						min_t = t_temp;
						col_pt = col_pt_temp;
						flag = flag_temp;
					}
				}
			}
			
			if (flag == 1)
			{
				sensor_lines.push_back(center);
				sensor_lines.push_back(col_pt);

				distances_from_sensors_[count] = sqrt(glm::dot(col_pt-center, col_pt-center));
			}
			else
			{
				sensor_lines.push_back(center);
				sensor_lines.push_back(r);

				distances_from_sensors_[count] = 1.0f;
			}
		}

		if(update_gl_obj == true)
			sensing_lines.update(sensor_lines);
		//sensing_lines.center_ = car_body.center_;
	}

	//void updateSensor(const GLSquare& my_square2)// parameter -> object list
	//{
	//	// sensor sensing_lines (distance from car view point)
	//	std::vector<glm::vec3> sensor_lines;
	//	const glm::vec3 center = car_body.center_;
	//	const float radius = 1.0;
	//	for (int i = 0; i < 360; i += 10)
	//	{
	//		glm::vec4 end_pt = glm::vec4(radius*cos(glm::radians((float)i)), radius*-sin(glm::radians((float)i)), 0.0f, 0.0f);
	//		end_pt = car_body.model_matrix_ * end_pt;

	//		const glm::vec3 r = center + glm::vec3(end_pt.x, end_pt.y, end_pt.z);

	//		int flag;
	//		glm::vec3 col_pt;
	//		float t;

	//		my_square2.checkCollisionLoop(center, r, flag, t, col_pt);

	//		//TODO: clamp col_pt by r
	//		if (flag == 1)
	//		{
	//			sensor_lines.push_back(center);
	//			sensor_lines.push_back(col_pt);
	//		}
	//		else
	//		{
	//			sensor_lines.push_back(center);
	//			sensor_lines.push_back(r);
	//		}
	//	}

	//	sensing_lines.update(sensor_lines);
	//	//sensing_lines.center_ = car_body.center_;
	//}
};