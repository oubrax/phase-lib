#pragma once
#include "surface/drawing_surface.hpp"
#include "widget.hpp"

#include <algorithm>

template <typename R>
class Application {

    R renderer;
public:
    explicit Application(R renderer): renderer(std::move(renderer)) {}



    WindowId spawn_window(WindowOptions options) {
       return renderer.create_surface(options);
    }

    static void render(WindowId id, ) {
        // in future would add the widget data per window
    }

    void run() {
        const auto& surfaces = renderer.get_surfaces();

        // check main window for now
        while (!surfaces[0]->should_close()) {
            unsigned int id = 0;
            for (const auto &surface : surfaces) {
                renderer.process(WindowId(id), {});
                id++;
            }
        };

    }
};