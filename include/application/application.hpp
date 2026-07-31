#pragma once
#include "surface/drawing_surface.hpp"
#include "widget.hpp"
#include "widgets/container/container.hpp"
#include "widgets/container/prop.hpp"

#include <algorithm>
#include <concepts>
#include <memory>

template <typename R>
    requires std::derived_from<R, Renderer<R>>
class Application {
    R renderer;
    std::vector<std::function<std::unique_ptr<Widget>()>> renderlist;

public:
    explicit Application(R renderer) : renderer(std::move(renderer)) {}

    SurfaceId spawn_surface(SurfaceOptions options) { return renderer.create_surface(options); }

    template <typename F> void render(SurfaceId surface, F render) {
        if (surface.id >= renderlist.size()) {
            renderlist.resize(2 * surface.id + 1);
        }
        renderlist[surface.id] = [render]() {
            return std::make_unique<decltype(render())>(render());
        };
    }

    void run() {
        while (true) {
            bool should_break = false;
            std::vector<SurfaceId> to_destroy;

            renderer.foreach_surface(
                // lambda to check for close conditions
                [this, &should_break, &to_destroy](SurfaceId id, DrawingSurface* surface) {
                    if (surface->should_close()) {
                        // if main surface closes, close everything
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

            renderer.foreach_surface([this](SurfaceId id, DrawingSurface* surface) {
                RendererCtx ctx = RendererCtx();
                std::unique_ptr<Widget> inner = renderlist[id.id]();
                auto &c = dynamic_cast<Container&>(*inner.get());
                auto widget = Container(
                    w(px(surface->width)),
                    h(px(surface->height)),
                    child(std::move(c))
                );
                widget.measure();
                widget.arrange();
                widget.draw(ctx);

                renderer.process(id, ctx.draw_cmds);
            });
        }
    }
};
