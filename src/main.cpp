#include "application/application.hpp"
#include "renderer/gl.hpp"
#include "surface/gl_glfw_surface.hpp"
#include "widgets/container/container.hpp"
#include "widgets/container/prop.hpp"

int main() {
    auto surface = GlfwOpenGlSurface();
    const auto renderer = OpenGLRenderer(surface);
    auto app = Application(renderer);


    return 0;
}
