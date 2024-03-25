#include "catch.hpp"
#include "../Vector.hpp"

void print(Vec a){
    std::cout << a << "\n";
}

/*
 * Very basic set of tests
 */

TEST_CASE( "Vec initialization", "[Vec]" ) {
    Vec a(1.0, 2.0, 3.0);

    REQUIRE(a.x() == 1.0);
    REQUIRE(a.y() == 2.0);
    REQUIRE(a.z() == 3.0);

    REQUIRE(a == Vec(1.0, 2.0, 3.0));
    REQUIRE(a != Vec(2.0, 2.0, 3.0));
}

TEST_CASE( "Unary minus", "[Vec]" ) {
    Vec a(1, 2, 3);
    Vec b(-1, -2, -3);

    REQUIRE(-a == b);

    // a should not have changed
    REQUIRE(a == Vec(1, 2, 3));
}

TEST_CASE( "Addition and Subtraction", "[Vec]" ) {
    Vec a(1, 2, 3);
    Vec b(-1, -2, -3);

    REQUIRE(a + b == Vec(0, 0, 0));
    REQUIRE(a - b == Vec(2, 4, 6));

    // Test a did not change
    REQUIRE(a == Vec(1, 2, 3));

}

TEST_CASE("Const multiplication", "[Vec]"){
    Vec a(1, 2, 3);
    Vec b(2, 4, 6);

    REQUIRE(a*2 == b);
    REQUIRE(2*a == b);
}

TEST_CASE("Compound operations", "[Vec]"){
    Vec a(1, 2, 3);

    a *= 2;
    REQUIRE(a ==  Vec(2, 4, 6));

    a += Vec(1, 1, 1);
    REQUIRE(a ==  Vec(3, 5, 7));

    a -= Vec(1, 1, 1);
    REQUIRE(a ==  Vec(2, 4, 6));

    a /=  2;
    REQUIRE(a ==  Vec(1, 2, 3));
}

TEST_CASE("Norms", "[Vec]"){
    Vec a(1, 2, 3);

    REQUIRE(a.norm() == Approx(sqrt(14)));
    REQUIRE(a.norm2() == 14);
    REQUIRE(a.norm3() == 14*sqrt(14));
}

TEST_CASE("Zero vector norms shoud be 0", "[Vec]"){
    Vec a(0, 0, 0);

    REQUIRE(a.norm() == Approx(0.));
    REQUIRE(a.norm2() == Approx(0.));
    REQUIRE(a.norm3() == Approx(0.));
}


TEST_CASE("Streaming vectors in the form of \"(1, 2.0, 3.0)\" ", "[Vec]"){
    Vec a(1, 2.0, 3.14);

    std::stringstream ss;
    ss << a;

    // note: 1 and 2.0 are both double but the formatting of the output is to not show the decimal point
    REQUIRE(ss.str() == "(1, 2, 3.14)");
}
