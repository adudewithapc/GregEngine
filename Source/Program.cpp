#include <stb_image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "Window.h"
#include <glad/glad.h>
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Rendering/TriangleRenderer.h"
#include <chrono>
#include "Time.h"
#include "Math/Matrix/Mat4x4.h"
#include "Math/Trigonometry.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "2D/Sprite.h"
#include "2D/Camera2D.h"
#include "Level.h"

#define USE_WIREFRAMES 0;
#define PRINT_FRAMERATE 0;

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
	// Vertex positions   // Normals           // Texture coordinates
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

//void CubeInit();
//void CubeRender();
//void DestroyCube();
//
//void LightInit();
//void LightRender();
//void DestroyLight();
//
//void InitFloor();
//void RenderFloor();
//void DestroyFloor();
//
void ApplyTransforms(Shader* shader, const Mat4x4f& modelMatrix);

Vec3f lightPos(0, 1, 0);
Camera2D* camera;

//Bigger aspect ratio makes objects taller, smaller makes objects wider
//const Mat4x4f projectionMatrix = mat4x4::PerspectiveView(trigonometry::Radians(45.0f), 800.0f / 600.0f, 0.1f, 100);
const Mat4x4f projectionMatrix = mat4x4::OrthographicView(0, 800, 0, 600, 1, 100);

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
	camera = new Camera2D();

	/*CubeInit();
	LightInit();
	InitFloor();*/

	glEnable(GL_DEPTH_TEST);

	Time time;
	std::chrono::time_point previousTime = std::chrono::high_resolution_clock::now();

	//Sprite sprite("Textures/awesomeface.png");
	Level level;
	bool running = true;
	while(running)
	{
		std::chrono::time_point currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration_cast<FloatingSeconds>(currentTime - previousTime).count();
		time.Tick(deltaTime);

		camera->Update();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		level.Update();
		level.Draw();

		//CubeRender();
		//LightRender();
		//RenderFloor();

		//sprite.Render();

		window.SwapBuffers();

		if(!window.ProcessMessages() || Keyboard::IsKeyDown(Key::Esc))
		{
			std::cout << "Closing window" << std::endl;
			running = false;
		}

#if PRINT_FRAMERATE
		std::cout << 1 / deltaTime << "\n";
#endif

#if USE_WIREFRAMES
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

		//lightPos = Vec3f(cos(Time::GetTimeSinceStartup()), sin(Time::GetTimeSinceStartup() * 2), sin(Time::GetTimeSinceStartup()));

		previousTime = currentTime;
	}

	/*DestroyCube();
	DestroyLight();
	DestroyFloor();*/
}

Shader* shader;

unsigned int cubeVAO;
unsigned int cubeVBO;
unsigned int cubeTexture;
unsigned int cubeSpecular;
unsigned int cubeEmission;

/*void CubeInit()
{
	shader = new Shader("Shader/cube_vertex_spot.shader", "Shader/cube_fragment_spot.shader");

	//Color map
	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	unsigned char* colorData = stbi_load("Textures/container2.png", &width, &height, &channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(colorData);

	glGenTextures(1, &cubeSpecular);
	glBindTexture(GL_TEXTURE_2D, cubeSpecular);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* specularData = stbi_load("Textures/container2_specular.png", &width, &height, &channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, specularData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(specularData);

	glGenTextures(1, &cubeEmission);
	glBindTexture(GL_TEXTURE_2D, cubeEmission);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* emissionData = stbi_load("Textures/matrix.jpg", &width, &height, &channels, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, emissionData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(emissionData);

	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader->Use();
	shader->SetInt("material.diffuse", 0);
	shader->SetInt("material.specular", 1);
	shader->SetInt("material.emission", 2);
}

void CubeRender()
{
	shader->Use();
	
	float currentTime = Time::GetTimeSinceStartup();
	Vec3f lightColor(1, 1, 1);

	shader->SetVec3("light.position", camera->GetPosition());
	shader->SetVec3("light.direction", camera->GetFront());
	shader->SetFloat("light.cutoff", cos(trigonometry::Radians(12.5f)));

	shader->SetVec3("light.ambient", lightColor * 0.1f);
	shader->SetVec3("light.diffuse", lightColor * 0.5f);
	shader->SetVec3("light.specular", 1, 1, 1);

	shader->SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
	shader->SetFloat("material.shininess", 32);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTexture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, cubeSpecular);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, cubeEmission);

	glBindVertexArray(cubeVAO);

	for(unsigned int i = 0; i < 10; i++)
	{
		float angle = 20 * i;
		Mat4x4f model = mat4x4::Identity<float>.Rotate(Vec3f(1, 0.3f, 0.5f), trigonometry::Radians(angle)).Translate(cubePositions[i]);
		ApplyTransforms(shader, model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	glBindVertexArray(0);
}

void DestroyCube()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void LightRender()
{
	lightingShader->Use();

	Mat4x4f model = mat4x4::Identity<float>.Scale(Vec3f(0.2f, 0.2f, 0.2f)).Translate(lightPos);
	ApplyTransforms(lightingShader, model);

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

	Mat4x4f model = mat4x4::Identity<float>.Scale(Vec3f(100, 100, 1)).Rotate(Vec3f(1, 0, 0), std::numbers::pi / 2.0f).Translate(Vec3f(0, -10, 0));

	ApplyTransforms(floorShader, model);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DestroyFloor()
{
	glDeleteVertexArrays(1, &floorVAO);
	glDeleteBuffers(1, &floorVBO);
	delete floorShader;
}*/

void ApplyTransforms(Shader* shader, const Mat4x4f& model)
{
	glUniformMatrix4fv(glGetUniformLocation(shader->programID, "model"), 1, GL_FALSE, model.ToArray());
	glUniformMatrix4fv(glGetUniformLocation(shader->programID, "view"), 1, GL_FALSE, camera->GetViewMatrix().ToArray());
	glUniformMatrix4fv(glGetUniformLocation(shader->programID, "projection"), 1, GL_FALSE, projectionMatrix.ToArray());
}