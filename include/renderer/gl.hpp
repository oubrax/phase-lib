#pragma once
#include <ranges>
#include <unordered_map>

#include "surface/gl_glfw_surface.hpp"
#include "widget.hpp"


class OpenGLRenderer: public Renderer<OpenGLRenderer> {
    std::unordered_map<WindowId, GlfwOpenGlSurface, WindowId::hash> surfaces;
    int id_counter = 0;

    static void draw(DrawCmd &cmd) {

    }

public:
    explicit OpenGLRenderer() = default;

    WindowId create_surface(const WindowOptions& options) {
        const auto id = id_counter++;

        surfaces.insert({
            WindowId(id),
            GlfwOpenGlSurface(options)
        });

        return WindowId(id);
    }

    void destroy_surface(WindowId id) {
        const auto surface = surfaces.find(id);
        assert(surface != surfaces.end());

        surface->second.destroy();
        surfaces.erase(id);
    }

    void destroy_all_surfaces() {
        for (auto& surface : surfaces | std::views::values) {
            surface.destroy();
        } surfaces.clear();
    }

    void foreach_surface(const std::function<void (WindowId id, DrawingSurface*)>& callback) {
        for (auto& [key, value] : surfaces) {
            callback(key, &value);
        }
    }

    void process(const WindowId window, const std::span<const DrawCmd> cmds) {
        assert(surfaces.contains(window));
        auto* surface = &(surfaces.find(window)->second);
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