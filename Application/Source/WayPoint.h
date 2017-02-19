#ifndef WAY_POINT_H
#define WAY_POINT_H

#include "Vector3.h"

class Scene;

// WayPoint Navigation Arrow that rotate towards a specific point on map with proper Pitching and Yawing
class WayPoint {

public:
	WayPoint(Scene* scene);
	~WayPoint() {};

	// Face the Arrow towards the target
	void RotateTowards(Vector3& target);

	// Render the Waypoint Arrow to the Screen
	void RenderArrow();

private:
	Vector3 arrowRotation;
	Vector3 arrowSize;

	Scene* _scene;

	const float defaultArrowSize = 5;

};
#endif