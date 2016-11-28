#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"

class GLFWExample
{
public:
	GLFWwindow* window = nullptr;
	GLuint VertexArrayID;
	GLuint programID;

	int init()
	{
		using namespace glm;

		// Initialise GLFW
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			getchar();
			return -1;
		}

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Open a window and create its OpenGL context
		window = glfwCreateWindow(1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);

		if (window == NULL) {
			fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
			getchar();
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			getchar();
			glfwTerminate();
			return -1;
		}

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		// background color
		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// Create and compile our GLSL program from the shaders
		programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

		return 0;
	}

	void swapBuffers()
	{
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	bool getKeyPressed(const int& key)
	{
		return (glfwGetKey(window, key) == GLFW_PRESS);
	}

	bool getWindowShouldClose()
	{
		return (glfwWindowShouldClose(window) == 0);
	}

	void clear()
	{
		// Cleanup VBO
		//glDeleteBuffers(1, &vertexbuffer);
		glDeleteVertexArrays(1, &VertexArrayID);
		glDeleteProgram(programID);
	}
};