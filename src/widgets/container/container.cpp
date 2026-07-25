#include "widgets/container/container.hpp"
#include "widgets/container/prop.hpp"
#include <algorithm>
#include <cstdio>

Container::Container()
    : style{ContainerAxis::Row, ContainerAlign::Start, ContainerAlign::Start, 0, 0, 0, 0, {Unit::Fit, {0}},
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
    if (style.w.unit == Unit::Fit || style.w.unit == Unit::Grow)
      layout.measured_w += child.layout.measured_w;

    if (style.h.unit == Unit::Fit || style.h.unit == Unit::Grow)
      layout.measured_h =
          std::max(child.layout.measured_h + height_additions(),
                   layout.measured_h);
  } else {
    if (style.h.unit == Unit::Fit || style.h.unit == Unit::Grow)
      layout.measured_h += child.layout.measured_h;

    if (style.w.unit == Unit::Fit || style.w.unit == Unit::Grow)
      layout.measured_w =
          std::max(child.layout.measured_w + width_additions(),
                   layout.measured_w);
  }
}

void Container::finalize_width() {
  switch (style.w.unit) {
  case Unit::Fit:
  case Unit::Pct: {
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
  case Unit::Fit:
  case Unit::Pct: {
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
static float compute_base(Unit unit, float value_px, float measured, float pad_a, float pad_b) {
  if (unit == Unit::Exact)
    return value_px - pad_a - pad_b;

  if (unit == Unit::Pct && measured > 0)
    return measured - pad_a - pad_b;

  return 0.f;
}

static void apply_pct(float base, float pct, float &out) {
  if (base > 0 && pct > 0)
    out = pct * base;
}

void Container::pct_check(Widget &child) {
  float base_w = compute_base(style.w.unit, style.w.value.px,
                              layout.measured_w, style.pl, style.pr);

  float base_h = compute_base(style.h.unit, style.h.value.px,
                              layout.measured_h, style.pt, style.pb);

  apply_pct(base_w, child.layout.x_pct, child.layout.measured_w);
  apply_pct(base_h, child.layout.y_pct, child.layout.measured_h);
}

void Container::resolve_grow_pct() {
  if (style.w.unit == Unit::Grow && layout.w > 0) {
    float content_w = layout.w - style.pl - style.pr;
    for (auto &c : children) {
      if (c->layout.x_pct)
        c->layout.measured_w = c->layout.x_pct * content_w;
    }
  }
  if (style.h.unit == Unit::Grow && layout.h > 0) {
    float content_h = layout.h - style.pt - style.pb;
    for (auto &c : children) {
      if (c->layout.y_pct)
        c->layout.measured_h = c->layout.y_pct * content_h;
    }
  }
}

static float compute_cross_space(float container_size,
                                 float pad_a, float pad_b,
                                 float child_size) {
  return container_size - pad_a - pad_b - child_size;
}

static float align_offset(ContainerAlign align, float space) {
  switch (align) {
    case ContainerAlign::Start:  return 0.f;
    case ContainerAlign::Center: return space / 2;
    case ContainerAlign::End:    return space;
  }
  return 0.f;
}

void Container::cross_offset(Widget &child) {
  float space = along_axis(ContainerAxis::Row)
    ? compute_cross_space(layout.h, style.pt, style.pb, child.layout.measured_h)
    : compute_cross_space(layout.w, style.pl, style.pr, child.layout.measured_w);

  float offset = align_offset(style.cross_align, space);

  if (along_axis(ContainerAxis::Row))
    child.layout.y += offset;
  else
    child.layout.x += offset;
}

void Container::draw(RendererCtx &ctx) {
  ctx.push_rect(
      Rect{.x = layout.x, .y = layout.y, .w = layout.w, .h = layout.h});
}

void Container::init_measured_width() {
  switch (style.w.unit) {
  case Unit::Fit:
  case Unit::Grow:
    layout.measured_w = width_additions();
    break;
  case Unit::Exact:
    layout.measured_w = style.w.value.px;
    break;
  case Unit::Pct:
    if (layout.measured_w == 0)
      layout.measured_w = width_additions();
    break;
  }
}

void Container::init_measured_height() {
  switch (style.h.unit) {
  case Unit::Fit:
  case Unit::Grow:
    layout.measured_h = height_additions();
    break;
  case Unit::Exact:
    layout.measured_h = style.h.value.px;
    break;
  case Unit::Pct:
    if (layout.measured_h == 0)
      layout.measured_h = height_additions();
    break;
  }
}

void Container::measure() {
  float ext_w = layout.measured_w;
  float ext_h = layout.measured_h;

  init_measured_width();
  init_measured_height();

  for (auto &c : children) {
    Widget &child = *c;
    pct_check(child);
    child.measure();
    adjust_fit(child);
    record_growth(child);
  }

  if (style.w.unit == Unit::Pct && ext_w > 0)
    layout.measured_w = ext_w;
  if (style.h.unit == Unit::Pct && ext_h > 0)
    layout.measured_h = ext_h;
}

void Container::grow(float free_width, float free_height, Widget &child) {
  if (child.layout.x_growth) {
    if (along_axis(ContainerAxis::Row)) {
      child.layout.growth_w = free_width / x_growers;
    } else {
      child.layout.growth_w = free_width;
    }
  }
  if (child.layout.y_growth) {
    if (along_axis(ContainerAxis::Column)) {
      child.layout.growth_h = free_height / y_growers;
    } else {
      child.layout.growth_h = free_height;
    }
  }
}

float Container::initial_offset() {
  if (along_axis(ContainerAxis::Row)) {
    if (x_growers > 0) return 0;

    float axis_size = layout.w - style.pl - style.pr;
    float children_size = 0;
    for (auto &c : children)
      children_size += c->layout.measured_w;
    if (children.size() > 1)
      children_size += (children.size() - 1) * style.gap;

    switch (style.main_align) {
    case ContainerAlign::Center:
      return (axis_size / 2) - (children_size / 2);
    case ContainerAlign::End:
      return axis_size - children_size;
    default:
      return 0;
    }
  } else {
    if (y_growers > 0) return 0;

    float axis_size = layout.h - style.pt - style.pb;
    float children_size = 0;
    for (auto &c : children)
      children_size += c->layout.measured_h;
    if (children.size() > 1)
      children_size += (children.size() - 1) * style.gap;

    switch (style.main_align) {
    case ContainerAlign::Center:
      return (axis_size / 2) - (children_size / 2);
    case ContainerAlign::End:
      return axis_size - children_size;
    default:
      return 0;
    }
  }
}

std::tuple<float, float> Container::free_space() {
  float free_width = layout.w - width_additions();
  float free_height = layout.h - height_additions();
  for (auto &c : children) {
    if (along_axis(ContainerAxis::Row)) {
      free_width -= c->layout.measured_w;
    } else {
      free_height -= c->layout.measured_h;
    }
  }

  return {free_width, free_height};
}

void Container::arrange() {
  finalize_size();
  resolve_grow_pct();

  auto [free_width, free_height] = free_space();

  float axis_offset = initial_offset();

  for (auto &c : children) {
    Widget &child = *c;
    apply_padding(child);

    offset(axis_offset, child);
    cross_offset(child);

    grow(free_width, free_height, child);

    child.arrange();

    axis_offset += calc_offset(child);
  }
}
