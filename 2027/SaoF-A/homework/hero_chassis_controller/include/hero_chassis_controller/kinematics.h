#ifndef HERO_CHASSIS_CONTROLLER_KINEMATICS_H_
#define HERO_CHASSIS_CONTROLLER_KINEMATICS_H_
#include <algorithm>
#include <array>
#include <cmath>
namespace hero_chassis_controller {
struct Velocity {
  double x{0.0};
  double y{0.0};
  double yaw{0.0};
};
inline double clamp(double value, double limit) { return std::max(-limit, std::min(value, limit)); }
inline double approach(double current, double target, double max_change) {
  return current + clamp(target - current, max_change);
}
inline bool commandFresh(double now, double stamp, double timeout) {
  return stamp > 0.0 && now >= stamp && now - stamp <= timeout;
}
// Order: front left, front right, back left, back right. x forward, y left, yaw CCW.
inline std::array<double, 4> inverse(const Velocity& v, double radius, double lever) {
  return {{(v.x - v.y - lever * v.yaw) / radius, (v.x + v.y + lever * v.yaw) / radius,
           (v.x + v.y - lever * v.yaw) / radius, (v.x - v.y + lever * v.yaw) / radius}};
}
inline Velocity forward(const std::array<double, 4>& w, double radius, double lever) {
  return {(w[0] + w[1] + w[2] + w[3]) * radius / 4.0, (-w[0] + w[1] + w[2] - w[3]) * radius / 4.0,
          (-w[0] + w[1] - w[2] + w[3]) * radius / (4.0 * lever)};
}
}  // namespace hero_chassis_controller
#endif  // HERO_CHASSIS_CONTROLLER_KINEMATICS_H_
