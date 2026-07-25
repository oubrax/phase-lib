#include "widget.hpp"

ScreenUnit px(float px) {
  return ScreenUnit{.unit = Unit::Exact, .value = {.px = px}};
}

ScreenUnit grow() { return ScreenUnit{.unit = Unit::Grow, .value = {.px = 0}}; }
ScreenUnit pct(float pct) {
    return ScreenUnit {.unit = Unit::Pct, .value = {.pct = pct / 100}};
}
