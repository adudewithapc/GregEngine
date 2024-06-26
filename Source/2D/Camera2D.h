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
	void Draw(Shader& shader, const Vec2f& modelMatrix);

	static Camera2D& Get();
private:
	Vec2f Position;

	static inline Mat4x4f ProjectionMatrix = mat4x4::OrthographicView(0, 800, 600, 0, 1, 100);
};

