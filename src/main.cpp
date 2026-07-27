#include "application/application.hpp"
#include "renderer/gl.hpp"
#include "surface/gl_glfw_surface.hpp"
#include "widgets/container/container.hpp"
#include "widgets/container/prop.hpp"

int main() {
    auto app = Application(OpenGLRenderer());

    auto window = app.spawn_window(WindowOptions().title("Main Window"));

    app.render(window);

    app.run();

    return 0;
}
