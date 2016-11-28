/*

http://www.opengl-tutorial.org/beginners-tutorials/
https://github.com/opengl-tutorials/ogl

#include <stdio.h> error fix : http://stackoverflow.com/questions/38290169/cannot-find-corecrt-h-universalcrt-includepath-is-wrong
Add directories

*/

#include "GLFWExample.h"
#include <memory>
#include "CarDrivingGame.h"
//#include "ReinforcementLearning.h"

GLFWExample glfw_example;
CarDrivingGame game_;
//ReinforcementLearning rl_;

bool is_training = false; // true: rendering, false: background mode
bool human_play = true;
bool use_conv = false;

void render_main();
//void initializeAI();
//void play_AI();

//TODO: reset history when game was reset

int main(void)
{
	glfw_example.init();
	
	game_.init();

	//initializeAI();

	do 
	{
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
			if (glfw_example.getKeyPressed(GLFW_KEY_UP) == true) game_.processInput(2);
			if (glfw_example.getKeyPressed(GLFW_KEY_DOWN) == true) game_.processInput(3);

			game_.update(true);
		}
		else
		{
			// AI play and training
			//play_AI();
		}		

		if (is_training == false) render_main();
		else glfwPollEvents();	// for mode change key input

	} // Check if the ESC key was pressed or the window was closed
	while (!glfw_example.getKeyPressed(GLFW_KEY_ESCAPE) &&	glfw_example.getWindowShouldClose());

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

//void initializeAI()
//{
//	if (use_conv == true)
//	{
//		exit(1);
//
//		game_.compat_state_ = false;
//		game_.state_buffer_.initialize(game_.getNumStateVariables(), true);
//
//		rl_.num_input_histories_ = 4;
//		rl_.num_exp_replay_ = 20;
//		rl_.num_state_variables_ = game_.getNumStateVariables();
//		rl_.num_game_actions_ = 4; //TODO: from game
//
//		rl_.initializeConv2D();
//
//		for (int h = 0; h < rl_.history_.array_.num_elements_; h++)
//		{
//			rl_.recordHistory(game_.getStateBuffer(), 0, 1);
//		}
//	}
//	else
//	{
//		game_.compat_state_ = true;
//		game_.state_buffer_.initialize(game_.getNumStateVariables(), true);
//
//		rl_.num_input_histories_ = 4;
//		rl_.num_exp_replay_ = 100;
//		rl_.num_state_variables_ = game_.getNumStateVariables();
//		rl_.num_game_actions_ = 4;//TODO: from game
//
//		rl_.initialize();
//
//		for (int h = 0; h < rl_.history_.array_.num_elements_; h++)
//		{
//			rl_.recordHistory(game_.getStateBuffer(), 0, 0);
//		}
//	}
//}

//void play_AI()
//{
//	rl_.forward();
//
//	int selected_dir;
//
//	// user supervised mode
//	if (glfw_example.getKeyPressed(GLFW_KEY_LEFT) == true) selected_dir = 0;
//	else if (glfw_example.getKeyPressed(GLFW_KEY_RIGHT) == true) selected_dir = 1;
//	else if (glfw_example.getKeyPressed(GLFW_KEY_UP) == true) selected_dir = 2;
//	else if (glfw_example.getKeyPressed(GLFW_KEY_DOWN) == true) selected_dir = 3;
//	else // AI mode
//	{
//		selected_dir = is_training == true ? rl_.nn_.getOutputIXEpsilonGreedy(0.2) : rl_.nn_.getOutputIXEpsilonGreedy(0.0);
//		//const int selected_dir = rl_.nn_.getOutputIXEpsilonGreedy(0.3);
//		
//	}
//	game_.processInput(selected_dir);//TODO: multiple input
//	
//	
//	float reward = game_.update(!is_training);
//
//	// update state	
//	{
//		rl_.history_.getLast().choice_ = selected_dir;
//		rl_.history_.getLast().reward_ = reward;
//		rl_.recordHistory(game_.getStateBuffer(), reward, selected_dir);
//
//		rl_.trainReward(); // note that history is updated
//	}
//
//	 // reset record, game was reset already in update
//	//if (reward < 0.0f)
//	//{
//	//	for (int h = 0; h < rl_.history_.array_.num_elements_; h++)
//	//	{
//	//		rl_.recordHistory(game_.getStateBuffer(), 0, 0);
//	//	}
//
//	//	//reward = 0.0f;
//	//}
//}