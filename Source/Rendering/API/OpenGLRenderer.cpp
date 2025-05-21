#include "OpenGLRenderer.h"

#include <algorithm>
#include <glad.h>
#include "../../Debugging/Log.h"
#include "../../Math/Color.h"

OpenGLRenderer::OpenGLRenderer(const HDC& hdc)
: Renderer(hdc)
{
    CreatePixelFormat();
    
    glContext = wglCreateContext(deviceContext);
    wglMakeCurrent(deviceContext, glContext);
    
    if(!gladLoadGL())
    {
        greg::log::Fatal("Rendering", "Failed to initialize OpenGL context.");
    }

    glEnable(GL_DEPTH_TEST);
}



OpenGLRenderer::~OpenGLRenderer()
{
    wglDeleteContext(glContext);
}

void OpenGLRenderer::Render(const Color& clearColor)
{
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(size_t i = 0; i < primitives.size(); i++)
    {
        std::weak_ptr<Primitive>& weakPrimitive = primitives[i];
        std::shared_ptr<Primitive> primitive = weakPrimitive.lock();

        if(!primitive)
        {
            std::swap(weakPrimitive, *(std::end(primitives) - 1));
            primitives.erase(std::end(primitives) - 1);
            i--;
            continue;
        }

        primitive->Draw();
    }
    
    wglSwapLayerBuffers(deviceContext, WGL_SWAP_MAIN_PLANE);

    primitivesToClear.clear();
}

void OpenGLRenderer::CreatePixelFormat()
{
    //Taken from https://www.khronos.org/opengl/wiki/Creating_an_OpenGL_Context_%28WGL%29
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
    SetPixelFormat(deviceContext, pixelFormat, &pfd);
}

void OpenGLRenderer::SetupPrimitive(std::shared_ptr<Primitive> primitive) {}
