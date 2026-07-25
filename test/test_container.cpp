#include "widgets/container/prop.hpp"
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "widgets/container/container.hpp"

TEST_CASE("measure_pass container exact sizing") {
    Container c = Container(
        w(px(50.)),
        h(px(50.))
    );

    c.measure();

    CHECK(c.layout.measured_w == 50.);
    CHECK(c.layout.measured_h == 50.);
}

TEST_CASE("measure_pass container fit sizing") {
    Container c = Container(
        child(
            Container(
                w(px(17.)),
                h(px(28.))
            )
        )
    );

    c.measure();

    CHECK(c.layout.measured_w == 17.);
    CHECK(c.layout.measured_h == 28.);
}

TEST_CASE("arrange_pass padded") {
    Container c = Container(
        pad(5.),
        child(
            Container(w(px(20.)), h(px(20.)))
        )
    );

    c.measure();
    c.arrange();

    CHECK(c.layout.w == 30.);
    CHECK(c.layout.h == 30.);


    CHECK(c.children[0]->layout.x == 5.);
    CHECK(c.children[0]->layout.y == 5.);

    CHECK(c.children[0]->layout.w == 20.);
    CHECK(c.children[0]->layout.h == 20.);
}

TEST_CASE("nested fit") {

    Container c = Container(
        child(Container(
            Container(
                Container(
                    w(px(50.)),
                    h(px(50.))
                )
            )
        ))
    );

    c.measure();
    c.arrange();

    CHECK(c.layout.w == 50.);
    CHECK(c.layout.h == 50.);
}

TEST_CASE("nested grow") {
    Container c = Container(
        w(px(100.)),
        h(px(100.)),
        child(
            Container(
                w(grow()),
                child(
                    Container(w(grow()))
                )
            )
        )
    );
    c.measure();
    c.arrange();
    CHECK(c.children[0]->layout.w == 100.);

    Container *c2 = dynamic_cast<Container* >(c.children[0].get());

    CHECK(c2->children[0]->layout.w == 100.);
}
