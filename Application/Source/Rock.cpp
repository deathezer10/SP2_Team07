#include "Assignment03.h"
#include "Rock.h"



static bool canPress = false;



Rock::Rock(Assignment03* scene, Vector3 pos) : Object(scene, pos) {
	type = Assignment03::GEO_ROCK1;
};


void Rock::checkInteract() {

	if (_isHarvested)
		return;

	// KeyUp detector
	if (!Application::IsKeyPressed('E'))
		canPress = true;

	if (!isInteracted) {
		if ((position - _scene->camera.target).Length() < _interactDistance) {

			if (_scene->charManager.isRockPickedUp == false) { // Hide text if player already has a rock
				_scene->textManager.queueRenderText(UIManager::Text{ "[E] Pick up Rock", Color(1, 1, 1), UIManager::ANCHOR_BOT_CENTER });

				// Rock picked up
				if (Application::IsKeyPressed('E') && canPress) {
					_defaultScale = scale;
					_scene->charManager.isRockPickedUp = true;
					isInteracted = true;
					canPress = false;
				}
			}
		}
	}
	else {
		interact();
	}
}

void Rock::interact() {

	// KeyUp detector
	if (!Application::IsKeyPressed('E'))
		canPress = true;

	float distanceFromBase = (_scene->camera.target - Vector3(0, 0, -15)).Length();

	if (distanceFromBase >= 3)
		_scene->textManager.queueRenderText(UIManager::Text("[E] Place the Rock", Color(1, 1, 1), UIManager::ANCHOR_CENTER_CENTER));
	else
		_scene->textManager.queueRenderText(UIManager::Text("[E] Harvest Rock", Color(0, 1, 0), UIManager::ANCHOR_CENTER_CENTER));

	position = _scene->camera.target;
	rotationY = -_scene->camera.getYaw();
	scale = 0.25f; // scale down when holding the rock

	// Rock placed
	if (Application::IsKeyPressed('E') && canPress) {

		scale = _defaultScale;
		_scene->charManager.isRockPickedUp = false;
		isInteracted = false;
		canPress = false;

		if (distanceFromBase < 3) { // Placed on the table
			
			float randomX = Math::RandFloatMinMax(-1.0f, 1.0f);
			float randomZ = Math::RandFloatMinMax(-0.5f, 0.5f);
			scale = 0.25f;
			_scene->charManager.increaseScore(1);
			position = Vector3(randomX, 1.5f, -15.f + randomZ);
			_isHarvested = true;
			return;
		}
		else {
			position.y = 0; // else place the rock on the floor
		}

	}

}