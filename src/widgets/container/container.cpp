#include "widgets/container/container.hpp"
#include "widgets/container/prop.hpp"
#include <algorithm>
#include <cstdio>
#include <iostream>

Container::Container()
    : style{ContainerAxis::Row, 0, 0, 0, 0, {Unit::Fit, {0}},
            {Unit::Fit, {0}},   0} {}

float Container::width_additions() {
  if (along_axis(ContainerAxis::Row) && children.size() > 0) {
    return style.pl + style.pr + (children.size() - 1) * style.gap;
  } else {
    return style.pl + style.pr;
  }
}

float Container::height_additions() {
  if (along_axis(ContainerAxis::Column) && children.size() > 0) {
    return style.pt + style.pb + (children.size() - 1) * style.gap;
  } else {
    return style.pt + style.pb;
  }
}

void Container::adjust_fit(Widget &child) {
  if (along_axis(ContainerAxis::Row)) {
    layout.measured_w += child.layout.measured_w;
    layout.measured_h = std::max(child.layout.measured_h + height_additions(),
                                 layout.measured_h);
  } else {
    layout.measured_h += child.layout.measured_h;
    layout.measured_w = std::max(child.layout.measured_w + width_additions(),
                                 layout.measured_w);
  }
}

void Container::finalize_width() {
  switch (style.w.unit) {
  case Unit::Fit: {
    layout.w = layout.measured_w;
    break;
  };
  case Unit::Grow: {
    layout.w = layout.growth_w;
    break;
  }
  case Unit::Exact: {
    layout.w = style.w.value.px;
    break;
  };
  }
}

void Container::finalize_height() {
  switch (style.h.unit) {
  case Unit::Fit: {
    layout.h = layout.measured_h;
    break;
  };
  case Unit::Exact: {
    layout.h = style.h.value.px;
    break;
  };
  case Unit::Grow: {
    layout.h = layout.growth_h;
    break;
  };
  }
}

void Container::finalize_size() {
  finalize_width();
  finalize_height();
}

float Container::calc_offset(Widget &child) {
  if (along_axis(ContainerAxis::Row)) {
    return child.layout.w + style.gap;
  } else {
    return child.layout.h + style.gap;
  }
}

void Container::offset(float offset, Widget &child) {
  if (along_axis(ContainerAxis::Row)) {
    child.layout.x += offset;
  } else {
    child.layout.y += offset;
  }
}

void Container::record_growth(Widget &child) {
  if (child.layout.x_growth) {
    x_growers++;
  }
  if (child.layout.y_growth) {
    y_growers++;
  }
}

void Container::draw(RendererCtx &ctx) {
  ctx.push_rect(
      Rect{.x = layout.x, .y = layout.y, .w = layout.w, .h = layout.h});
}

void Container::measure() {
  layout.measured_w = width_additions();
  layout.measured_h = height_additions();

  for (auto &c : children) {
    Widget &child = *c;
    child.measure();

    adjust_fit(child);
    record_growth(child);
  }

  if (leaf_node()) {
    layout.measured_w += style.w.value.px;
    layout.measured_h += style.h.value.px;
  }
}

void Container::grow(float free_width, float free_height, Widget &child) {
  if (child.layout.x_growth) {
    if (along_axis(ContainerAxis::Row)) {
      child.layout.growth_w = free_width / x_growers;
      std::cout << "set it to: " << child.layout.measured_w << ": "
                << x_growers;
    } else {
      child.layout.growth_w = free_width;
    }
  } else {
    if (along_axis(ContainerAxis::Row)) {
      child.layout.growth_h = free_height / y_growers;
    } else {
      child.layout.growth_h = free_height;
    }
  }
}

std::tuple<float, float> Container::free_space() {
    float free_width = layout.w - width_additions();
    float free_height = layout.h - height_additions();
    for (auto &c : children) {
      if (along_axis(ContainerAxis::Row)) {
        free_width -= c->layout.w;
      } else {
        free_width -= c->layout.h;
      }
    }

    return {free_width, free_height};
}

void Container::arrange() {
  finalize_size();

  auto [free_width, free_height] = free_space();

  float axis_offset = initial_offset();

  for (auto &c : children) {
    Widget &child = *c;
    apply_padding(child);
    offset(axis_offset, child);
    grow(free_width, free_height, child);
    child.arrange();

    axis_offset += calc_offset(child); // TODO: cross offset (for cross-axis
                                       // alignment) would be added here too
  }
}
