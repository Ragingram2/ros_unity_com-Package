#include <ros/ros.h>
#include <ros/console.h>

#include <std_msgs/String.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/PointCloud.h>
#include <geometry_msgs/Pose.h>
#include <image_transport/image_transport.h>


#include <string>

bool processing = false;
bool gotColor = false;
bool gotDepth = false;
bool gotOdom = false;

ros::Publisher pointcloudPub;
sensor_msgs::PointCloud cloud_msg;
geometry_msgs::Point32 cameraPos;
geometry_msgs::Quaternion cameraDir;

std::vector<geometry_msgs::Point32> calculateDepthPoints(sensor_msgs::ImageConstPtr depthMsg)
{
  std::vector<geometry_msgs::Point32> points;
  auto count = depthMsg->data.size();
  for(int i=0;i<count;i++)
  {
    geometry_msgs::Point32 point;
    point.x = cameraPos.x - ((depthMsg->width/2)+i%depthMsg->width);
    point.y = cameraPos.y - (depthMsg->height/2);
    point.y += i%depthMsg->width == 0 && i > 0?1:0;
    point.z = cameraPos.z + (depthMsg->data[i]*5);
    points.push_back(point);
  }
  return points;
}

sensor_msgs::ChannelFloat32 calculateColor(sensor_msgs::ImageConstPtr colorMsg)
{
  sensor_msgs::ChannelFloat32 color;
  color.name = "color";
  std::vector<float> colorData;
  auto count = colorMsg->data.size();
  for(int i =0;i<count;i++)
  {
    auto num = colorMsg->data[i];
    colorData.push_back(num);
  }
  return color;
}

void publishPointCloud()
{
  processing = false;
  gotColor = false;
  gotDepth = false;
  gotOdom = false;
  pointcloudPub.publish(cloud_msg);
}

void color_frame_recived(const sensor_msgs::ImageConstPtr& msg)
{
  gotColor = true;
  std::cout << "I recieved color frame"<< std::endl;
  cloud_msg.channels.push_back(calculateColor(msg));
}

void depth_frame_recived(const sensor_msgs::ImageConstPtr& msg)
{
  gotDepth = true;
  std::cout << "I recieved depth frame"<< std::endl;
  while(!gotOdom){}
  cloud_msg.points = calculateDepthPoints(msg);
}

void odometry_recieved(const geometry_msgs::Pose::ConstPtr& msg)
{
  gotOdom = true;
  std::cout << "I recieved camera odometry"<< std::endl;
}

std::string color_path;
std::string depth_path;

int main(int argc, char **argv)
{

  ros::init(argc, argv, "pointcloud_vizualizer");
  ros::NodeHandle n("~");
  image_transport::ImageTransport it(n);
  std::cout << "Node Started" << std::endl;
  n.getParam("color_path",color_path);
  n.getParam("depth_path",depth_path);

  pointcloudPub = n.advertise<sensor_msgs::PointCloud>("/pointcloud0",1);

  image_transport::Subscriber colorSub = it.subscribe(std::string("/"+color_path+"/image_raw").c_str(), 1000, color_frame_recived);
  image_transport::Subscriber depthSub = it.subscribe(std::string("/"+depth_path + "/image_raw").c_str(), 1000, depth_frame_recived);
  ros::Subscriber cameraOdomSub = n.subscribe("/camera_odometry",1,odometry_recieved);

  ros::spin();
  return 0;
}
