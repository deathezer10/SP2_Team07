#include "Application.h"
#include "Camera3.h"
#include "Scene.h"
#include "PlayerDataManager.h"

#include <GLFW\glfw3.h>



Camera3::Camera3(Scene* scene) : _scene(scene), collider(&playerView, bboxWidth, bboxHeight, bboxDepth) {
}

Camera3::~Camera3() {
}

void cbMouseEvent(GLFWwindow* window, int button, int action, int mods) {

	// Toggle cursor on middle click
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide cursor
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Show Cursor

			// Reset the cursor to middle
			int w, h;
			glfwGetWindowSize(window, &w, &h);
			glfwSetCursorPos(window, (double)w / 2, (double)h / 2);
		}
	}

}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up) {
	this->position = defaultPosition = pos;
	this->view = defaultView = (target - position).Normalized();
	this->target = defaultTarget = position + view;

	this->up = defaultUp = up;

	this->right = defaultRight = view.Cross(up);
	this->right.Normalize();

	yaw = -90;
	pitch = 0;
	roll = 0;

	mouseMovedX = 0;
	mouseMovedY = 0;

	glfwSetMouseButtonCallback(glfwGetCurrentContext(), cbMouseEvent);

	velocityAccelerationRate = &PlayerDataManager::getInstance()->getPlayerStats()->base_speed;

	Reset();
	ResetCursorVariables();
}


void Camera3::updateCursor(double dt) {

	// Prevent the cursor from moving when window is resized
	if (Application::IsWindowValid() == false) {
		Application::ValidateWindow();
		return;
	}

	double currentX = 0;
	double currentY = 0;

	glfwGetCursorPos(glfwGetCurrentContext(), &currentX, &currentY);

	if (currentX != lastX) {
		mouseMovedX = (currentX > lastX) ? 1 : -1; // 1 for left, -1 for right
		mouseMovedDistanceX = std::abs((float)(currentX - lastX)); // distance the cursor moved and return a positive value of it
	}
	else {
		mouseMovedX = 0;
	}

	if (currentY != lastY) {
		mouseMovedY = (currentY < lastY) ? 1 : -1; // 1 for up, -1 for down
		mouseMovedDistanceY = std::abs((float)(currentY - lastY)); // distance the cursor moved and return a positive value of it
	}
	else {
		mouseMovedY = 0;
	}

	lastX = currentX;
	lastY = currentY;

}

void Camera3::Update(double dt) {

	_dt = (float)dt;

	// All Controls are disabled!
	if (canMove == false) {
		setVelocity(0);
		return;
	}

	GLFWwindow* window = glfwGetCurrentContext();

	// Stop rotating the Camera if Cursor is shown or Window is switching modes
	if (isMouseEnabled && glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED && !glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE)) {
		updateCursor(dt);
	}
	else {
		ResetCursorVariables();
	}

	float CAMERA_SPEED = _dt;
	float CAMERA_ROLLBACK_SPEED = 45 * _dt;
	float CAMERA_STRAFE_SPEED = 10 * _dt;
	float CAMERA_LEFT_RIGHT_SPEED = 60.0f * _dt;
	float rotationSpeed = _dt;

	// Bring roll back to zero
	if ((!glfwGetKey(window, GLFW_KEY_A) && !glfwGetKey(window, GLFW_KEY_D) || ((mouseMovedX == 0) && mouseYawEnabled))) {
		if (roll > 1) {
			roll -= rollFalloffSpeed * _dt;
		}
		else if (roll < -1) {
			roll += rollFalloffSpeed * _dt;
		}
		else {
			roll = 0;
		}
	}

	// Camera Forward / Backward / Left / Right
	if (glfwGetKey(window, GLFW_KEY_W)) { // Forward
		wasMovingForward = true;
		currentVelocity += *velocityAccelerationRate * _dt;
	}
	else if (glfwGetKey(window, GLFW_KEY_S)) { // Backward
		wasMovingForward = false;
		currentVelocity -= velocityBrakingRate * _dt;
	}
	else {
		currentVelocity -= velocityDecelerationRate * _dt;
	}

	if (canYaw) {
		if ((mouseMovedX < 0 && mouseYawEnabled)) { // Left

			float angle = rotationSpeed * mouseMovedDistanceX  * 1.5f;
			yaw -= angle;
			roll -= mouseMovedDistanceX * 0.025f;

			// Roll back to center faster
			if (roll > 0) {
				roll -= CAMERA_ROLLBACK_SPEED;
			}


			Mtx44 rotation;
			rotation.SetToRotation(angle, 0, 1, 0);
			view = (target - position).Normalized();
			view = rotation * view;
			target = position + view;
			up = rotation * up;
		}
		else if (mouseMovedX > 0 && mouseYawEnabled) { // Right

			float angle = rotationSpeed * mouseMovedDistanceX * 1.5f;
			yaw += angle;
			roll += mouseMovedDistanceX * 0.025f;

			// Roll back to center faster
			if (roll < 0) {
				roll += CAMERA_ROLLBACK_SPEED;
			}


			Mtx44 rotation;
			rotation.SetToRotation(-angle, 0, 1, 0);
			view = (target - position).Normalized();
			view = rotation * view;
			target = position + view;
			up = rotation * up;
		}

		if (glfwGetKey(window, GLFW_KEY_A) && !glfwGetKey(window, GLFW_KEY_D)) { // Strafe Left

			if (!glfwGetKey(window, GLFW_KEY_W)){
				if (currentVelocity > 31)
				{
					currentVelocity -= *velocityAccelerationRate * _dt;
				}
				else if (currentVelocity < 29)
				{
					currentVelocity += *velocityAccelerationRate * _dt;
				}

				if (currentVelocity > 29 && currentVelocity < 31)
				{
					currentVelocity = currentVelocity;
				}
			}
			else {
				CAMERA_SPEED *= currentVelocity;
				position += view * CAMERA_SPEED;
			}

			position -= right * (fabs(currentVelocity) / 100);
			target = position + view;

			if (FakeYaw <= 10) {
				FakeYaw += rotationSpeed * 60;
			}
			if (FakeRow <= 30) {
				FakeRow += rotationSpeed * 60;
			}
		}
		else {
			if (FakeYaw > 0)
				FakeYaw -= rotationSpeed * 30;
			if (FakeRow > 0)
				FakeRow -= rotationSpeed * 60;
		}
		if (glfwGetKey(window, GLFW_KEY_D) && !glfwGetKey(window, GLFW_KEY_A)) { // Strafe Right

			if (!glfwGetKey(window, GLFW_KEY_W)){
				if (currentVelocity > 31)
				{
					currentVelocity -= *velocityAccelerationRate * _dt;
				}
				else if (currentVelocity < 29)
				{
					currentVelocity += *velocityAccelerationRate * _dt;
				}

				if (currentVelocity > 29 && currentVelocity < 31)
				{
					currentVelocity = currentVelocity;
				}
			}
			else {
				CAMERA_SPEED *= currentVelocity;
				position += view * CAMERA_SPEED;
			}

			position += right * (fabs(currentVelocity) / 100);
			target = position + view;

			if (FakeYaw >= -10) {
				FakeYaw -= rotationSpeed * 60;
			}
			if (FakeRow >= -30) {
				FakeRow -= rotationSpeed * 60;
			}
		}
		else {
			if (FakeYaw < 0)
				FakeYaw += rotationSpeed * 30;
			if (FakeRow < 0)
				FakeRow += rotationSpeed * 60;
		}


	}

	// Limit maximum Roll angle
	roll = Math::Clamp(roll, -rollAngleLimit, rollAngleLimit);

	// Re-calculate view and right vectors
	view = (target - position).Normalized();
	right = view.Cross(up).Normalized();

	if (canPitch) {
		if (mouseMovedY > 0) { // Up
			float angle = rotationSpeed * mouseMovedDistanceY;
			pitch += angle;

			Mtx44 rotation;
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			rotation.SetToRotation(angle, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
		}
		else if (mouseMovedY < 0) { // Down
			float angle = -rotationSpeed * mouseMovedDistanceY;
			pitch += angle;

			Mtx44 rotation;
			right.y = 0;
			right.Normalize();
			up = right.Cross(view).Normalized();
			rotation.SetToRotation(angle, right.x, right.y, right.z);
			view = rotation * view;
			target = position + view;
		}
		// Camera Move Up / Down
		if (glfwGetKey(window, GLFW_KEY_Q) && !glfwGetKey(window, GLFW_KEY_E)) { // Thrust Down

			if (!glfwGetKey(window, GLFW_KEY_W))
				currentVelocity += *velocityAccelerationRate * _dt;

			position -= up * (currentVelocity / 100);
			target = position + view;
			if (FakePitch <= 25) {
				FakePitch += rotationSpeed * 60;
			}
		}
		else {
			if (FakePitch > 0)
				FakePitch -= rotationSpeed * 60;
		}
		if (glfwGetKey(window, GLFW_KEY_E) && !glfwGetKey(window, GLFW_KEY_Q)) { // Thrust Up

			if (!glfwGetKey(window, GLFW_KEY_W))
				currentVelocity += *velocityAccelerationRate * _dt;

			position += up * (currentVelocity / 100);
			target = position + view;
			if (FakePitch >= -15) {
				FakePitch -= rotationSpeed * 60;
			}
		}

		else {
			if (FakePitch < 0)
				FakePitch += rotationSpeed * 60;
		}
	}

	if (std::abs(position.x) >= skyboxBound || std::abs(position.y) >= skyboxBound || std::abs(position.z) >= skyboxBound) {
		currentVelocity = -10;
	}

	// Move the Camera according to the velocity

	if (canYaw == false || (!glfwGetKey(window, GLFW_KEY_A) && !glfwGetKey(window, GLFW_KEY_D))){
		CAMERA_SPEED *= currentVelocity;
		position += view * CAMERA_SPEED;
		target = position + view;
	}


	// Clamping max current velocity
	if (glfwGetKey(window, GLFW_KEY_S))
		currentVelocity = Math::Clamp(currentVelocity, velocityMin, velocityMax);
	else {
		currentVelocity = Math::Clamp(currentVelocity, (wasMovingForward || currentVelocity > 0) ? 1.0f : -1.0f, velocityMax);
	}


	// Model Offset when moving
	if (glfwGetKey(window, GLFW_KEY_W)) {
		if (currentViewOffsetDistance < viewOffsetMaxLength) {
			currentViewOffsetDistance += (float)dt * viewOffsetIncreaseSpeed;
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_S)) {
		if (currentViewOffsetDistance > viewOffsetMinLength) {
			currentViewOffsetDistance -= (float)dt * viewOffsetDecreaseSpeed;
		}
	}
	else if (currentViewOffsetDistance > 0.05f) { // Move from front to back
		currentViewOffsetDistance -= (float)dt * viewOffsetDecreaseSpeed;
	}
	else if (currentViewOffsetDistance < -0.05f) { // Move from back to front
		currentViewOffsetDistance += (float)dt * viewOffsetIncreaseSpeed;
	}
	else {
		currentViewOffsetDistance = 0; // Since float will never become fully zero
	}

	viewOffset = view * currentViewOffsetDistance;
	playerView = target + viewOffset;
}

void Camera3::ResetCursorVariables() {
	mouseMovedX = 0;
	mouseMovedY = 0;
	mouseMovedDistanceX = 0;
	mouseMovedDistanceY = 0;
	glfwGetCursorPos(glfwGetCurrentContext(), &lastX, &lastY);
}

void Camera3::Reset() {
	view = defaultView;
	target = defaultTarget;
	position = defaultPosition;
	up = defaultUp;
	right = defaultRight;

	yaw = -90.0f;
	pitch = 0.0f;
	roll = 0.0f;
}

