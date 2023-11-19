#pragma once

#include "Math/Vector.h"
#include "Window.h"
#include "Math/Matrix/Mat4x4.h"

//Only allow a single camera for now
class Camera
{
public:
	Camera();
	Camera(Camera&) = delete;
	void operator=(const Camera&) = delete;
	~Camera();

	Mat4x4f GetViewMatrix();

	static Camera* Get();

private:
	static Camera* Instance;

	float yaw = -90;
	float pitch = 0;

	Vec3f cameraPos = Vec3f(0, 0, 3);
	Vec3f cameraFront = Vec3f(0, 0, -1);
	Vec3f cameraUp = Vec3f(0, 1, 0);

	Vec2i lastPos = Vec2i(Window::WindowY + Window::WindowWidth / 2, Window::WindowX + Window::WindowHeight);

	static constexpr float sensitivity = 0.1f;
};

