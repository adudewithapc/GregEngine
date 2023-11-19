#include "Camera.h"
#include <algorithm>
#include "Time.h"
#include "Input/Mouse.h"
#include "Input/Mouse.h"
#include "Math/Trigonometry.h"
#include "Input/Keyboard.h"
#include <glad/glad.h>

Camera::Camera()
{
	if(Instance == nullptr)
	{
		Instance = this;
	}
}

Camera::~Camera()
{
	Instance = nullptr;
}

Mat4x4f Camera::GetViewMatrix()
{

	float cameraSpeed = 5 * Time::GetDeltaTime();

	Vec2i currentPos = Mouse::GetRawPosition();

	Vec2f movement(currentPos.x - lastPos.x, currentPos.y - lastPos.y);
	movement *= sensitivity;

	lastPos = currentPos;

	yaw += movement.x;
	pitch += movement.y;

	pitch = std::clamp(pitch, -89.0f, 89.0f);

	cameraFront.x = cos(trigonometry::Radians(yaw)) * cos(trigonometry::Radians(pitch));
	cameraFront.y = sin(trigonometry::Radians(pitch));
	cameraFront.z = sin(trigonometry::Radians(yaw)) * cos(trigonometry::Radians(pitch));
	cameraFront.Normalize();

	//Forward/back
	cameraPos += cameraFront * cameraSpeed * (Keyboard::IsKeyDown(W) - Keyboard::IsKeyDown(S));

	//Left/right
	cameraPos += cameraFront.Cross(cameraUp).Normalize() * cameraSpeed * (Keyboard::IsKeyDown(D) - Keyboard::IsKeyDown(A));

	//Up/down
	cameraPos += Vec3f(0, -1, 0) * cameraSpeed * (Keyboard::IsKeyDown(Space) - Keyboard::IsKeyDown(LeftControl));

	return mat4x4::LookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

Camera* Camera::Get()
{
	return Instance;
}

Camera* Camera::Instance = nullptr;