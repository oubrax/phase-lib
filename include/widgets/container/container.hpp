#pragma once
#include "prop.hpp"

struct ContainerStyle {
  ContainerAxis axis;

  float pl, pr, pt, pb;
  ScreenUnit w, h;

  float gap;
};

class Container : public PapaWidget {
private:
  ContainerStyle style;

  bool along_axis(ContainerAxis axis) { return style.axis == axis; }

  float width_additions();
  float height_additions();

  void adjust_fit(Widget &child);

  bool width_fit() { return style.w.unit == Unit::Fit; }
  bool height_fit() { return style.h.unit == Unit::Fit; }

  bool leaf_node() { return !children.size(); }

  void finalize_width();
  void finalize_height();
  void finalize_size();

  void apply_padding(Widget &child) {
    child.layout.x += style.pl;
    child.layout.y += style.pt;
  }

  float initial_offset() {
    return 0; // TODO: add alignment handling here
  }

  float calc_offset(Widget &child);
  void offset(float offset, Widget &child);

public:
  Container();

  Container &w(ScreenUnit unit) {
    style.w = unit;
    return *this;
  }
  Container &h(ScreenUnit unit) {
    style.h = unit;
    return *this;
  }

  Container &axis(ContainerAxis a) {
    style.axis = a;
    return *this;
  }
  Container &gap(float g) {
    style.gap = g;
    return *this;
  }

  template <typename... Props> Container(Props &&...props) : Container() {
    (std::forward<Props>(props).apply(*this), ...);
  }

  Container &pad(float px) {
    style.pb = px;
    style.pt = px;
    style.pl = px;
    style.pr = px;
    return *this;
  }

  Container &child(std::unique_ptr<Widget> w) {
    children.push_back(std::move(w));
    return *this;
  }

  void draw(RendererCtx &ctx) override;
  void measure() override;
  void arrange() override;
};
