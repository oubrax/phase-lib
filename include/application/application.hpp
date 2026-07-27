#pragma once
#include "surface/drawing_surface.hpp"
#include "widget.hpp"

template<typename R>
class Application {

    Renderer<R> renderer;
public:
    explicit Application(Renderer<R> renderer): renderer(renderer) {

    }



    WindowHandle spawn_window(const WindowOptions& options) {
        return renderer.get_surface().create_surface(options);
    }

    void render(WindowHandle window) {
        
    }
};