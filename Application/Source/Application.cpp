#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneManager.h"
#include "SceneGameOver.h"
#include "SceneMainMenu.h"
#include "SceneShop.h"


GLFWwindow* m_window;

bool Application::_isFullScreen = false;
bool Application::_isValid = true;

int Application::_windowWidth = 1024;
int Application::_windowHeight = 768;


//Define an error callback
static void error_callback(int error, const char* description) {
	fputs(description, stderr);
	_fgetchar();
}

//Define window resizing callback
void resize_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h); //update opengl the new window size
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_F5 && action == GLFW_RELEASE) {
		Application::ToggleFullscreen();
	}
}

void Application::ToggleFullscreen() {

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if (_isFullScreen == false) { // Switch to Borderless Fullscreen
		glfwSetWindowMonitor(m_window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
	}
	else { // Switch to Windowed
		glfwSetWindowMonitor(m_window, NULL, mode->width / 5, mode->height / 5, _windowWidth, _windowHeight, GL_DONT_CARE);
	}

	_isValid = false; // Window was resized

	_isFullScreen = !_isFullScreen;

}

bool Application::IsKeyPressed(unsigned short key) {
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application() {
}

Application::~Application() {
}

void Application::Init() {

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	//Current monitor's video mode
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Set the Video mode to same as the current one
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(_windowWidth, _windowHeight, "Space Fighter 27", NULL, NULL);

	//Center the window
	glfwSetWindowPos(m_window, mode->width / 5, mode->height / 5);

	//If the window couldn't be created
	if (!m_window) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetWindowUserPointer(m_window, nullptr); // To retrieve the window handle in other classes
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable cursor on start

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Resize Window callback
	glfwSetWindowSizeCallback(m_window, resize_callback);

	//Sets the key callback
	glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile

	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void Application::Run() {
	SceneManager* scene_manager = SceneManager::getInstance();

	scene_manager->changeScene(new SceneMainMenu());
	scene_manager->Update();
}

void Application::Exit() {
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);

	//Finalize and clean up GLFW
	glfwTerminate();
}
