#pragma once
#include "widget.hpp"

class OpenGLRenderer: public Renderer<OpenGLRenderer> {
    DrawingSurface &surface;

    static void draw(DrawCmd &cmd) {

    }

public:
    explicit OpenGLRenderer(DrawingSurface &s): surface(s) {
        init();
    };


    [[nodiscard]] DrawingSurface &get_surface() const {
        return surface;
    }

    void init() const {
        surface.init();
    }



    static void process(std::span<const DrawCmd> cmds) {
        for (auto cmd: cmds) {
            draw(cmd);
        }
    }



    ~OpenGLRenderer() = default;
};