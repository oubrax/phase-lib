#include "widgets/container/container.hpp"
#include "widgets/container/prop.hpp"

int main() {
  auto ui = Container(
    pad(15.f),
    gap(10.f),
    child(Container(
     w(px(80.)),
     h(px(50.))
    )),
    child(Container(
     w(px(40.)),
     h(px(30.))
    ))
  );

  ui.measure();
  ui.arrange();

  return 0;
}
