#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "application/window.hpp"
#include "drawing_surface.hpp"

#include <cassert>
#include <string>
#include <string_view>


class GlfwOpenGlSurface final : public DrawingSurface {
public:
    explicit GlfwOpenGlSurface(const WindowOptions& options) {
        glfwInit();
        assert (!m_window_handle);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        const std::string title(options.window_name);

        switch (options.window_type) {
        case WindowType::Default: {
            m_window_handle = glfwCreateWindow(options.window_width, options.window_height,
                                              title.c_str(), nullptr, nullptr);
            break;
        }

        case WindowType::Borderless: {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            m_window_handle = glfwCreateWindow(options.window_width, options.window_height,
                                              title.c_str(), nullptr, nullptr);
            break;
        }

        case WindowType::Fullscreen: {

            GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
            if (!primaryMonitor) {
                printf("FAILED TO LOAD PRIMARY MONITOR");
                glfwTerminate();
                return;
            }

            const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

            m_window_handle = glfwCreateWindow(mode->width, mode->height, title.c_str(),
                                              nullptr, nullptr);
            break;
        }
        }

        glfwMakeContextCurrent(m_window_handle);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    void destroy() override {
        assert(m_window_handle);
        glfwDestroyWindow(m_window_handle);
    }

    void make_current() override {
        assert(m_window_handle);
        glfwMakeContextCurrent(m_window_handle);
    }

    void swap_buffers() override {
        assert(m_window_handle);
        glfwSwapBuffers(m_window_handle);
    }

    void poll_events() override {
        assert(m_window_handle);
        glfwPollEvents();
    }

    bool should_close() override {
        assert(m_window_handle);
        return glfwWindowShouldClose(m_window_handle);
    }

    static void global_cleanup() {
        glfwTerminate();
    }

private:
    GLFWwindow *m_window_handle = nullptr;
};
