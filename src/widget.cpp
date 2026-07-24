#include "widget.hpp"

ScreenUnit px(float px) {
  return ScreenUnit{.unit = Unit::Exact, .value = {.px = px}};
}
