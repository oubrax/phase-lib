#include "widgets/container/prop.hpp"
#include "widgets/container/container.hpp"

void PadProp::apply(Container& c) { c.pad(value); }
void WidthProp::apply(Container& c) { c.w(value); }
void HeightProp::apply(Container& c) { c.h(value); }
void AxisProp::apply(Container& c) { c.axis(value); }
void GapProp::apply(Container& c) { c.gap(value); }
void ChildProp::apply(Container& c) { c.child(std::move(value)); }
