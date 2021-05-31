#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Image.h"

#include <string>

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void handleFrame(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}

std::string color_path;
std::string depth_path;

int main(int argc, char **argv)
{

  ros::init(argc, argv, "pointcloud_vizualizer");
  ros::NodeHandle n;
  n.setParam("color_path",color_path);
  n.setParam("depth_path",depth_path);

  ros::Subscriber colorSub = n.subscribe(std::string(color_path + "/image_raw"), 1000, handleFrame)
  ros::Subscriber depthSub = n.subscribe(std::string(dpethPath + "/image_raw"), 1000, handleFrame);
  ros::spin();

  return 0;
}
