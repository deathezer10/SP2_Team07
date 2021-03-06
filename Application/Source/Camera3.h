#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera2.h"
#include "Mtx44.h"
#include "Collider.h"



class Scene;

// Space Fighter Camera
class Camera3 : public Camera2 {

public:
	Camera3(Scene* scene);
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	const float skyboxSize = 1001.0f; // Distance of each skybox textures from the origin
	const float skyboxBound = 2500.f; // Clamps position of the Camera to this value

	Vector3 getForward() { return up.Cross(right); };
	Vector3& getRight() { return right; };
	Vector3& getView() { return view; };
	float& getYaw() { return yaw; };
	float& getPitch() { return pitch; };
	float& getRoll() { return roll; };
	float& getCurrentVelocity() { return currentVelocity; };

	Collider& getCollider() { return collider; };

	double& getMouseMovedX() { return mouseMovedX; };
	double& getMouseMovedY() { return mouseMovedY; };

	// Enable mouse controls
	void enableMouse() { isMouseEnabled = true; };

	// Disable mouse controls
	void disableMouse() { isMouseEnabled = false; };

	void allowMovement(bool toggle) { canMove = toggle; };
	void allowYaw(bool toggle) { canYaw = toggle; };
	void allowPitch(bool toggle) { canPitch = toggle; };
	void allowRoll(bool toggle) { canRoll = toggle; };

	// Player Box Collider
	const float bboxWidth = 1;
	const float bboxHeight = 1;
	const float bboxDepth = 1;

	void setViewOffsetToMax() { currentViewOffsetDistance = viewOffsetMaxLength; };

	void setVelocity(float speed) { currentVelocity = speed; };
	float getMaxVelocity() { return velocityMax; };

	float FakePitch = 0;
	float FakeYaw = 0;
	float FakeRow = 0;
	Vector3 playerView; // Player model offset, used for indicating that the player has moved

	// Reset the helper variables of this class back to default
	void ResetCursorVariables();

private:
	Scene* _scene;

	Vector3 view;
	Vector3 right;
	Vector3 defaultRight;

	Collider collider;

	float _dt = 0;

	// Offsets the player model when accelerating or decelerating
	Vector3 viewOffset; // helper to offset player model
	float viewOffsetMaxLength = 0.5f; // prevent the model from move forward too much
	float viewOffsetMinLength = -0.25f; // prevent the model from move forward too much
	float currentViewOffsetDistance = 0;
	float viewOffsetIncreaseSpeed = 1.0f; // Speed in which the offset increases
	float viewOffsetDecreaseSpeed = 1.5f; // Speed in which the offset decreases

	// Current angle of camera rotation
	float yaw = -90;
	float pitch = 0;
	float roll = 0; // Does not actually roll the camera

	// Controls Toggle
	bool canYaw = true;
	bool canPitch = true;
	bool canRoll = true;
	bool canMove = true;

	// Current speed of the camera
	float currentVelocity = 1;
	float* velocityAccelerationRate;
	float velocityDecelerationRate = 5.0f;
	float velocityBrakingRate = 75.0f;
	float velocityMax = 200;
	float velocityMin = -5;
	bool wasMovingForward = true; // Was the camera accelerating?

	// Enable Mouse Horizontal Control
	bool mouseYawEnabled = true;

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
	const float rollFalloffSpeed = 50.0f;

	// Cursor's previous position
	double lastX = 0;
	double lastY = 0;

};
#endif