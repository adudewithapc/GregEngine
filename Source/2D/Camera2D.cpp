#include "Camera2D.h"

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
	glUniformMatrix4fv(glGetUniformLocation(shader.programID, "model"), 1, GL_TRUE, modelMatrix.ToArray());
	glUniformMatrix4fv(glGetUniformLocation(shader.programID, "view"), 1, GL_TRUE, GetViewMatrix().ToArray());
	glUniformMatrix4fv(glGetUniformLocation(shader.programID, "projection"), 1, GL_TRUE, mat4x4::Identity<float>.ToArray());
}

Camera2D& Camera2D::Get()
{
	static Camera2D instance;
	return instance;
}