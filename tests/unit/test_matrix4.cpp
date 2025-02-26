#define CATCH_CONFIG_MAIN
#include "Logger.hpp"
#include "catch2/catch_all.hpp"
#include "math/Matrix4.hpp"

TEST_CASE("Matrix4: Should initialize as Identity Matrix",
          "[Matrix4][Identity]") {
  Matrix4 identity;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (i == j) {
        REQUIRE(identity[i][j] ==
                Catch::Approx(1.0f)); // Use overloaded operator[]
      } else {
        REQUIRE(identity[i][j] == Catch::Approx(0.0f));
      }
    }
  }
}

TEST_CASE("Matrix4: Should correctly apply translation",
          "[Matrix4][Translation]") {
  Vector3 translation(3.0f, 4.0f, 5.0f);
  Matrix4 translationMat = Matrix4::from_translation(translation);

  REQUIRE(translationMat[3][0] == Catch::Approx(3.0f));
  REQUIRE(translationMat[3][1] == Catch::Approx(4.0f));
  REQUIRE(translationMat[3][2] == Catch::Approx(5.0f));
}

TEST_CASE("Matrix4: Should correctly apply scaling", "[Matrix4][Scaling]") {
  Vector3 scale(2.0f, 3.0f, 4.0f);
  Matrix4 scalingMat = Matrix4::from_scaling(scale);

  REQUIRE(scalingMat[0][0] == Catch::Approx(2.0f));
  REQUIRE(scalingMat[1][1] == Catch::Approx(3.0f));
  REQUIRE(scalingMat[2][2] == Catch::Approx(4.0f));
}

TEST_CASE("Matrix4: Should rotate vector 90 degrees around Y-axis",
          "[Matrix4][Rotation]") {
  Vector3 axis(0.0f, 1.0f, 0.0f);
  float angle = 90.0f;
  Matrix4 rotationMat = Matrix4::from_rotation(axis, angle);

  LOG(DEBUG, "RotationMatrix: \n%s", rotationMat.toString());

  Vector3 v(1.0f, 0.0f, 0.0f);
  Vector3 result = rotationMat.transform(v);

  LOG(DEBUG, "resultVector: %s", result.toString());

  REQUIRE(result.x == Catch::Approx(0.0f).margin(0.0001));
  REQUIRE(result.y == Catch::Approx(0.0f).margin(0.0001));
  REQUIRE(result.z == Catch::Approx(-1.0f).margin(0.0001));
}

TEST_CASE("Matrix4: Should correctly multiply scaling and translation matrices",
          "[Matrix4][Multiplication]") {
  Matrix4 scaleMat = Matrix4::from_scaling(Vector3(2.0f, 2.0f, 2.0f));
  Matrix4 translationMat = Matrix4::from_translation(Vector3(1.0f, 2.0f, 3.0f));
  Matrix4 result = translationMat * scaleMat;

  REQUIRE(result[3][0] == Catch::Approx(2.0f));
  REQUIRE(result[3][1] == Catch::Approx(4.0f));
  REQUIRE(result[3][2] == Catch::Approx(6.0f));
}

TEST_CASE("Matrix4: Should create a valid perspective projection matrix",
          "[Matrix4][Perspective]") {
  float fov = 90.0f;
  float aspect = 16.0f / 9.0f;
  float near = 0.1f;
  float far = 100.0f;
  Matrix4 perspectiveMat = Matrix4::perspective(fov, aspect, near, far);

  REQUIRE(perspectiveMat[0][0] ==
          Catch::Approx(1.0f / (aspect * tan(fov * 0.5f * M_PI / 180.0f))));
  REQUIRE(perspectiveMat[1][1] ==
          Catch::Approx(1.0f / tan(fov * 0.5f * M_PI / 180.0f)));
  REQUIRE(perspectiveMat[2][2] == Catch::Approx(-(far + near) / (far - near)));
  REQUIRE(perspectiveMat[2][3] == Catch::Approx(-1.0f));
  REQUIRE(perspectiveMat[3][2] ==
          Catch::Approx(-(2.0f * far * near) / (far - near)));
}

TEST_CASE("Matrix4: Should detect equality and inequality between matrices",
          "[Matrix4][Equality]") {
  Matrix4 matA;
  Matrix4 matB;

  REQUIRE(matA == matB);

  Matrix4 matC = Matrix4::from_translation(Vector3(1.0f, 0.0f, 0.0f));
  REQUIRE(matA != matC);
}

TEST_CASE("Matrix4: Should initialize using the initializer list constructor",
          "[Matrix4][Constructor]") {
  Matrix4 customMat =
      Matrix4({1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f,
               11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f});

  REQUIRE(customMat[0][0] == Catch::Approx(1.0f));
  REQUIRE(customMat[0][1] == Catch::Approx(2.0f));
  REQUIRE(customMat[1][0] == Catch::Approx(5.0f));
  REQUIRE(customMat[3][3] == Catch::Approx(16.0f));
}
