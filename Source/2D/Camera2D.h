#pragma once

#include "../Math/Matrix/Mat4x4.h"
#include "../Rendering/Shader.h"
#include "../Math/Vector.h"

class Camera2D
{
public:
	Camera2D() = default;
	
	Camera2D(Camera2D&) = delete;
	Camera2D& operator=(const Camera2D&) = delete;

	Camera2D(Camera2D&&) = delete;
	Camera2D& operator=(Camera2D&&) = delete;

	Mat4x4f GetViewMatrix() const;
	void Update();
	void Draw(Shader& shader, const Mat4x4f& modelMatrix);

	static Camera2D& Get();
private:
	Vec2f Position;

	static inline Mat4x4f ProjectionMatrix = mat4x4::OrthographicView(0, 800, 0, 600, 1, 100);
};

