#ifndef APPLICATION_H
#define APPLICATION_H

#include "Scene.h"
#include "timer.h"

// Must be included before GLFW/GLEW or it willl cause "macro redefinition"
class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);

	static const int _windowWidth = 800;
    static const int _windowHeight = 600;

    static const unsigned char FPS = 60; // FPS of this game
    static const unsigned int frameTime = 1000 / FPS; // time for each frame
    
private:


};

#endif