#include "Camera2D.h"
#include <glad.h>

Mat4x4f Camera2D::GetViewMatrix() const
{
	Mat4x4f view = mat4x4::Identity<float>;
	view(3, 0) = Position.x;
	view(3, 1) = Position.y;
	return view;
}

void Camera2D::Update() {}

void Camera2D::Draw(Shader& shader, const Mat4x4f& modelMatrix)
{
	shader.SetMatrix4x4("model", modelMatrix);
	shader.SetMatrix4x4("view", GetViewMatrix());
    shader.SetMatrix4x4("projection", ProjectionMatrix);
}

Camera2D& Camera2D::Get()
{
	static Camera2D instance;
	return instance;
}