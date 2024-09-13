#ifndef YOUR_SOLUTION_SRC_SPIN_SOL_H_
#define YOUR_SOLUTION_SRC_SPIN_SOL_H_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include <vector>

using ArrayMsg = std_msgs::msg::Float64MultiArray;

class SpinSolution : public rclcpp::Node {
 public:
  SpinSolution();
 private:
  void position_callback(const ArrayMsg::SharedPtr msg);
  void velocity_callback(const ArrayMsg::SharedPtr msg);
  void timer_callback();

  rclcpp::Subscription<ArrayMsg>::SharedPtr subscription_pos_;
  rclcpp::Subscription<ArrayMsg>::SharedPtr subscription_vel_;
  rclcpp::Publisher<ArrayMsg>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::vector<double> prev_position_;
  std::vector<double> prev_velocity_;
  rclcpp::Time last_update_;
};

#endif //YOUR_SOLUTION_SRC_SPIN_SOL_H_
