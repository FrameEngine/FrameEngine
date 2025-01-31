#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#define CATCH_CONFIG_MAIN
#include "math/Vector3.hpp"

TEST_CASE("Vector3 addition works correctly", "[Vector3]") {
  Vector3 a(1.0f, 2.0f, 3.0f);
  Vector3 b(4.0f, 5.0f, 6.0f);
  Vector3 result = a + b;

  REQUIRE(result.x == Catch::Approx(5.0f));
  REQUIRE(result.y == Catch::Approx(7.0f));
  REQUIRE(result.z == Catch::Approx(9.0f));
}

TEST_CASE("Vector3 subtraction works correctly", "[Vector3]") {
  Vector3 a(5.0f, 7.0f, 9.0f);
  Vector3 b(1.0f, 2.0f, 3.0f);
  Vector3 result = a - b;

  REQUIRE(result.x == Catch::Approx(4.0f));
  REQUIRE(result.y == Catch::Approx(5.0f));
  REQUIRE(result.z == Catch::Approx(6.0f));
}

TEST_CASE("Vector3 scalar multiplication works correctly", "[Vector3]") {
  Vector3 a(2.0f, 3.0f, 4.0f);
  float scalar = 2.0f;
  Vector3 result = a * scalar;

  REQUIRE(result.x == Catch::Approx(4.0f));
  REQUIRE(result.y == Catch::Approx(6.0f));
  REQUIRE(result.z == Catch::Approx(8.0f));
}

TEST_CASE("Vector3 dot product works correctly", "[Vector3]") {
  Vector3 a(1.0f, 2.0f, 3.0f);
  Vector3 b(4.0f, 5.0f, 6.0f);

  float dotProduct = a.dot(b);
  REQUIRE(dotProduct == Catch::Approx(32.0f)); // 1*4 + 2*5 + 3*6 = 32
}

TEST_CASE("Vector3 cross product works correctly", "[Vector3]") {
  Vector3 a(1.0f, 0.0f, 0.0f);
  Vector3 b(0.0f, 1.0f, 0.0f);
  Vector3 result = a.cross(b);

  REQUIRE(result.x == Catch::Approx(0.0f));
  REQUIRE(result.y == Catch::Approx(0.0f));
  REQUIRE(result.z == Catch::Approx(1.0f)); // It's perpendicular
}

TEST_CASE("Vector3 magnitude (length) works correctly", "[Vector3]") {
  Vector3 a(3.0f, 4.0f, 0.0f);
  float length = a.magnitude();

  REQUIRE(length == Catch::Approx(5.0f));
}

TEST_CASE("Vector3 normalization works correctly", "[Vector3]") {
  Vector3 a(3.0f, 4.0f, 0.0f);
  Vector3 normalized = a.normalized();

  REQUIRE(normalized.x == Catch::Approx(0.6f));
  REQUIRE(normalized.y == Catch::Approx(0.8f));
  REQUIRE(normalized.z == Catch::Approx(0.0f));
}

TEST_CASE("Vector3 normalization of zero vector returns zero vector",
          "[Vector3]") {
  Vector3 zero(0.0f, 0.0f, 0.0f);
  Vector3 normalized = zero.normalized();

  REQUIRE(normalized.x == Catch::Approx(0.0f));
  REQUIRE(normalized.y == Catch::Approx(0.0f));
  REQUIRE(normalized.z == Catch::Approx(0.0f));
}
