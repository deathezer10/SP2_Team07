#include "Camera3.h"
#include "Application.h"
#include <GLFW\glfw3.h>

Camera3::Camera3() {
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
	this->target = defaultTarget = target;
	this->view = defaultView = (target - position).Normalized();

	this->right = defaultRight = view.Cross(up);
	this->right.y = 0;
	this->right.Normalize();

	this->up = defaultUp = right.Cross(view).Normalized();

	yaw = 0.0f;
	pitch = 0.0f;

	mouseMovedX = 0;
	mouseMovedY = 0;

	glfwSetMouseButtonCallback(glfwGetCurrentContext(), cbMouseEvent);

}


void Camera3::updateCursor() {

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
		updateCursor();
	}
	else {
		ResetCursorVariables();
	}

	float CAMERA_SPEED = 4.0f * (float)dt;
	float rotationSpeed = 2.5f * (float)dt;

	// "Sprint"
	if (Application::IsKeyPressed(VK_LSHIFT)) {
		CAMERA_SPEED *= 2.0f;
	}

	view = (target - position).Normalized();
	right = view.Cross(up).Normalized();

	// Camera Forward / Backward / Left / Right
	if (Application::IsKeyPressed('W')) { // Forward
		position.x += view.x * CAMERA_SPEED;
		position.z += view.z * CAMERA_SPEED;
	}
	else if (Application::IsKeyPressed('S')) { // Backward
		position.x -= view.x * CAMERA_SPEED;
		position.z -= view.z * CAMERA_SPEED;
	}
	if (Application::IsKeyPressed('A')) { // Left
		position = position - (right * CAMERA_SPEED);
		target = position + view;
	}
	else if (Application::IsKeyPressed('D')) { // Right
		position = position + right * CAMERA_SPEED;
		target = position + view;
	}

	// Camera Move Up / Down
	if (Application::IsKeyPressed('Z')) { // Up
		position = position + up  * CAMERA_SPEED;
		target = position + view;
	}
	if (Application::IsKeyPressed('X')) { // Down
		position = position - up * CAMERA_SPEED;
		target = position + view;
	}


	// Rotate Camera with mouse-axis
	if (mouseMovedX < 0) { // Left
		yaw -= rotationSpeed * mouseMovedDistanceX;
	}

	if (mouseMovedX > 0) { // Right
		yaw += rotationSpeed * mouseMovedDistanceX;
	}

	if (mouseMovedY > 0) { // Up
		pitch += rotationSpeed * mouseMovedDistanceY;
	}

	if (mouseMovedY < 0) { // Down
		pitch -= rotationSpeed * mouseMovedDistanceY;
	}

	// Positional bounds check
	position.x = Math::Clamp(position.x, -skyboxBound, skyboxBound);
	position.z = Math::Clamp(position.z, -skyboxBound, skyboxBound);

	pitch = Math::Clamp(pitch, _MinYawAngle, _MaxYawAngle); // clamp the up/down rotation of the camera to these angles

	target.x = cos(Math::DegreeToRadian(pitch)) * cos(Math::DegreeToRadian(yaw)) + position.x;
	target.y = sin(Math::DegreeToRadian(pitch)) + position.y;
	target.z = cos(Math::DegreeToRadian(pitch)) * sin(Math::DegreeToRadian(yaw)) + position.z;

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

	yaw = 0.0f;
	pitch = 0.0f;

	target.x = cos(Math::DegreeToRadian(pitch)) * cos(Math::DegreeToRadian(yaw)) + position.x;
	target.y = sin(Math::DegreeToRadian(pitch)) + position.y;
	target.z = cos(Math::DegreeToRadian(pitch)) * sin(Math::DegreeToRadian(yaw)) + position.z;
}