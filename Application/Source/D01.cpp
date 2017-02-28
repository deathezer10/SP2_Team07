#include "Scene.h"
#include "D01.h"
#include "CargoShip.h"
#include "PlayerDataManager.h"
#include "SceneDogfight.h"

#include <map>

using std::multimap;


unsigned D01::D01Count = 0;
Vector3* D01::NearestD01Pos = nullptr;


D01::D01(Scene* scene, Vector3 pos) : NPC(scene, pos) {
	setHealth(200);
	type = Scene::GEO_XF2;
	scale = 2.0f;
	isLightingEnabled = false;
	++D01Count;

	setCollision(true);
	collider.setBoundingBoxSize(Vector3(scale, scale, scale));
}

D01::~D01() {
	--D01Count;

	SceneDogfight::killcount += 1;

	if (NearestD01Pos == &position) {
		NearestD01Pos = &Vector3(0, 0, 0);
	}
}