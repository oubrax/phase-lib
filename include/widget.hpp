#pragma once
#include <memory>
#include <span>
#include <vector>

enum class CmdTag {
  Rect,
};

enum class Unit {
  Fit,
  Grow,

  Exact,
};

struct ScreenUnit {
  Unit unit;
  union ScreenUnion {
    float px;
  } value;
};

struct Rect {
  ScreenUnit x, y, w, h;
};

struct DrawCmd {
  CmdTag tag;
  union DrawUnion {
    Rect rect;
  } value;
};

template <typename R> class Renderer {
public:
  void process(std::span<const DrawCmd> cmds) {
    static_cast<R *>(this)->process(cmds);
  }
};

class RendererCtx {
  std::vector<DrawCmd> draw_cmds;

public:
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
  };

  // Measure layout pass: calculate the widget's (and optionally the children's)
  // sizes. (Edits the public `layout`)
  virtual void measure() = 0;
  // Arrange layout pass: calculate the widget's (and optionally the children's)
  // positions. (Edits the public `layout`)
  virtual void arrange() = 0;

  virtual void draw(RendererCtx &ctx) = 0;

  virtual ~Widget() = default;
};

class PapaWidget : public Widget {
public:
  std::vector<std::unique_ptr<Widget>> children;
};

ScreenUnit px(float px);
