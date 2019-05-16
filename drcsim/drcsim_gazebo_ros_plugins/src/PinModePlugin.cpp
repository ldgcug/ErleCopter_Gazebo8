/*
 * PinModePlugin.cpp
 * Author: Yuki Furuta <furushchev@jsk.imi.i.u-tokyo.ac.jp>
 */

#include "drcsim_gazebo_ros_plugins/PinModePlugin.h"

namespace gazebo {
  GZ_REGISTER_MODEL_PLUGIN(PinModePlugin)

  PinModePlugin::PinModePlugin() : rosNode(NULL), robotName("ROBOT"), pinLinkName("TORSO"), leftFootLinkName("L_FOOT"), rightFootLinkName("R_FOOT"), initWithPinMode(false)
  {}

  PinModePlugin::~PinModePlugin()
  {
    rosNode->shutdown();
    rosQueue.clear();
    rosQueue.disable();
    callbackQueueThread.join();
    delete rosNode;
  }

  void PinModePlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
  {
    model = _model;
    sdf = _sdf;

    if (sdf->HasElement("robotname"))
      robotName = sdf->Get<std::string>("robotname");
    modeTopicName = robotName + "/mode";

    if (sdf->HasElement("pinlink"))
      pinLinkName = sdf->Get<std::string>("pinlink");
    pinLink = model->GetLink(pinLinkName);

    if (sdf->HasElement("l_foot_link"))
      leftFootLinkName = sdf->Get<std::string>("l_foot_link");

    if (sdf->HasElement("r_foot_link"))
      rightFootLinkName = sdf->Get<std::string>("r_foot_link");

    if (sdf->HasElement("init_with_pin"))
      initWithPinMode = sdf->Get<bool>("init_with_pin");

    this->DeferedLoad();
    gzmsg << "PinMode Initialized and advertised as " << modeTopicName << "\n";
  }

  void PinModePlugin::DeferedLoad()
  {
    if(!ros::isInitialized())
    {
      gzerr << "Not loading PinMode plugin since ROS hasn't been "
            << "properly initialized.  Try starting gazebo with ros plugin:\n"
            << "  gazebo -s libgazebo_ros_api_plugin.so\n";
      return;
    }

    this->rosNode = new ros::NodeHandle("");

    // subscribe mode
    ros::SubscribeOptions mode_so = ros::SubscribeOptions::create<std_msgs::String>(
      modeTopicName, 100, boost::bind(&PinModePlugin::SetRobotModeTopic, this, _1),
      ros::VoidPtr(), &this->rosQueue);
    this->subMode = this->rosNode->subscribe(mode_so);

    this->callbackQueueThread = boost::thread(boost::bind(&PinModePlugin::ROSQueueThread, this));

    this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&PinModePlugin::UpdateStates, this));
  }

  void PinModePlugin::PinRobot(bool _with_gravity)
  {
    if(!this->pinJoint)
    {
      this->pinJoint = this->AddJoint(this->model->GetWorld(),
                                      this->model,
                                      physics::LinkPtr(),
                                      this->pinLink,
                                      "revolute",
                                      math::Vector3(0,0,0),
                                      math::Vector3(0,0,1),
                                      0.0, 0.0);
    }
    this->model->SetGravityMode(_with_gravity);
    this->SetFeetCollide("none");
  }

  void PinModePlugin::UnpinRobot()
  {
    physics::WorldPtr w = model->GetWorld();
    this->model->SetGravityMode(true);
    if (this->pinJoint)
      this->RemoveJoint(this->pinJoint);
    this->SetFeetCollide("all");
    if (w->GetPhysicsEngine()->GetType() == "simbody"||
        w->GetPhysicsEngine()->GetType() == "dart")
    {
      gzerr << "physics engines \"simbody\", \"dart\" are not supported.\n";
      return;
    }
  }

  physics::JointPtr PinModePlugin::AddJoint(physics::WorldPtr _world,
                                            physics::ModelPtr _model,
                                            physics::LinkPtr _link1,
                                            physics::LinkPtr _link2,
                                            std::string _type,
                                            math::Vector3 _anchor,
                                            math::Vector3 _axis,
                                            double _upper, double _lower,
                                            bool _disableCollision)
  {
    physics::JointPtr joint;
    if (_world->GetPhysicsEngine()->GetType() == "ode" ||
        _world->GetPhysicsEngine()->GetType() == "bullet")
    {
      joint = _world->GetPhysicsEngine()->CreateJoint(
        _type, _model);
      joint->Attach(_link1, _link2);
      // load adds the joint to a vector of shared pointers kept
      // in parent and child links, preventing joint from being destroyed.
      joint->Load(_link1, _link2, math::Pose(_anchor, math::Quaternion()));
      joint->SetAxis(0, _axis);
      joint->SetHighStop(0, _upper);
      joint->SetLowStop(0, _lower);

      if (_link1)
        joint->SetName(_link1->GetName() + std::string("_") +
                       _link2->GetName() + std::string("_joint"));
      else
        joint->SetName(std::string("world_") +
                       _link2->GetName() + std::string("_joint"));
      joint->Init();


      // disable collision between the link pair
      if (_disableCollision)
      {
        if (_link1)
          _link1->SetCollideMode("fixed");
        if (_link2)
          _link2->SetCollideMode("fixed");
      }
    }
    else if (_world->GetPhysicsEngine()->GetType() == "simbody" ||
             _world->GetPhysicsEngine()->GetType() == "dart")
    {
      gzerr << "physics engines \"simbody\", \"dart\" are not supported on this version.\n";
    }

    return joint;
  }


  void PinModePlugin::RemoveJoint(physics::JointPtr &_joint)
  {
    physics::WorldPtr w = model->GetWorld();
    bool paused = w->IsPaused();
    w->SetPaused(true);
    if (_joint)
    {
      // reenable collision between the link pair
      physics::LinkPtr parent = _joint->GetParent();
      physics::LinkPtr child = _joint->GetChild();
      if (parent)
        parent->SetCollideMode("all");
      if (child)
        child->SetCollideMode("all");

      _joint->Detach();
      _joint.reset();
    }
    w->SetPaused(paused);
  }

  void PinModePlugin::SetFeetCollide(const std::string &_mode)
  {
    physics::LinkPtr l_foot_link = this->model->GetLink(this->leftFootLinkName);
    if (!l_foot_link)
      ROS_WARN_STREAM("Couldn't find " << this->leftFootLinkName << " link when setting collide mode");
    else
      l_foot_link->SetCollideMode(_mode);

    physics::LinkPtr r_foot_link = this->model->GetLink(this->rightFootLinkName);
    if (!r_foot_link)
      ROS_WARN_STREAM("Couldn't find " << this->rightFootLinkName << " link when setting collide mode");
    else
      r_foot_link->SetCollideMode(_mode);
  }

  void PinModePlugin::SetRobotModeTopic(const std_msgs::String::ConstPtr &_msg)
  {
    this->SetRobotMode(_msg->data);
  }

  void PinModePlugin::SetRobotMode(const std::string &_str)
  {
    if (_str == "pinned")
    {
      this->PinRobot(false);
    }
    else if (_str == "pinned_with_glavity")
    {
      this->PinRobot(true);
    }
    else if (_str == "nominal")
    {
      this->UnpinRobot();
    }
    else
    {
      gzerr << "invalid mode. skipping...\n";
    }
  }

  void PinModePlugin::ROSQueueThread()
  {
    static const double timeout = 0.01;

    while (this->rosNode->ok())
    {
      this->rosQueue.callAvailable(ros::WallDuration(timeout));
    }
  }

  void PinModePlugin::UpdateStates() {
    if (initWithPinMode) {
      gzmsg << "initialized with pin mode enabled\n";
      PinRobot(false);
      initWithPinMode = false;
    }
  }
}

