#include "Application.h"
#include "WayPoint.h"
#include "Scene.h"



WayPoint::WayPoint(Scene* scene) {

	_scene = scene;

	arrowSize.Set(defaultArrowSize, defaultArrowSize, defaultArrowSize);

}


void WayPoint::RotateTowards(Vector3& target) {

	Camera3 camera = _scene->camera; // Shortcut variable

	Vector3	cameraToRing = target - camera.position;


	if (cameraToRing.z < 0) { // Target is behind the camera
		arrowRotation.y = Math::RadianToDegree(atan2(cameraToRing.z, cameraToRing.x)) + (-camera.getYaw());
		arrowRotation.x = -(Math::RadianToDegree(atan2(cameraToRing.y, cameraToRing.z))) - camera.getPitch();
	}
	else {
		arrowRotation.y = Math::RadianToDegree(atan2(cameraToRing.x, cameraToRing.z)) + camera.getYaw() + 90;
		arrowRotation.x = Math::RadianToDegree(atan2(cameraToRing.y, cameraToRing.z)) - camera.getPitch();
	}

}


void WayPoint::RenderArrow() {
	_scene->textManager.RenderMeshOnScreen(_scene->meshList[Scene::GEO_WAYPOINT], Application::_windowWidth / 20, Application::_windowHeight / 12, arrowRotation, arrowSize);
}