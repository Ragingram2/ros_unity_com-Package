#include "ros/console.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Vector3.h"
#include "std_msgs/UInt8MultiArray.h"

#include <sstream>


int count = 0;
int radius = 17;

ros::Publisher pub;
ros::Subscriber sub;

void sendNewPosition(geometry_msgs::Vector3::Ptr recived)
{
	geometry_msgs::Vector3 msg;
	count++;
	srand(count);
	msg.x = rand()%17;
	msg.y = rand()%17;
	msg.z = rand()%17;
	//ROS_INFO("Sending: X:%f, Y:%f, Z:%f",msg.x,msg.y,msg.z);
	pub.publish(msg);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "position_publisher");
  ros::NodeHandle n; //("~"); Never ever add this! This break everything!

  pub = n.advertise<geometry_msgs::Vector3>("get_position", 1000);
  sub = n.subscribe("send_position", 1000, sendNewPosition);

  geometry_msgs::Vector3::Ptr message;
  sendNewPosition(message);

  ros::spin();
  return 0;
}
