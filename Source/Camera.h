#pragma once

#include "Math/Vector.h"
#include "Rendering/Window.h"
#include "Math/Matrix/Mat4x4.h"

//Only allow a single camera for now
class Camera
{
public:
	Camera();
	Camera(Camera&) = delete;
	void operator=(const Camera&) = delete;
	~Camera();

	Vec3f GetPosition() const;
	Vec3f GetFront() const;
	Mat4x4f GetViewMatrix();

	static Camera* Get();

private:
	void Viewbob(Vec3f movement);

	static Camera* Instance;

	float yaw = -90;
	float pitch = 0;

	Vec3f position = Vec3f(0, 1, 3);
	Vec3f front = Vec3f(0, 0, -1);
	Vec3f up = Vec3f(0, 1, 0);

	Vec2i lastMousePos = Vec2i(Window::StartWindowY + Window::StartWindowWidth / 2, Window::StartWindowX + Window::StartWindowHeight);

	float viewbobY = 0;

	static constexpr float sensitivity = 0.1f;
};

