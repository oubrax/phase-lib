#include "application/application.hpp"
#include "application/window.hpp"
#include "renderer/gl.hpp"
#include "surface/gl_glfw_surface.hpp"
#include "widgets/container/container.hpp"
#include "widgets/container/prop.hpp"

int main() {
    auto app = Application(OpenGLRenderer());

    const auto window = app.spawn_surface(SurfaceOptions().title("Main Window"));
    const auto window2 = app.spawn_surface(SurfaceOptions().title("Main Window 2"));

    app.render(window2, []() {
       return Container(w(px(100.)), h(px(50.)), color(1., 1., 1.));
    });
    app.render(window, []() {
        return Container(
            Container(w(grow()), h(grow()), pad(5.), child(
                Container(
                    w(grow()), h(px(50.)), color(0.2, 0.18, 0.042)
                )
            )));
    });

    app.run();

    return 0;
}
