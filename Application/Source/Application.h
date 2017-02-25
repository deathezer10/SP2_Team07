#ifndef APPLICATION_H
#define APPLICATION_H

#include "Scene.h"
#include "timer.h"

// Must be included before GLFW/GLEW or it willl cause "macro redefinition"
class Application {

public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

	static const unsigned char FPS = 60; // FPS of this game
	static const unsigned int frameTime = 1000 / FPS; // time for each frame

	// Returns the current window Width
	static int windowWidth() { return _windowWidth; };

	// Returns the current window Height
	static int windowHeight() { return _windowHeight; };

	// Is the application currently in Fullscreen?
	static bool IsFullScreenMode() { return _isFullScreen; };

	// Switch between Fullscreen and Window mode
	static void ToggleFullscreen();

	// Was window resized recently?
	static bool IsWindowValid() { return _isValid; };

	// Flag window as clean
	static void ValidateWindow() { _isValid = true; };

private:
	static int _windowWidth;
	static int _windowHeight;

	static bool _isFullScreen;
	static bool _isValid;

};

#endif