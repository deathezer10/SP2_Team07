#include "Radar.h"
#include "Scene.h"

#include <algorithm>


Radar::Radar(Scene* scene){
	_scene = scene;
}



void Radar::addUnit(NPC* unit){
	unitContainer.push_back(unit);
}

void Radar::removeUnit(NPC* unit){
	unitContainer.erase(std::remove(unitContainer.begin(), unitContainer.end(), unit), unitContainer.end());
}

void Radar::RenderRadar(float x, float y){

	// Go through the number of enemies and translate them relative to the Radar's position
	for (auto it = unitContainer.begin(); it != unitContainer.end(); ++it){
				
		Vector3 direction = _scene->camera.position - (*it)->position;

		// TODO: Convert enemy position to Screen Coordinates
		_scene->textManager.RenderMeshOnScreen(_scene->meshList[Scene::GEO_RADAR_ENEMY], direction.x / 10, direction.z / 10, Vector3(90, 0, 0), Vector3(_RadarEnemySize, 1, _RadarEnemySize));

	}

	_scene->textManager.RenderMeshOnScreen(_scene->meshList[Scene::GEO_RADAR_PLAYER], x, y, Vector3(90, 0, 0), Vector3(_RadarPlayerSize, 1, _RadarPlayerSize));
	_scene->textManager.RenderMeshOnScreen(_scene->meshList[Scene::GEO_RADAR_BACKGROUND], x, y, Vector3(90, 0, 0), Vector3(_RadarSize, 1, _RadarSize));

}