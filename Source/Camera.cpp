#include "Camera.h"
#include <algorithm>
#include "GregTime.h"
#include "Input/Mouse.h"
#include "Input/Mouse.h"
#include "Math/Trigonometry.h"
#include "Input/Keyboard.h"
#include <glad.h>

static constexpr bool USE_FPS_CAMERA = 0;

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

Vec3f Camera::GetPosition() const
{
	return position;
}

Vec3f Camera::GetFront() const
{
	return front;
}

Mat4x4f Camera::GetViewMatrix()
{
	float speed = 5 * Time::GetDeltaTime();

	Vec2i currentMousePos = Mouse::GetRawPosition();

	Vec2f mouseMovement(currentMousePos.x - lastMousePos.x, currentMousePos.y - lastMousePos.y);
	mouseMovement *= sensitivity;

	lastMousePos = currentMousePos;

	yaw += mouseMovement.x;
	pitch += mouseMovement.y;

	pitch = std::clamp(pitch, -89.0f, 89.0f);

	front.x = cos(trigonometry::Radians(yaw)) * cos(trigonometry::Radians(pitch));
	front.y = sin(trigonometry::Radians(pitch));
	front.z = sin(trigonometry::Radians(yaw)) * cos(trigonometry::Radians(pitch));
	front.Normalize();

	if(!USE_FPS_CAMERA)
	{
		// Flying movement
		//Forward/back
		position += front * speed * (Keyboard::IsKeyDown(W) - Keyboard::IsKeyDown(S));

		//Left/right
		position += front.Cross(up).Normalize() * speed * (Keyboard::IsKeyDown(D) - Keyboard::IsKeyDown(A));

		//Up/down
		position += Vec3f(0, -1, 0) * speed * (Keyboard::IsKeyDown(Space) - Keyboard::IsKeyDown(LeftControl));
	}
	else
	{
		Vec3f lastPos = position;

		Vec3f groundedFront = Vec3f(front.x, 0, front.z).Normalize();
		position += groundedFront * speed * (Keyboard::IsKeyDown(W) - Keyboard::IsKeyDown(S));
		position += groundedFront.Cross(up).Normalize() * speed * (Keyboard::IsKeyDown(D) - Keyboard::IsKeyDown(A));

		Vec3f movement = position - lastPos;

		if(movement.Magnitude())
		{
			Viewbob(movement);
		}
	}

	return mat4x4::LookAt(position, position + front, up);
}

void Camera::Viewbob(Vec3f movement)
{
	viewbobY += movement.Normalized().Magnitude();
	position.y = sin(viewbobY * 0.035f) * 0.5f;
}

Camera* Camera::Get()
{
	return Instance;
}

Camera* Camera::Instance = nullptr;