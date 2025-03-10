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

void Camera2D::Draw(Shader& shader, const Vec2f& position)
{
	Mat4x4f positionMatrix = mat4x4::Identity<float>.Translate(position);
	glUniformMatrix4fv(glGetUniformLocation(shader.programID, "model"), 1, GL_TRUE, positionMatrix.ToArray());
	glUniformMatrix4fv(glGetUniformLocation(shader.programID, "view"), 1, GL_TRUE, GetViewMatrix().ToArray());
	glUniformMatrix4fv(glGetUniformLocation(shader.programID, "projection"), 1, GL_TRUE, mat4x4::Identity<float>.ToArray());
}

Camera2D& Camera2D::Get()
{
	static Camera2D instance;
	return instance;
}