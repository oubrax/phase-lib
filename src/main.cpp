#include "container.hpp"
#include "widget.hpp"
#include <memory>


int main() {
  Container c;
  c.w(px(50.)).h(px(50.));
  c.child(std::make_unique<Container>());
  c.pad(15.);
  c.measure();
  c.arrange();

  return 0;
}
