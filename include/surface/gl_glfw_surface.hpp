#pragma once
#include "GLFW/glfw3.h"
#include "application/window.hpp"
#include "drawing_surface.hpp"
#include "glad/glad.h"

#include <cassert>
#include <string>
#include <string_view>

class GlfwOpenGlSurface final : public DrawingSurface {
static GLFWwindow* from_handle(const WindowHandle handle) {
    return static_cast<GLFWwindow*>(handle.get());
}
public:
    void init() override { glfwInit(); }

    void destroy(const WindowHandle handle) override {
        glfwDestroyWindow(from_handle(handle));
    }

    WindowHandle create_surface(const WindowOptions& options) override {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        const std::string title(options.window_name);

        GLFWwindow* handle = nullptr;
        switch (options.window_type) {
        case WindowType::Default: {
            handle = glfwCreateWindow(options.window_width, options.window_height,
                                              title.c_str(), nullptr, nullptr);
            break;
        }

        case WindowType::Borderless: {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            handle = glfwCreateWindow(options.window_width, options.window_height,
                                              title.c_str(), nullptr, nullptr);
            break;
        };
        case WindowType::Fullscreen: {

            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            if (!primaryMonitor) {
                printf("FAILED TO LOAD PRIMARY MONITOR");
                return WindowHandle(nullptr);
            }

            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

            handle = glfwCreateWindow(mode->width, mode->height, title.c_str(),
                                              nullptr, nullptr);
            break;
        }
        }

        glfwMakeContextCurrent(handle);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        return WindowHandle(handle);
    }

    void swap_buffers(const WindowHandle handle) override {
        glfwSwapBuffers(from_handle(handle));
    }

    void poll_events(const WindowHandle handle) override {
        glfwMakeContextCurrent(from_handle(handle));
        glfwPollEvents();
    }

    bool should_close(const WindowHandle handle) override {
        glfwMakeContextCurrent(from_handle(handle));
        return glfwWindowShouldClose(from_handle(handle));
    }

    ~GlfwOpenGlSurface() override {
        glfwTerminate();
    };

};
