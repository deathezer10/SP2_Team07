#include "Application.h"

#include "Camera3.h"
#include <GLFW\glfw3.h>

Camera3::Camera3() : collider(&position, bboxWidth, bboxHeight, bboxDepth) {
}

Camera3::~Camera3() {
}

void cbMouseEvent(GLFWwindow* window, int button, int action, int mods) {

	// Toggle cursor on right click
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
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

	Reset();
	ResetCursorVariables();
}


void Camera3::updateCursor(double dt) {

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

	// Cursor is shown, stop rotating the camera
	if (isMouseEnabled && glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED && !Application::IsKeyPressed(MK_RBUTTON)) {
		updateCursor(dt);
	}
	else {
		ResetCursorVariables();
	}

	float CAMERA_SPEED = currentVelocity * (float)dt;
	float CAMERA_LEFT_RIGHT_SPEED = 40.0f * (float)dt;
	float rotationSpeed = 1.0f * (float)dt;

	// Bring roll back to zero
	if ((!Application::IsKeyPressed('A') && !Application::IsKeyPressed('D') || ((mouseMovedX == 0) && mouseYawEnabled))) {
		if (roll > 1) {
			roll -= rollFalloffSpeed * (float)dt;
		}
		else if (roll < -1) {
			roll += rollFalloffSpeed * (float)dt;
		}
		else {
			roll = 0;
		}
	}
	
	position.x += view.x * CAMERA_SPEED;
	position.y += view.y * CAMERA_SPEED;
	position.z += view.z * CAMERA_SPEED;
	target = position + view;

	// Camera Forward / Backward / Left / Right
	if (Application::IsKeyPressed('W')) { // Forward
		currentVelocity += velocityAccelerationRate * (float)dt;
	}
	else if (Application::IsKeyPressed('S')) { // Backward
		currentVelocity -= velocityDecelerationRate * (float)dt;
		CAMERA_SPEED /= 3;
		position.x -= view.x * CAMERA_SPEED;
		position.y -= view.y * CAMERA_SPEED;
		position.z -= view.z * CAMERA_SPEED;
		target = position + view;
	}
	else {
		currentVelocity -= velocityDecelerationRate * (float)dt;
	}

	if (Application::IsKeyPressed('A') || ((mouseMovedX < 0 && mouseYawEnabled) && !Application::IsKeyPressed('D'))) { // Left
		yaw -= CAMERA_LEFT_RIGHT_SPEED;
		roll -= CAMERA_LEFT_RIGHT_SPEED;

		Mtx44 rotation;
		rotation.SetToRotation(CAMERA_LEFT_RIGHT_SPEED, 0, 1, 0);
		view = (target - position).Normalized();
		view = rotation * view;
		target = position + view;
		up = rotation * up;
	}
	else if ((!Application::IsKeyPressed('A') && Application::IsKeyPressed('D')) || (!Application::IsKeyPressed('A') && (mouseMovedX > 0 && mouseYawEnabled))) { // Right
		yaw += CAMERA_LEFT_RIGHT_SPEED;
		roll += CAMERA_LEFT_RIGHT_SPEED;

		Mtx44 rotation;
		rotation.SetToRotation(-CAMERA_LEFT_RIGHT_SPEED, 0, 1, 0);
		view = (target - position).Normalized();
		view = rotation * view;
		target = position + view;
		up = rotation * up;
	}

	// Camera Move Up / Down
	if (Application::IsKeyPressed('Z')) { // Up
		position = position + up  * CAMERA_SPEED;
		target = position + view;
	}
	else if (Application::IsKeyPressed('X')) { // Down
		position = position - up * CAMERA_SPEED;
		target = position + view;
	}

	// Positional bounds check
	position.x = Math::Clamp(position.x, -skyboxBound, skyboxBound);
	position.y = Math::Clamp(position.y, -skyboxBound, skyboxBound);
	position.z = Math::Clamp(position.z, -skyboxBound, skyboxBound);

	// Limit maximum Roll angle
	roll = Math::Clamp(roll, -rollAngleLimit, rollAngleLimit);

	// Re-calculate view and right vectors
	view = (target - position).Normalized();
	right = view.Cross(up).Normalized();

	// It works :o
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

	// Clamping max current velocity
	if (Application::IsKeyPressed('S'))
		currentVelocity = Math::Clamp(currentVelocity, velocityMin, velocityMax);
	else {
		currentVelocity = Math::Clamp(currentVelocity, 1.0f, velocityMax);
	}
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
	position = defaultPosition;
	up = defaultUp;
	right = defaultRight;

	yaw = -90.0f;
	pitch = 0.0f;
	roll = 0.0f;
}