#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

#include "renderEngine.h"
#include "../toolbox/input.h"
#include "../engineTester/main.h"
#include "../toolbox/split.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_close_callback(GLFWwindow* window);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// default settings
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

unsigned int F_WIDTH = 1280;
unsigned int F_HEIGHT = 720;
unsigned int F_BITSRED = 8;
unsigned int F_BITSGREEN = 8;
unsigned int F_BITSBLUE = 8;
unsigned int F_HZ = 60;

unsigned int AA_SAMPLES = 4;

bool useFullscreeen = false;

extern float input_zoom_buffer;

GLFWwindow* window;

void loadDisplaySettings();

int createDisplay()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	loadDisplaySettings();

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

	GLFWmonitor* monitor = NULL;

	glfwWindowHint(GLFW_SAMPLES, AA_SAMPLES);

	unsigned int screenWidth = SCR_WIDTH;
	unsigned int screenHeight = SCR_HEIGHT;

	if (useFullscreeen)
	{
		monitor = glfwGetPrimaryMonitor();

		screenWidth = F_WIDTH;
		screenHeight = F_HEIGHT;
	}

	//int count;
	//const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);

	//for (int i = 0; i < count; i++)
	//{
		//std::fprintf(stdout, "%s\n", modes[i].);
	//}

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(screenWidth, screenHeight, "Sonic 3D Game 2 2", monitor, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::fprintf(stdout, "Renderer:       %s\n", glGetString(GL_RENDERER));
	std::fprintf(stdout, "OpenGL version: %s\n", glGetString(GL_VERSION));
	std::fprintf(stdout, "GLSL version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glfwSwapInterval(1); //1 = vsync. 0 = off. 2 = half monitor refresh rate
	glEnable(GL_MULTISAMPLE);

	//glfwGetWindowAttrib(window, GLFW_SAMPLES);
	//std::fprintf(stdout, "samples:   %d\n", glfwGetWindowAttrib(window, GLFW_SAMPLES));

	//float aniso = 0.0f;
	//glGetFloatv(GL_MAX_TEXTURE_LOD_BIAS, &aniso);
	//std::fprintf(stdout, "max lod bias:   %f\n", aniso);

	//To check what extensions are avalible 
	//int ext_cnt;
	//glGetIntegerv(GL_NUM_EXTENSIONS, &ext_cnt);
	//for (int i = 0; i < ext_cnt; i++)
	{
		//std::fprintf(stdout, "extensions:   %s\n", glGetStringi(GL_EXTENSIONS, i));
	}


	return 0;
}

void updateDisplay()
{
	// glfw: swap buffers
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
}

void closeDisplay()
{
	glfwDestroyWindow(window);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

int displayWantsToClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* getWindow()
{
	return window;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	Master_makeProjectionMatrix();
}

void window_close_callback(GLFWwindow* window)
{
	Global::gameState = STATE_EXITING;
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	input_zoom_buffer = (float)yoffset;
}


void loadDisplaySettings()
{
	std::ifstream file("Settings/DisplaySettings.ini");
	if (!file.is_open())
	{
		std::fprintf(stdout, "Error: Cannot load file 'Settings/DisplaySettings.ini'\n");
		file.close();
	}
	else
	{
		std::string line;

		while (!file.eof())
		{
			getline(file, line);

			char lineBuf[512]; //Buffer to copy line into
			memset(lineBuf, 0, 512);
			memcpy(lineBuf, line.c_str(), line.size());

			int splitLength = 0;
			char** lineSplit = split(lineBuf, ' ', &splitLength);

			if (splitLength == 2)
			{
				if (strcmp(lineSplit[0], "Width") == 0)
				{
					SCR_WIDTH = std::stoi(lineSplit[1], nullptr, 10);
				}
				else if (strcmp(lineSplit[0], "Height") == 0)
				{
					SCR_HEIGHT = std::stoi(lineSplit[1], nullptr, 10);
				}
				else if (strcmp(lineSplit[0], "Fullscreen") == 0)
				{
					useFullscreeen = (strcmp(lineSplit[1], "on") == 0);
				}
				else if (strcmp(lineSplit[0], "F_Width") == 0)
				{
					F_WIDTH = std::stoi(lineSplit[1], nullptr, 10);
				}
				else if (strcmp(lineSplit[0], "F_Height") == 0)
				{
					F_HEIGHT = std::stoi(lineSplit[1], nullptr, 10);
				}
				else if (strcmp(lineSplit[0], "F_Bits_Red") == 0)
				{
					F_BITSRED = std::stoi(lineSplit[1], nullptr, 10);
				}
				else if (strcmp(lineSplit[0], "F_Bits_Green") == 0)
				{
					F_BITSGREEN = std::stoi(lineSplit[1], nullptr, 10);
				}
				else if (strcmp(lineSplit[0], "F_Bits_Blue") == 0)
				{
					F_BITSBLUE = std::stoi(lineSplit[1], nullptr, 10);
				}
				else if (strcmp(lineSplit[0], "F_RefreshRate") == 0)
				{
					F_HZ = std::stoi(lineSplit[1], nullptr, 10);
				}
				else if (strcmp(lineSplit[0], "Anti-Aliasing_Samples") == 0)
				{
					AA_SAMPLES = std::stoi(lineSplit[1], nullptr, 10);
				}
			}
			free(lineSplit);
		}
		file.close();
	}
}