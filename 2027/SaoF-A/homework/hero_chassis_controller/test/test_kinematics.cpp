#include <gtest/gtest.h>
#include <hero_chassis_controller/kinematics.h>
namespace hc = hero_chassis_controller;
int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
TEST(Kinematics, ZeroAndIndependentAxes) {
  EXPECT_EQ(hc::inverse({-0.2, 0, 0}, 0.1, 0.4), (std::array<double, 4>{{-2, -2, -2, -2}}));
  for (double speed : hc::inverse({}, 0.1, 0.4)) EXPECT_DOUBLE_EQ(speed, 0);
  EXPECT_EQ(hc::inverse({0.2, 0, 0}, 0.1, 0.4), (std::array<double, 4>{{2, 2, 2, 2}}));
  EXPECT_EQ(hc::inverse({0, 0.2, 0}, 0.1, 0.4), (std::array<double, 4>{{-2, 2, 2, -2}}));
  EXPECT_EQ(hc::inverse({0, 0, 0.5}, 0.1, 0.4), (std::array<double, 4>{{-2, 2, -2, 2}}));
}
TEST(Kinematics, RoundTripOnMixedCommandsAndGeometry) {
  for (double radius : {0.07625, 0.13})
    for (double lever : {0.4, 0.57})
      for (const auto& v : {hc::Velocity{0.2, -0.1, 0.7}, hc::Velocity{-0.4, 0.3, -0.2}}) {
        const auto result = hc::forward(hc::inverse(v, radius, lever), radius, lever);
        EXPECT_NEAR(result.x, v.x, 1e-12);
        EXPECT_NEAR(result.y, v.y, 1e-12);
        EXPECT_NEAR(result.yaw, v.yaw, 1e-12);
      }
}
TEST(Odometry, UnequalMeasuredWheelsGiveExpectedBodyVelocity) {
  const auto measured = hc::forward({{1, 2, 3, 4}}, 0.1, 0.5);
  EXPECT_NEAR(measured.x, 0.25, 1e-12);
  EXPECT_NEAR(measured.y, 0.0, 1e-12);
  EXPECT_NEAR(measured.yaw, 0.1, 1e-12);
}
TEST(Limits, EffortAndAccelerationBothDirections) {
  EXPECT_DOUBLE_EQ(hc::clamp(9, 5), 5);
  EXPECT_DOUBLE_EQ(hc::clamp(-9, 5), -5);
  EXPECT_DOUBLE_EQ(hc::approach(0, 1, 0.01), 0.01);
  EXPECT_DOUBLE_EQ(hc::approach(0, -1, 0.01), -0.01);
  EXPECT_DOUBLE_EQ(hc::approach(0, 0.005, 0.01), 0.005);
}
TEST(Watchdog, ExpiryAndClockReset) {
  EXPECT_TRUE(hc::commandFresh(2.2, 2, 0.3));
  EXPECT_FALSE(hc::commandFresh(2.31, 2, 0.3));
  EXPECT_FALSE(hc::commandFresh(0.5, 2, 0.3));
  EXPECT_FALSE(hc::commandFresh(0, 0, 0.3));
}
