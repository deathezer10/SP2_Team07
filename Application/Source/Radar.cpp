#include "Application.h"
#include "Radar.h"
#include "Scene.h"
#include "NPC.h"

#include <algorithm>


Radar::Radar(Scene* scene) : _RadarSizeSquared(((_RadarSize * _RadarSize))  * _RadarRange) {
	_scene = scene;
}

void Radar::addUnit(NPC* unit) {
	unitContainer.push_back(unit);
}

void Radar::removeUnit(NPC* unit) {
	unitContainer.erase(std::remove(unitContainer.begin(), unitContainer.end(), unit), unitContainer.end());
}

void Radar::RenderRadar(float x, float y) {

	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::_windowWidth / 10, 0, Application::_windowHeight / 10, -10, 10); //size of screen UI
	_scene->projectionStack.PushMatrix();
	_scene->projectionStack.LoadMatrix(ortho);
	_scene->viewStack.PushMatrix();
	_scene->viewStack.LoadIdentity(); //No need camera for ortho mode
	_scene->modelStack.PushMatrix();
	_scene->modelStack.LoadIdentity();
	_scene->modelStack.Translate(x, y, 1);
	_scene->modelStack.Rotate(90, 1, 0, 0);
	// _scene->modelStack.Rotate(_scene->camera.getYaw() + 90, 0, 1, 0);
	_scene->modelStack.Scale(_RadarSize, 1, _RadarSize);
	_scene->RenderMesh(_scene->meshList[Scene::GEO_RADAR_BACKGROUND], false);

	_scene->modelStack.PushMatrix();
	_scene->modelStack.Translate(0, 2, 0);
	_scene->modelStack.Scale(_RadarSize * _RadarPlayerSize, 1, _RadarSize * _RadarPlayerSize);
	_scene->RenderMesh(_scene->meshList[Scene::GEO_RADAR_PLAYER], false);
	_scene->modelStack.PopMatrix();

	//// Go through the number of enemies and translate them relative to the Radar's position
	for (auto it = unitContainer.begin(); it != unitContainer.end(); ++it) {
		Vector3 direction = _scene->camera.position - (*it)->position;

		// Skip if enemy is too far from player
		if (((direction.HorizontalLengthSquared() / 100) / _RadarRange) >= _RadarSizeSquared)
			continue;

		_scene->modelStack.PushMatrix();
		_scene->modelStack.Rotate(_scene->camera.getYaw() + 90, 0, 1, 0); // Rotate first
		_scene->modelStack.Translate((direction.x / (10 * _RadarSize)) / _RadarRange, 3, (direction.z / (10 * _RadarSize)) / _RadarRange);
		_scene->modelStack.Scale(_RadarSize * _RadarEnemySize, 1, _RadarSize * _RadarEnemySize);
		_scene->RenderMesh(_scene->meshList[Scene::GEO_RADAR_ENEMY], false);
		_scene->modelStack.PopMatrix();
	}

	_scene->projectionStack.PopMatrix();
	_scene->viewStack.PopMatrix();
	_scene->modelStack.PopMatrix();
}