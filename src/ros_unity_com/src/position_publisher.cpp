#include "ros/console.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Vector3.h"
#include "std_msgs/UInt8MultiArray.h"

#include <sstream>


int count = 0;
ros::Publisher pub;
ros::Publisher imagePub;

ros::Subscriber sub;
ros::Subscriber imageSub;

void sendNewPosition(geometry_msgs::Vector3::Ptr recived)
{
	geometry_msgs::Vector3 msg;
	count++;
	srand(count);
	msg.x = rand()%10;
	msg.y = rand()%10;
	msg.z = rand()%10;
	//ROS_INFO("Sending: X:%f, Y:%f, Z:%f",msg.x,msg.y,msg.z);
	pub.publish(msg);
}

void processBuffer(std_msgs::UInt8MultiArray::Ptr image)
{
	ROS_INFO("Buffer Size: %lu", sizeof(image->data));
	imagePub.publish(image);
}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "position_publisher");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  pub = n.advertise<geometry_msgs::Vector3>("get_position", 1000);
  sub = n.subscribe("send_position", 1000, sendNewPosition);

  geometry_msgs::Vector3::Ptr message;
  sendNewPosition(message);

  ros::spin();
  return 0;
}
