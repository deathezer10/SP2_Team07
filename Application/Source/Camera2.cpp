#include "Camera2.h"
#include "Application.h"
#include "Mtx44.h"

Camera2::Camera2() {
}

Camera2::~Camera2() {
}

void Camera2::Init(const Vector3& pos, const Vector3& target, const Vector3& up) {
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

}

void Camera2::Update(double dt) {

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = right.Cross(view).Normalized();

	const float CAMERA_SPEED = 50.f;


	if (Application::IsKeyPressed(VK_LEFT)) {
		float yaw = (float)(-CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_RIGHT)) {
		float yaw = (float)(CAMERA_SPEED * dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		position = rotation * position;
		up = rotation * up;
	}
	if (Application::IsKeyPressed(VK_UP)) {
		float pitch = (float)(-CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}
	if (Application::IsKeyPressed(VK_DOWN)) {
		float pitch = (float)(CAMERA_SPEED * dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		position = rotation * position;
	}
	if (Application::IsKeyPressed('N')) {
		Vector3 direction = target - position;
		if (direction.Length() > 5) {
			Vector3 view = (target - position).Normalized();
			position += view * (float)(50.f * dt);
		}
	}
	if (Application::IsKeyPressed('M')) {
		Vector3 view = (target - position).Normalized();
		position -= view * (float)(50.f * dt);
	}
	if (Application::IsKeyPressed('R')) {
		Reset();
	}
}

void Camera2::Reset() {
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}
