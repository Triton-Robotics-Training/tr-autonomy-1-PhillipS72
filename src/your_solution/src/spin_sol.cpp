#include "spin_sol.h"

#include <chrono>

using namespace std::chrono_literals;
using std::placeholders::_1;


int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SpinSolution>());
  rclcpp::shutdown();
  return 0;
}

//your code here

SpinSolution::SpinSolution() : Node("spinsolution"),
  prev_position_(2, 0.0),
  prev_velocity_(2, 0.0),
  last_position_update_time_(this->get_clock()->now())

{
  subscription_pos_ = this->create_subscription<ArrayMsg>("/measuredpos", 10, std::bind(&SpinSolution::position_callback, this, std::placeholders::_1));
  subscription_vel_ = this->create_subscription<ArrayMsg>("/measuredvel", 10, std::bind(&SpinSolution::velocity_callback, this, std::placeholders::_1));

  publisher_ = this->create_publisher<ArrayMsg>("/predictedpos", 10);

  timer_ = this->create_wall_timer(10ms, std::bind(&SpinSolution::timer_callback, this));
}

void SpinSolution::position_callback(const ArrayMsg::SharedPtr msg) {

  prev_position_[0] = msg->data[0];
  prev_position_[1] = msg->data[1];
  last_position_update_time_ = this->get_clock()->now();

}

void SpinSolution::velocity_callback(const ArrayMsg::SharedPtr msg) {

  prev_velocity_[0] = msg->data[0];
  prev_velocity_[1] = msg->data[1];

}


void SpinSolution::timer_callback() {

  auto msg = ArrayMsg();
  auto now = this->get_clock()->now();
  auto time_diff = (now - last_position_update_time_).seconds();

  msg.data.resize(2);
  msg.data[0] = prev_position_[0] + time_diff * prev_velocity_[0];
  msg.data[1] = prev_position_[1] + time_diff * prev_velocity_[1];

  publisher_->publish(msg);
}
