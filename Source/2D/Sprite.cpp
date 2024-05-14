#include "Sprite.h"
#include <glad/glad.h>
#include "../Rendering/Shader.h"
#include "../Math/Matrix/Mat4x4.h"
#include "Camera2D.h"

const float quad[] = {
	//Pos      //Texture coordinates
	1,   1, 0, 1, 1, //Top right
	1,  -1, 0, 1, 0, //Bottom right
	-1, -1, 0, 0, 0, //Bottom left
	-1,  1, 0, 0, 1  //Top left
};

const unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

Sprite::Sprite(const std::string& textureLocation)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data = stbi_load(textureLocation.c_str(), &width, &height, &channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_RGBA : GL_RGB, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

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
		SpriteShader = std::unique_ptr<Shader>(new Shader("Shader/vertex.shader", "Shader/fragment.shader"));

	SpriteShader->Use();

	glUniform1i(glGetUniformLocation(SpriteShader->programID, "texture1"), 0);
}

void Sprite::Render()
{
	SpriteShader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(quadVAO);

	Camera2D::Get()->Draw(*SpriteShader, Vec2f(1, 0.25f));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}


std::unique_ptr<Shader> Sprite::SpriteShader = nullptr;