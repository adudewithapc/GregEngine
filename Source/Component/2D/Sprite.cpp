#include "Sprite.h"
#include <glad.h>
#include "../../GameObject.h"
#include "../../2D/Camera2D.h"
#include "../../Rendering/Window.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "../../Image.h"

const unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

Sprite::Sprite(GameObject* owner, const std::string& textureLocation) : Component(owner)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data = nullptr;
	//Testing with custom file reading
	if(textureLocation != "basket_man.bmp")
	{
		data = stbi_load(textureLocation.c_str(), &width, &height, &channels, 0);
	}
	else
	{
		data = greg::image::LoadBMP(textureLocation, width, height, channels);
		if(data == nullptr)
			std::cerr << "Failed to load BMP\n";
	}

	glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_RGBA : GL_RGB, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	if(textureLocation != "basket_man.bmp")
		stbi_image_free(data);

	int vertexElements;
	const std::unique_ptr<float> proportions = GetScreenProportions(width, height, vertexElements);
	const float* const proportionsPointer = proportions.get();

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(proportionsPointer) * vertexElements, proportionsPointer, GL_STATIC_DRAW);

	glGenBuffers(1, &quadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if(!SpriteShader)
		SpriteShader = std::make_unique<Shader>("Resources/Shader/vertex.shader", "Resources/Shader/fragment.shader");

	SpriteShader->Use();

	glUniform1i(glGetUniformLocation(SpriteShader->programID, "texture1"), 0);
}

void Sprite::Draw()
{
	SpriteShader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(quadVAO);

	Camera2D::Get().Draw(*SpriteShader, Window::PixelToView(GetOwner()->Position));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

std::unique_ptr<float> Sprite::GetScreenProportions(const float textureWidth, const float textureHeight, int& elements) const
{
	elements = 20;
	const float widthOnWindow = textureWidth / Window::WindowWidth;
	const float heightOnWindow = textureHeight / Window::WindowHeight;
	float* proportions = new float[elements] {
		//x              y               z tx ty
		widthOnWindow,   heightOnWindow, 0, 1, 0, //Top right
		widthOnWindow,  -heightOnWindow, 0, 1, 1, //Bottom right
		-widthOnWindow, -heightOnWindow, 0, 0, 1, //Bottom left
		-widthOnWindow,  heightOnWindow, 0, 0, 0  //Top left
	};
	std::unique_ptr<float> ret(proportions);
	return std::move(ret);
}


std::unique_ptr<Shader> Sprite::SpriteShader = nullptr;