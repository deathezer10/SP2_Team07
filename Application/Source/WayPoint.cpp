#include "Application.h"
#include "WayPoint.h"
#include "Scene.h"



WayPoint::WayPoint(Scene* scene) {

	_scene = scene;

	arrowSize.Set(defaultArrowSize, defaultArrowSize, defaultArrowSize);

}


void WayPoint::RotateTowards(Vector3& target) {

	Camera3 camera = _scene->camera; // Shortcut variable

	Vector3	direction = camera.playerView - target;

	arrowRotation.y = -Math::RadianToDegree(atan2(direction.z, direction.x)) + camera.getYaw();
	arrowRotation.x = -(Math::RadianToDegree(atan2(direction.y, direction.HorizontalLength()))) - camera.getPitch();

}


void WayPoint::RenderArrow() {
	_scene->textManager.RenderMeshOnScreen(_scene->meshList[Scene::GEO_WAYPOINT], (float)Application::windowWidth() / 20, (float)Application::windowHeight() / 12, arrowRotation, arrowSize);
}