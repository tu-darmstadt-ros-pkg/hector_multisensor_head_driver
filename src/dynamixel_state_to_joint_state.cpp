// This node subscribes the published states from the dynamixel motor node and pulishes them in joint_states

#include "sensor_msgs/JointState.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "dynamixel_msgs/JointState.h"

class DynamixelStateToJointState
{
    public:
    DynamixelStateToJointState();
    ros::NodeHandle n;
    ros::Publisher jointStatePub;
    sensor_msgs::JointState dynamixelJointStates;
         
    void updateJointStates(const dynamixel_msgs::JointState& msg);
    ros::Subscriber initSubscriber();
};

DynamixelStateToJointState::DynamixelStateToJointState()
{   // Initialize all values to 0
  
    dynamixelJointStates.name.resize(1);
    dynamixelJointStates.position.resize(1);
    dynamixelJointStates.velocity.resize(1);
    dynamixelJointStates.effort.resize(1);
    
    dynamixelJointStates.name[0] = "";
    dynamixelJointStates.position[0] = 0;
    dynamixelJointStates.velocity[0] = 0;
    dynamixelJointStates.effort[0] = 0;
    
    this->jointStatePub = this->n.advertise<sensor_msgs::JointState>("/joint_states",1);

}
void DynamixelStateToJointState::updateJointStates(const dynamixel_msgs::JointState& msg)
{  
    this->dynamixelJointStates.header.stamp = msg.header.stamp;
	this->dynamixelJointStates.name[0] = msg.name;
	this->dynamixelJointStates.position[0] = msg.current_pos;
	this->dynamixelJointStates.velocity[0] = msg.velocity;
	this->dynamixelJointStates.effort[0] = 0;
}


void updateJointStates(const std_msgs::String::ConstPtr& msg);


 ros::Subscriber DynamixelStateToJointState::initSubscriber(){
   ros::Subscriber dynamixelStateSub = this->n.subscribe("/head_lidar_controller/state",1,&DynamixelStateToJointState::updateJointStates,this);
   return dynamixelStateSub;
 }  

int main(int argc, char** argv)
{
    // Create publisher
    ros::init(argc, argv, "DynamixelStateToJointState");
    
    DynamixelStateToJointState d;
    
    ros::Publisher jointStatePub;
    d.jointStatePub = d.n.advertise<sensor_msgs::JointState>("/joint_states",1);
    ros::Subscriber dynamixelStateSub = d.initSubscriber();


    while(ros::ok())
    {
	d.jointStatePub.publish(d.dynamixelJointStates);
        ros::Duration(0.05).sleep();
        ros::spinOnce();
    }

    return 0;
}
