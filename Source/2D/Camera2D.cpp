#include "Camera2D.h"
#include "../Input/Keyboard.h"
#include "../Time.h"

Camera2D::Camera2D()
{
	if(!Instance)
		Instance = std::shared_ptr<Camera2D>(this);
}

Mat4x4f Camera2D::GetViewMatrix()
{
	Mat4x4f view = mat4x4::Identity<float>;
	view(3, 0) = Position.x;
	view(3, 1) = Position.y;
	return view;
}

void Camera2D::Update()
{
	float horizontal = Keyboard::IsKeyDown(Key::D) - Keyboard::IsKeyDown(Key::A);
	float vertical = Keyboard::IsKeyDown(Key::W) - Keyboard::IsKeyDown(Key::S);

	Vec2f movement(horizontal, vertical);

	Position += movement * Time::GetDeltaTime();
}

void Camera2D::Draw(Shader& shader, Vec2f position)
{
	Mat4x4f positionMatrix = mat4x4::Identity<float>;
	positionMatrix(3, 0) = position.x;
	positionMatrix(3, 1) = position.y;

	glUniformMatrix4fv(glGetUniformLocation(shader.programID, "model"), 1, GL_FALSE, positionMatrix.ToArray());
	glUniformMatrix4fv(glGetUniformLocation(shader.programID, "view"), 1, GL_FALSE, GetViewMatrix().ToArray());
	glUniformMatrix4fv(glGetUniformLocation(shader.programID, "projection"), 1, GL_FALSE, mat4x4::Identity<float>.ToArray());
}

std::shared_ptr<Camera2D> Camera2D::Get()
{
	return Instance;
}

std::shared_ptr<Camera2D> Camera2D::Instance = nullptr;