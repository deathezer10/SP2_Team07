#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera2.h"
#include "Mtx44.h"

// FPS Camera
class Camera3 : public Camera2 {

	friend class CharacterController;

public:

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	const float skyboxSize = 1001.0f; // Distance of each skybox textures from the origin
	const float skyboxBound = 50.f; // Clamps position of the Camera to this value

	Vector3 getTarget() { return target; };
	Vector3 getUp() { return up; };
	Vector3 getPosition() { return position; };
	Vector3 getRight() { return right; };
	float getYaw() { return yaw; };
	float getPitch() { return pitch; };
	float getRoll() { return roll; };
	
	double getMouseMovedX() { return mouseMovedX; };
	double getMouseMovedY() { return mouseMovedY; };

	void disableMouse() { isMouseEnabled = false; };
	void enableMouse() { isMouseEnabled = true; };

	void ResetCursorVariables();

	// Player Collision detection
	const float bboxWidth = 5;
	const float bboxHeight = 5;

private:
	Vector3 view;
	Vector3 right;
	Vector3 defaultRight;
	
	// Current angle of camera rotation
	float yaw = -90;
	float pitch = 0;
	float roll = 0; // Does not actually roll the camera

	// Mouse direction moved, possible values: -1, 0, 1
	double mouseMovedX;
	double mouseMovedY;
	
	// Distance the Cursor moved from the current and last frame. Used to determine mouse sensitivity
	float mouseMovedDistanceX;
	float mouseMovedDistanceY; 		

	// Updates the direction in which the cursor has moved
	void updateCursor(double dt); 

	// True if mouse is currently hidden
	bool isMouseEnabled = true;
	
	// Both Minimum and Maximum Roll angle in both directions
	const float rollAngleLimit = 50.0f;

	// Rotate roll back to zero, unit is degrees per second
	const float rollFalloffSpeed = 75.0f; 

	// Cursor's previous position
	double lastX = 0;
	double lastY = 0;


};
#endif