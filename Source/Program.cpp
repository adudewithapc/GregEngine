#include <iostream>
#include "Window.h"
#include <glad/glad.h>
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
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
#include "Camera.h"

constexpr bool USE_WIREFRAMES = 0;
constexpr bool PRINT_FRAMERATE = 0;

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
	//Positions       //Texture coordinates
	0.5f,   0.5f, 0,  1, 1, //Top right
	0.5f,  -0.5f, 0,  1, 0, //Bottom right
	-0.5f, -0.5f, 0,  0, 0, //Bottom left
	-0.5f,  0.5f, 0,  0, 1 //Top left
};

const float cube[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

Vec3f cubePositions[] = {
	Vec3f(0.0f,  0.0f,  0.0f),
	Vec3f(2.0f,  5.0f, -15.0f),
	Vec3f(-1.5f, -2.2f, -2.5f),
	Vec3f(-3.8f, -2.0f, -12.3f),
	Vec3f(2.4f, -0.4f, -3.5f),
	Vec3f(-1.7f,  3.0f, -7.5f),
	Vec3f(1.3f, -2.0f, -2.5f),
	Vec3f(1.5f,  2.0f, -2.5f),
	Vec3f(1.5f,  0.2f, -1.5f),
	Vec3f(-1.3f,  1.0f, -1.5f)
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

void CubeInit();
void CubeRender();
void DestroyCube();

void LightInit();
void LightRender();
void DestroyLight();

void InitFloor();
void RenderFloor();
void DestroyFloor();

void ApplyTransforms(Shader* shader, Mat4x4f modelMatrix, Mat4x4f worldMatrix);

const Vec3f lightPos(1.2f, 1, 2);
Camera* camera;

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
	camera = new Camera();

	CubeInit();
	LightInit();
	InitFloor();

	glEnable(GL_DEPTH_TEST);

	Time time;
	std::chrono::time_point previousTime = std::chrono::high_resolution_clock::now();
	
	bool running = true;
	while(running)
	{
		std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<FloatingSeconds>(currentTime - previousTime).count();
		time.Tick(deltaTime);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CubeRender();
		LightRender();
		RenderFloor();

		window.SwapBuffers();

		if(!window.ProcessMessages() || Keyboard::IsKeyDown(Key::Esc))
		{
			std::cout << "Closing window" << std::endl;
			running = false;
		}

		previousTime = currentTime;

		if(PRINT_FRAMERATE)
		{
			std::cout << 1 / deltaTime << "\n";
		}

		if(USE_WIREFRAMES)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}

	DestroyCube();
	DestroyLight();
	DestroyFloor();
	delete camera;
}

Shader* shader;

unsigned int VAO;
unsigned int VBO;

void CubeInit()
{
	shader = new Shader("Shader/custom_vertex.shader", "Shader/custom_fragment.shader");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	shader->Use();
}

void CubeRender()
{
	shader->Use();

	shader->SetVec3("objectColor", 1, 0.5f, 0.31f);
	shader->SetVec3("lightColor", 1, 1, 1);

	Mat4x4f modelMatrix = mat4x4::Identity<float>;
	Mat4x4f worldMatrix = mat4x4::Identity<float>;

	ApplyTransforms(shader, modelMatrix, worldMatrix);

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void DestroyCube()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete shader;
}

unsigned int lightVAO;
unsigned int lightVBO;
Shader* lightingShader;

void LightInit()
{
	lightingShader = new Shader("Shader/lightsource_vertex.shader", "Shader/lightsource_fragment.shader");
	lightingShader->Use();

	lightingShader->SetVec3("objectColor", 1, 0.5f, 0.31f);
	lightingShader->SetVec3("lightColor", 1, 1, 1);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LightRender()
{
	lightingShader->Use();

	Mat4x4f model = mat4x4::Identity<float>.Scale(Vec3f(0.2f, 0.2f, 0.2f));
	Mat4x4f world = mat4x4::Identity<float>.Translate(lightPos);
	ApplyTransforms(lightingShader, model, world);

	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void DestroyLight()
{
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &lightVBO);
	delete lightingShader;
}

Shader* floorShader;

unsigned int texture1;
unsigned int floorVAO;
unsigned int floorVBO;
unsigned int floorEBO;

void InitFloor()
{
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, channels;
	unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glGenVertexArrays(1, &floorVAO);
	glBindVertexArray(floorVAO);

	glGenBuffers(1, &floorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &floorEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	floorShader = new Shader("Shader/vertex.shader", "Shader/fragment.shader");
	floorShader->Use();

	glUniform1i(glGetUniformLocation(floorShader->programID, "texture1"), 0);
}

void RenderFloor()
{
	floorShader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glBindVertexArray(floorVAO);

	Mat4x4f model = mat4x4::Identity<float>.Scale(Vec3f(100, 100, 1));
	Mat4x4f world = mat4x4::Identity<float>.Translate(Vec3f(0, 0, -10)).Rotate(Vec3f(1, 0, 0), std::numbers::pi / 2.0f);

	ApplyTransforms(floorShader, model, world);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DestroyFloor()
{
	glDeleteVertexArrays(1, &floorVAO);
	glDeleteBuffers(1, &floorVBO);
	delete floorShader;
}

void ApplyTransforms(Shader* shader, Mat4x4f model, Mat4x4f world)
{
	//Bigger aspect ratio makes objects taller, smaller makes objects wider
	Mat4x4f projectionMatrix = mat4x4::PerspectiveView(trigonometry::Radians(45.0f), 800.0f / 600.0f, 0.1f, 100);

	int modelLocation = glGetUniformLocation(shader->programID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.ToArray());

	int worldLocation = glGetUniformLocation(shader->programID, "world");
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, world.ToArray());

	int viewLocation = glGetUniformLocation(shader->programID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, camera->GetViewMatrix().ToArray());

	int projectionLocation = glGetUniformLocation(shader->programID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix.ToArray());
}