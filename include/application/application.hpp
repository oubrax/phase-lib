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

    static void render(WindowId id) {
        // in future would add the widget data per window
    }

    void run() {
        while (true) {
            bool should_break = false;
            std::vector<WindowId> to_destroy;

            renderer.foreach_surface(
                // lambda to check for close conditions
                [this, &should_break, &to_destroy](WindowId id, DrawingSurface* surface) {
                if (surface->should_close()) {
                    // if main window closes, close everything
                    if (id.id == 0) {
                        should_break = true;
                   } else {
                       // mark for destruction
                       to_destroy.push_back(id);
                   }
                }
            });

            if (should_break) {
                renderer.destroy_all_surfaces();
                break;
            }

            for (auto id : to_destroy) {
                renderer.destroy_surface(id);
            }

            renderer.foreach_surface([this](WindowId id, DrawingSurface*) {
                renderer.process(id, {});
            });
        }
    }
};