#include "Application.h"
#include "Scene.h"
#include "Door.h"



static bool canPress = false;



Door::Door(Scene* scene, Vector3 pos) : Object(scene, pos) {
	type = Scene::GEO_DOOR; scale = 0.5f;
};



void Door::checkInteract() {

	// KeyUp detector
	if (!Application::IsKeyPressed('F'))
		canPress = true;

	// Move the Door
	if (!isDoorOpen) {
		position.y -= (float)_scene->_dt;
	}
	else {
		position.y += (float)_scene->_dt;
	}
	position.y = Math::Clamp<float>(position.y, 0, _doorYLimit); // Prevent the door from flying away


	if ((position - _scene->camera.target).Length() < _interactDistance) {

		// Render Text and move the Door
		if (!isDoorOpen) {
			_scene->textManager.queueRenderText(UIManager::Text{ "[F] Open Door", Color(1, 1, 1), UIManager::ANCHOR_BOT_CENTER });
		}
		else {
			_scene->textManager.queueRenderText(UIManager::Text{ "[F] Close Door", Color(1, 1, 1), UIManager::ANCHOR_BOT_CENTER });
		}

		// Open the Door
		if (Application::IsKeyPressed('F') && canPress) {
			isDoorOpen = !isDoorOpen;
			canPress = false;
		}

	}
}

void Door::interact() {}