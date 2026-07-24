#pragma once
#include "widget.hpp"
#include <memory>

class Container;

enum class ContainerAxis {
  Row,
  Column,
};

struct PadProp {
  float value;
  void apply(Container& c);
};

struct WidthProp {
  ScreenUnit value;
  void apply(Container& c);
};

struct HeightProp {
  ScreenUnit value;
  void apply(Container& c);
};

struct AxisProp {
  ContainerAxis value;
  void apply(Container& c);
};

struct GapProp {
  float value;
  void apply(Container& c);
};

struct ChildProp {
  std::unique_ptr<Widget> value;
  void apply(Container& c);
};


inline PadProp pad(float v) { return {v}; }
inline WidthProp w(ScreenUnit v) { return {v}; }
inline HeightProp h(ScreenUnit v) { return {v}; }
inline AxisProp axis(ContainerAxis v) { return {v}; }
inline GapProp gap(float v) { return {v}; }

template <typename T>
ChildProp child(T&& w) {
  return {std::make_unique<T>(std::forward<T>(w))};
}
