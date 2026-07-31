#pragma once
#include "shaders.hpp"
#include "surface/gl_glfw_surface.hpp"
#include "widget.hpp"

#include <iostream>
#include <ranges>
#include <string>
#include <utility>


struct Vertex {
    float x,y;
    float u,v;
};

struct QuadData {
    float r,g,b,a;
    float x, y;
    float width, height;
};

// simple abstraction over the vao, ebo and vbo
class Buffer {
    unsigned int vao{};
    unsigned int vbo{};
    unsigned int ebo{};
    // for instancing
    unsigned int quad_vbo{};


public:
    std::vector<QuadData> quads;

    Buffer() {
        constexpr Vertex vertices[] = {
            {.x = 0.0, .y = 0.0, .u = 0.0, .v = 0.0},
            {.x = 1.0, .y = 0.0, .u = 1.0, .v = 0.0},
            {.x = 0.0, .y = 1.0, .u = 0.0, .v = 1.0},
            {.x = 1.0, .y = 1.0, .u = 1.0, .v = 1.0},
        };
        const unsigned int indices[] = {
            0, 1, 2,
            1, 3, 2
        };
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glGenBuffers(1, &quad_vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, u))));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
        glBufferData(GL_ARRAY_BUFFER, 4096 * sizeof(QuadData), nullptr, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(QuadData), reinterpret_cast<void*>(offsetof(QuadData, r)));
        glEnableVertexAttribArray(2);
        glVertexAttribDivisor(2, 1);


        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), reinterpret_cast<void*>(offsetof(QuadData, width)));
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);


        glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(QuadData), reinterpret_cast<void*>(offsetof(QuadData, x)));
        glEnableVertexAttribArray(4);
        glVertexAttribDivisor(4, 1);
    };


    void render() {
        std::cout << quads.size() << std::endl;
        glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, quads.size() * sizeof(QuadData), quads.data());
        glBindVertexArray(vao);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, quads.size());
        quads.clear();
    }


    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept
        : vao(std::exchange(other.vao, 0)),
          vbo(std::exchange(other.vbo, 0)),
          ebo(std::exchange(other.ebo, 0)),
          quad_vbo(std::exchange(other.quad_vbo, 0)),
          quads(std::move(other.quads)) {}

    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            if (quad_vbo) glDeleteBuffers(1, &quad_vbo);
            if (vbo) glDeleteBuffers(1, &vbo);
            if (ebo) glDeleteBuffers(1, &ebo);
            if (vao) glDeleteVertexArrays(1, &vao);

            vao = std::exchange(other.vao, 0);
            vbo = std::exchange(other.vbo, 0);
            ebo = std::exchange(other.ebo, 0);
            quad_vbo = std::exchange(other.quad_vbo, 0);
            quads = std::move(other.quads);
        }
        return *this;
    }

    ~Buffer() {
        if (quad_vbo) glDeleteBuffers(1, &quad_vbo);
        if (vbo) glDeleteBuffers(1, &vbo);
        if (ebo) glDeleteBuffers(1, &ebo);
        if (vao) glDeleteVertexArrays(1, &vao);
    }
};


class Program {
    unsigned int gl_program;

    static void checkCompileErrors(const unsigned int shader, const bool program) {
        int success;
        char infoLog[1024];
        if (!program) {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "SHADER_COMPILATION_ERROR " << "\n"
                          << infoLog << "\n";
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "PROGRAM_LINKING_ERROR of type: " << "\n" << infoLog << "\n";
            }
        }
    }
public:
    Program(const std::string_view vertex, const std::string_view frag) {
        const auto vertex_src = std::string(vertex);
        const auto frag_src = std::string(frag);
        const unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        const unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vertex_ptr = vertex_src.c_str();
        glShaderSource(vertex_shader, 1, &vertex_ptr, nullptr);
        glCompileShader(vertex_shader);
        checkCompileErrors(vertex_shader, false);

        const char* frag_ptr = frag_src.c_str();
        glShaderSource(frag_shader, 1, &frag_ptr, nullptr);
        glCompileShader(frag_shader);
        checkCompileErrors(frag_shader, false);

        const unsigned int shader = glCreateProgram();
        glAttachShader(shader, vertex_shader);
        glAttachShader(shader, frag_shader);
        glLinkProgram(shader);
        checkCompileErrors(shader, true);

        glDeleteShader(vertex_shader);
        glDeleteShader(frag_shader);

        gl_program = shader;
    }

    Program(const Program&) = delete;
    Program& operator=(const Program&) = delete;

    Program(Program&& other) noexcept : gl_program(std::exchange(other.gl_program, 0)) {}

    Program& operator=(Program&& other) noexcept {
        if (this != &other) {
            if (gl_program) glDeleteProgram(gl_program);
            gl_program = std::exchange(other.gl_program, 0);
        }
        return *this;
    }

    void use() const {
        glUseProgram(gl_program);
    }

    void set_projection(const float width, const float height) const {
        const float ortho[16] = {
            2.f / width, 0.f, 0.f, 0.f,
            0.f, -2.f / height, 0.f, 0.f,
            0.f, 0.f, -1.f, 0.f,
            -1.f, 1.f, 0.f, 1.f,
        };
        glUniformMatrix4fv(glGetUniformLocation(gl_program, "u_projection"), 1, GL_FALSE, ortho);
    }

    ~Program() {
        if (gl_program) glDeleteProgram(gl_program);
    }
};

struct SurfaceGLState {
    Program program;
    Buffer buffer;

    SurfaceGLState() : program(VERTEX_SHADER, FRAGMENT_SHADER) {}
};

class OpenGLRenderer : public Renderer<OpenGLRenderer> {
    std::unordered_map<SurfaceId, GlfwOpenGlSurface, SurfaceId::hash> surfaces;
    std::unordered_map<SurfaceId, SurfaceGLState, SurfaceId::hash> gl_states;

    int id_counter = 0;

    static void draw(DrawCmd& cmd, Buffer& buffer) {
        switch (cmd.tag) {
        case CmdTag::Rect: {
            buffer.quads.push_back(QuadData {
                .r = cmd.value.rect.r,
                .g = cmd.value.rect.g,
                .b = cmd.value.rect.b,
                .a = cmd.value.rect.a,
                .x = cmd.value.rect.x,
                .y = cmd.value.rect.y,
                .width = cmd.value.rect.w,
                .height = cmd.value.rect.h,
            });
            break;
        }
        }
    }

public:
    explicit OpenGLRenderer() = default;
    SurfaceId create_surface(const SurfaceOptions& options) {
        const auto id = id_counter++;

        // use try_emplace so the glfw pointer stays valid
        surfaces.try_emplace(SurfaceId(id), options);

        // the surface constructor made its context current; create its GL state in it
        gl_states.emplace(SurfaceId(id), SurfaceGLState());
        gl_states.at(SurfaceId(id)).program.use();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return SurfaceId(id);
    }

    void destroy_surface(const SurfaceId id) {
        const auto surface = surfaces.find(id);
        assert(surface != surfaces.end());

        // GL object names are per-context, so delete this surface's objects with
        // its own context current, or we'd delete another surface's objects
        surface->second.make_current();
        gl_states.erase(id);
        surface->second.destroy();
        surfaces.erase(id);
    }

    void destroy_all_surfaces() {
        for (auto& [id, surface] : surfaces) {
            surface.make_current();
            gl_states.erase(id);
            surface.destroy();
        }
        gl_states.clear();
        surfaces.clear();
    }

    void foreach_surface(const std::function<void(SurfaceId id, DrawingSurface*)>& callback) {
        for (auto& [key, value] : surfaces) {
            callback(key, &value);
        }
    }

    void process(const SurfaceId window, const std::span<const DrawCmd> cmds) {
        assert(surfaces.contains(window));
        auto* surface = &(surfaces.find(window)->second);
        surface->make_current();
        surface->poll_events();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        auto& state = gl_states.at(window);
        for (auto cmd : cmds) {
            draw(cmd, state.buffer);
        }

        state.program.use();
        state.program.set_projection(static_cast<float>(surface->width), static_cast<float>(surface->height));
        state.buffer.render();

        surface->swap_buffers();
    }

    OpenGLRenderer(OpenGLRenderer&&) = default;
    OpenGLRenderer& operator=(OpenGLRenderer&&) = default;

    ~OpenGLRenderer() { GlfwOpenGlSurface::global_cleanup(); }
};
