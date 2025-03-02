#include "Renderer.h"

#include <glad.h>
#include "../Debugging/Log.h"

Renderer::Renderer(const HDC& deviceContext)
    : deviceContext(deviceContext)
{
    glContext = wglCreateContext(deviceContext);
    wglMakeCurrent(deviceContext, glContext);
    if(!gladLoadGL())
    {
        greg::log::Fatal("Rendering", "Failed to initialize OpenGL context.");
    }

    glEnable(GL_DEPTH_TEST);
}
Renderer::~Renderer()
{
    wglDeleteContext(glContext);
}
void Renderer::Clear(Vec4<float> color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::SwapBuffers()
{
    wglSwapLayerBuffers(deviceContext, WGL_SWAP_MAIN_PLANE);
}
