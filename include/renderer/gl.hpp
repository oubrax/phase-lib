#pragma once
#include "surface/gl_glfw_surface.hpp"
#include "widget.hpp"

class OpenGLRenderer: public Renderer<OpenGLRenderer> {
    std::vector<std::unique_ptr<DrawingSurface>> surfaces;

    static void draw(DrawCmd &cmd) {

    }

public:
    explicit OpenGLRenderer() = default;

    WindowId create_surface(const WindowOptions& options) {
        auto surface = std::make_unique<GlfwOpenGlSurface>(options);
        const auto id = surfaces.size();

        // push to surfaces
        surfaces.push_back(std::move(surface));

        return WindowId(id);

    }

    const std::vector<std::unique_ptr<DrawingSurface>>& get_surfaces() {
        return surfaces;
    }

    void process(const WindowId window, const std::span<const DrawCmd> cmds) const {
        const auto surface = surfaces[window.id].get();
        surface->make_current();
        surface->poll_events();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto cmd: cmds) {
            draw(cmd);
        }

        surface->swap_buffers();
    }

    OpenGLRenderer(OpenGLRenderer&&) = default;
    OpenGLRenderer& operator=(OpenGLRenderer&&) = default;

    ~OpenGLRenderer() {
        GlfwOpenGlSurface::global_cleanup();
    }
};