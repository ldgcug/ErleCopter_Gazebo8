/*
 * PinModePlugin.h
 * Author: Yuki Furuta <furushchev@jsk.imi.i.u-tokyo.ac.jp>
 */

#ifndef PINMODEPLUGIN_H__
#define PINMODEPLUGIN_H__

#include <boost/thread.hpp>

#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <ros/advertise_options.h>
#include <ros/subscribe_options.h>
#include <std_msgs/String.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Pose.h>

#include <gazebo/common/Plugin.hh>
#include <gazebo/common/Events.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/math/Vector3.hh>

namespace gazebo
{
  class PinModePlugin : public ModelPlugin {
    ros::NodeHandle *rosNode;
    ros::CallbackQueue rosQueue;
    event::ConnectionPtr updateConnection;
    boost::thread callbackQueueThread;
    boost::thread deferedLoadThread;

    std::string modeTopicName;
    ros::Subscriber subMode;

    physics::ModelPtr model;
    sdf::ElementPtr sdf;

    bool initWithPinMode;

    // robot params
    std::string robotName;
    std::string pinLinkName;
    physics::LinkPtr pinLink;
    physics::JointPtr pinJoint;
    std::string leftFootLinkName, rightFootLinkName;

    void DeferedLoad();
    void ROSQueueThread();

    void PinRobot(bool _with_gravity);
    void UnpinRobot();
    void SetFeetCollide(const std::string &_mode);
    physics::JointPtr AddJoint(physics::WorldPtr _world,
                               physics::ModelPtr _model,
                               physics::LinkPtr _link1,
                               physics::LinkPtr _link2,
                               std::string _type,
                               math::Vector3 _anchor,
                               math::Vector3 _axis,
                               double _upper, double _lower,
                               bool _disableCollision = false);
    void RemoveJoint(physics::JointPtr &_joint);

  protected:
    void UpdateStates();
  public:
    PinModePlugin();
    ~PinModePlugin();
    void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
    void SetRobotModeTopic(const std_msgs::String::ConstPtr &_str);
    void SetRobotMode(const std::string &_str);
  };
}

#endif // PINMODEPLUGIN_H__
