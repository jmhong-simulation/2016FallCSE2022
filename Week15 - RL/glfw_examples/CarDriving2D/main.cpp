/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong at Dongguk University CSE					   //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

/*

http://www.opengl-tutorial.org/beginners-tutorials/
https://github.com/opengl-tutorials/ogl

#include <stdio.h> error fix : http://stackoverflow.com/questions/38290169/cannot-find-corecrt-h-universalcrt-includepath-is-wrong
Add directories

*/

#include "GLFWExample.h"
#include <memory>
#include "CarDrivingGame.h"
#include "ReinforcementLearning.h"

GLFWExample glfw_example;
CarDrivingGame game_;
ReinforcementLearning rl_;

bool is_training = false; // false: rendering, true: background mode
bool human_play = false;
bool use_conv = false;

void initializeAI();
void render_main();
void play_AI();

//TODO: reset history when game was reset

int main(void)
{
	glfw_example.init();
	
	game_.init();

	initializeAI();
	
	if (is_training)
	{
		std::cout << "Back ground training mode." << std::endl;
		std::cout << "Press space to change mode." << std::endl;
	}

	while(true)
	{
		glfwPollEvents();

		// Check if the ESC key was pressed or the window was closed
		if (glfw_example.getKeyPressed(GLFW_KEY_ESCAPE) || glfw_example.getWindowShouldClose())
			break;

		// training mode change key input
		// TODO: define key_reset_flag inside glfw_example
		static bool key_reset_flag = true;
		if (glfw_example.getKeyPressed(GLFW_KEY_SPACE) == true)
		{
			if(key_reset_flag == true)
			{
				//std::cout << "space key pressed" << std::endl;

				is_training = !is_training;

				key_reset_flag = false;

				if (is_training)
				{
					std::cout << "Back ground training mode" << std::endl;
				}
				else
				{
					std::cout << "Interactive rendering mode" << std::endl;
				}
			}
		}
		else
		{
			key_reset_flag = true;
		}

		// animate update

		if(human_play == true)
		{
			if (glfw_example.getKeyPressed(GLFW_KEY_LEFT) == true) game_.processInput(0);
			if (glfw_example.getKeyPressed(GLFW_KEY_RIGHT) == true) game_.processInput(1);
			// do nothing gives game_.processInput(2);
			//if (glfw_example.getKeyPressed(GLFW_KEY_UP) == true) game_.processInput(2);
			//if (glfw_example.getKeyPressed(GLFW_KEY_DOWN) == true) game_.processInput(3);

			game_.processInput(2); // always accelerate in this example

			// dummy reward
			float reward;
			int flag;

			game_.update(true, reward, flag);

			render_main();
		}
		else // AI play and training
		{
			play_AI();

			if (is_training == false) render_main();
			else
			{
				// do not render in batch training mode
				//render_main();
				// for mode change key input
			}
		}
	} 

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void render_main()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	// Use our shader
	glUseProgram(glfw_example.programID);

	GLuint MatrixID = glGetUniformLocation(glfw_example.programID, "MVP");
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);
	//glm::mat4 Projection = glm::ortho(-1.0f,1.0f,-1.0f,1.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.5, 0.5, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0.5, 0.5, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// draw
	game_.my_car.car_body.drawLineLoop(MatrixID, Projection * View);
	game_.my_car.sensing_lines.drawLineLoop(MatrixID, Projection * View);

	//for (auto itr : obj_list) // this doesn't work with unique ptr
	for (int i = 0; i < game_.obj_list.size(); i++)
	{
		game_.obj_list[i]->drawLineLoop(MatrixID, Projection * View);
	}

	glDisableVertexAttribArray(0);

	glfw_example.swapBuffers();
}

void initializeAI()
{
	if (use_conv == true)
	{
		exit(1);

		//game_.compat_state_ = false;
		//game_.state_buffer_.initialize(game_.getNumStateVariables(), true);

		//rl_.num_input_histories_ = 4;
		//rl_.num_exp_replay_ = 20;
		//rl_.num_state_variables_ = game_.getNumStateVariables();
		//rl_.num_game_actions_ = 4; //TODO: from game

		//rl_.initializeConv2D();

		//for (int h = 0; h < rl_.num_input_histories_; h++)
		//{
		//	rl_.recordHistory(game_.getStateBuffer(), 0.5, 2);
		//}
	}
	else
	{
		game_.compat_state_ = true;
		game_.state_buffer_.initialize(game_.getNumStateVariables(), true);

		rl_.num_input_histories_ = 1;
		rl_.num_exp_replay_ = 0;
		rl_.num_state_variables_ = game_.getNumStateVariables();
		rl_.num_game_actions_ = game_.getNumActions();//TODO: obtain from game, left, right, stay

		rl_.initialize();

		for (int h = 0; h < rl_.num_input_histories_; h++)
		{
			rl_.recordHistory(game_.getStateBuffer(), 0.0f, 2, VectorND<float>(game_.getNumActions())); // choice 2 is stay
		}
	}
}

const int getSelectedDir()
{
	int selected_dir;

	// user supervised mode
	if (glfw_example.getKeyPressed(GLFW_KEY_LEFT) == true) selected_dir = 0;
	else if (glfw_example.getKeyPressed(GLFW_KEY_RIGHT) == true) selected_dir = 1;
	//else if (glfw_example.getKeyPressed(GLFW_KEY_UP) == true) selected_dir = 2;
	//else if (glfw_example.getKeyPressed(GLFW_KEY_DOWN) == true) selected_dir = 3;
	else // AI mode
	{
		selected_dir = is_training == true ? rl_.nn_.getOutputIXEpsilonGreedy(0.2f) : rl_.nn_.getOutputIXEpsilonGreedy(0.0f);	
	}

	if (glfw_example.getKeyPressed(GLFW_KEY_Q) == true)
	{
		rl_.nn_.writeTXT("nn.txt");

		std::cout << "writing complete" << std::endl;
	}

	return selected_dir;
}

void play_AI()
{
	rl_.forward();

	VectorND<float> output_vector_temp;
	rl_.nn_.copyOutputVectorTo(false, output_vector_temp);

	VectorND<float> output_target_temp;

	const int selected_dir = getSelectedDir(); // epsilon-greedy

	game_.processInput(selected_dir);//TODO: multiple input
	
	float reward;
	int flag;
	game_.update(!is_training, reward, flag);// flag = 0 : continue, 1 : terminal

	// record state and reward
	rl_.recordHistory(game_.getStateBuffer(), reward, selected_dir, output_vector_temp);

	static float reward_sum = 0.0f;
	reward_sum += reward;
	static float reward_max = 0.0f;

	// start memory replay training at terminal state
	if(flag != 0) // this is terminal state
	{
		std::cout << "Reward sum " << reward_sum << std::endl;
		
		int tr_num = 10;

		if (reward_max < reward_sum)
		{
			reward_max = reward_sum;
			tr_num = 100;

			std::cout << "New record " << reward_max << std::endl;
		}

		reward_sum = 0.0f;

		// train backward (memory replay)

		if(is_training == true)
		for(int tr = 0; tr < tr_num; tr ++)
		for (int m_tr = rl_.memory_.num_elements_ - 2; m_tr >= rl_.num_input_histories_; m_tr--)
		{
			// stochastic training
			int m = rand() % (rl_.memory_.num_elements_ - 1 - rl_.num_input_histories_) + rl_.num_input_histories_;

			// memory index from end
			const int inv_m = m - (rl_.memory_.num_elements_ - 1);

			float Q_next = 0.0f;

			if (m != rl_.memory_.num_elements_ - 2) // if next is not the terminal state
			{
				// Q_next = ...;
			}

			float Q_target;
			// Q_target = ...;

			// forward propagation from previous inputs
			rl_.makeInputVectorFromHistory(inv_m-1, rl_.old_input_vector_);
			rl_.nn_.setInputVector(rl_.old_input_vector_);
			for (int i = 0; i < 100; i ++)
			{
				rl_.nn_.feedForward();
				rl_.nn_.copyOutputVectorTo(false, output_target_temp);
				
				// output_target_temp[...] = ...;

				rl_.nn_.propBackward(output_target_temp);
			}

			rl_.nn_.check();
		}

		// reset memory
		rl_.memory_.reset();		

		for (int h = 0; h < rl_.num_input_histories_; h++)
		{
			rl_.recordHistory(game_.getStateBuffer(), 0.0f, 2, VectorND<float>(game_.getNumActions())); // choice 2 is stay
		}
	}

	//if(is_training == true)
	//for (int ex = 0; ex <= rl_.num_exp_replay_; ex++)
	//{
	//	rl_.trainReward(-ex);  // note that history is updated
	//}	
}