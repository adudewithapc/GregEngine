#include <iostream>
#include "Window.h"
#include <glad/glad.h>
#include "Input/Keyboard.h"
#include "Rendering/TriangleRenderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <chrono>
#include "Time.h"
#include "Math/Matrix/Mat4x4.h"
#include "Math/Trigonometry.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define USE_WIREFRAMES 0

const float triangle1[] = {
	//Positions        //Colors
	-0.75f,  -0.5f, 0, 1, 0, 0,
	-0.25f,  -0.5f, 0, 0, 1, 0,
	-0.25f,  0.25f, 0, 0, 0, 1
};

const float triangle2[] = {
	//Positions       //Colors
	0.25f,  0.25f, 0, 0, 0, 1,
	0.25f, -0.5f,  0, 0, 1, 0,
	0.75f, -0.5f,  0, 1, 0, 0
};

const float triangle3[] = {
	0.5f,  -0.5f, 0, 1, 0, 0,
	-0.5f, -0.5f, 0, 0, 1, 0,
	0,      0.5f, 0, 0, 0, 1
};

const float quad[] = {
	//Positions       //Colors	//Texture coordinates
	0.5f,   0.5f, 0,  1, 0, 0,	1, 1, //Top right
	0.5f,  -0.5f, 0,  0, 1, 0,	1, 0, //Bottom right
	-0.5f, -0.5f, 0,  0, 0, 1,  0, 0, //Bottom left
	-0.5f,  0.5f, 0,  1, 1, 0,  0, 1 //Top left
};

const float cube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

/*const float vertices2[] = {
	-0.75f, 0.25f, 0,
	-0.75f, -0.5f, 0,
	-0.25f, 0.25f, 0
};*/

unsigned int indices[] = {
	0, 1, 3, //First triangle
	1, 2, 3  //Second triangle
};

void CustomInit();
void CustomRender();
void DestroyRenderer();

int main()
{
	using FloatingSeconds = std::chrono::duration<float>;

	std::cout << "Creating window" << std::endl;

	Window window;

	if(!gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	window.ResizeViewport(Window::WindowWidth, Window::WindowHeight);

	TriangleRenderer triangle1(quad, indices);
	CustomInit();

	glEnable(GL_DEPTH_TEST);

	Time time;
	std::chrono::time_point previousTime = std::chrono::high_resolution_clock::now();
	
	bool running = true;
	while(running)
	{
		std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<FloatingSeconds>(currentTime - previousTime).count();
		time.Tick(deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//triangle1.Draw();
		CustomRender();

		window.SwapBuffers();

		if(!window.ProcessMessages() || Keyboard::IsKeyDown(Key::Esc))
		{
			std::cout << "Closing window" << std::endl;
			running = false;
		}

		previousTime = currentTime;
	}

	DestroyRenderer();
}

Shader* shader;

unsigned int VAO;
unsigned int VBO;
unsigned int texture1;
unsigned int texture2;

void CustomInit()
{
	shader = new Shader("Shader/vertex.shader", "Shader/fragment.shader");

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width2, height2, channels2;
	unsigned char* data2 = stbi_load("Textures/awesomeface.png", &width2, &height2, &channels2, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data2);

	shader = new Shader("Shader/custom_vertex.shader", "Shader/custom_fragment.shader");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	shader->Use();
	shader->SetInt("texture1", 0);
	shader->SetInt("texture2", 1);

	if(USE_WIREFRAMES)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void CustomRender()
{
	shader->Use();

	Mat4x4f modelMatrix = mat4x4::Identity<float>.Rotate(Vec3f(1, 0, 0), trigonometry::Radians(-55.0f)).Rotate(Vec3f(0.447214f, 0.894427f, 0.0f), Time::GetTimeSinceStartup() * trigonometry::Radians(50.0f));

	Mat4x4f viewMatrix = mat4x4::Identity<float>.Translate(Vec3f(0.0f, 0.0f, -3.0f));
	Mat4x4f projectionMatrix = mat4x4::PerspectiveView(trigonometry::Radians(-45.0f), 800.0f / 600.0f, 0.1f, 100);

	int modelLocation = glGetUniformLocation(shader->programID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, modelMatrix.ToArray());

	int viewLocation = glGetUniformLocation(shader->programID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewMatrix.ToArray());

	int projectionLocation = glGetUniformLocation(shader->programID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix.ToArray());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DestroyRenderer()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete shader;
}
