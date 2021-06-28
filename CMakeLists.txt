cmake_minimum_required(VERSION 3.0.2)
project(ros_unity_com)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  std_msgs
  sensor_msgs
  roslaunch
  image_transport
)
roslaunch_add_file_check(launch)

################################################
## Declare ROS messages, services and actions ##
################################################

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################


###################################
## catkin specific configuration ##
###################################


###########
## Build ##
###########


include_directories(
  ${catkin_INCLUDE_DIRS}
)


add_executable(publisher src/position_publisher.cpp)
target_link_libraries(publisher ${catkin_LIBRARIES})

#############
## Install ##
#############


#############
## Testing ##
#############
