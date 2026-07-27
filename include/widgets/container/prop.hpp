#pragma once
#include "widget.hpp"

#include <memory>

class Container;

enum class ContainerAxis {
    Row,
    Column,
};

enum class ContainerAlign {
    Start,
    Center,
    End,
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

struct MainAlignProp {
    ContainerAlign value;
    void apply(Container& c);
};

struct CrossAlignProp {
    ContainerAlign value;
    void apply(Container& c);
};

inline PadProp pad(float v) {
    return {v};
}
inline WidthProp w(ScreenUnit v) {
    return {v};
}
inline HeightProp h(ScreenUnit v) {
    return {v};
}
inline AxisProp axis(ContainerAxis v) {
    return {v};
}
inline GapProp gap(float v) {
    return {v};
}
inline MainAlignProp main_align(ContainerAlign align) {
    return {align};
}
inline CrossAlignProp cross_align(ContainerAlign align) {
    return {align};
}

template <typename T>
    requires std::derived_from<T, Widget>
ChildProp child(T&& w) {
    return {std::make_unique<T>(std::forward<T>(w))};
}
