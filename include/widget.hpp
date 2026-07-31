#pragma once
#include "application/window.hpp"
#include "surface/drawing_surface.hpp"

#include <functional>
#include <memory>
#include <span>
#include <vector>

enum class CmdTag {
    Rect,
};

enum class Unit {
    Fit,
    Grow,
    Pct,

    Exact,
};

struct ScreenUnit {
    Unit unit;
    union ScreenUnion {
        float px;
        float pct;
    } value;
};

struct Rect {
    float x, y, w, h;
    float r = 0, g = 0, b = 0, a = 0;
};

struct DrawCmd {
    CmdTag tag;
    union DrawUnion {
        Rect rect;
    } value;
};

template <typename R> class Renderer {
public:
    void process(SurfaceId id, std::span<const DrawCmd> cmds) {
        static_cast<R*>(this)->process(id, cmds);
    }

    SurfaceId create_surface(SurfaceOptions& options) {
        return static_cast<R*>(this)->create_surface(options);
    }

    void foreach_surface(std::function<void(SurfaceId id, DrawingSurface*)> fn) {
        static_cast<R*>(this)->foreach_surface(std::move(fn));
    }

    void destroy_surface(SurfaceId id) { static_cast<R*>(this)->destroy_surface(id); }

    void destroy_all_surfaces() { static_cast<R*>(this)->destroy_all_surfaces(); }
};

class RendererCtx {

public:
    std::vector<DrawCmd> draw_cmds;
    void push_rect(Rect rect) {
        draw_cmds.push_back({
            .tag = CmdTag::Rect,
            .value = {.rect = rect},
        });
    };
};

struct Layout {
    float x, y;
    float w, h;

    float measured_w = 0;
    float measured_h = 0;

    float growth_w = 0;
    float growth_h = 0;

    float x_pct = 0;
    float y_pct = 0;

    bool x_growth;
    bool y_growth;
};

class Widget {
public:
    Layout layout = Layout{
        .x = 0,
        .y = 0,
        .w = 0,
        .h = 0,
        .measured_w = 0,
        .measured_h = 0,
        .growth_w = 0,
        .growth_h = 0,
        .x_pct = 0,
        .y_pct = 0,
        .x_growth = false,
        .y_growth = false,
    };

    // Measure layout pass: calculate the widget's (and optionally the children's)
    // sizes. (Edits the public `layout`)
    virtual void measure() = 0;
    // Arrange layout pass: calculate the widget's (and optionally the children's)
    // positions. (Edits the public `layout`)
    virtual void arrange() = 0;

    virtual void draw(RendererCtx& ctx) = 0;

    virtual ~Widget() = default;
};

class ParentWidget : public Widget {
public:
    std::vector<std::unique_ptr<Widget>> children;
};

ScreenUnit px(float px);
ScreenUnit pct(float pct);
ScreenUnit grow();
