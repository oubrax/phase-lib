#pragma once
#include <cassert>
#include <string>
#include <string_view>

#include "DrawingSurface.hpp"
#include "GLFW/glfw3.h"
#include "glad/glad.h"


class GlfwOpenGlSurface final : public DrawingSurface {
public:
    void init() override {
        glfwInit();
    }

    void destroy() override {
        glfwTerminate();
    }

    void create_surface(const std::string_view name, const int width, const int height) override {
        assert(!m_WindowHandle);  // ensure this is not called twice
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_WindowTitleAlloc = name;
        m_WindowHandle = glfwCreateWindow(width, height, m_WindowTitleAlloc.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_WindowHandle);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    void swap_buffers() override {
        assert(m_WindowHandle);
        glfwSwapBuffers(m_WindowHandle);
    }

    void poll_events() override {
        glfwPollEvents();
    }

    bool should_close() override {
        assert(m_WindowHandle);
        return glfwWindowShouldClose(m_WindowHandle);
    }

    ~GlfwOpenGlSurface() override = default;

private:
    GLFWwindow* m_WindowHandle = nullptr;
    std::string m_WindowTitleAlloc;
};