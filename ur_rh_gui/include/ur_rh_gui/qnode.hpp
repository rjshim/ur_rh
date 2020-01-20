﻿/*******************************************************************************
* Copyright 2020 ROBOTIS CO., LTD.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/* Authors: Ryan Shim */

/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef UR_RH_GUI_QNODE_HPP_
#define UR_RH_GUI_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

// To workaround boost/qt4 problems that won't be bugfixed. Refer to
//    https://bugreports.qt.io/browse/QTBUG-22829
#ifndef Q_MOC_RUN
#include <ros/ros.h>
#endif
#include <string>
#include <QThread>
#include <QStringListModel>

#include <sensor_msgs/JointState.h>
#include <std_msgs/String.h>

#include <eigen3/Eigen/Eigen>

#include "robotis_manipulator/robotis_manipulator.h"

#include "open_manipulator_msgs/OpenManipulatorState.h"
#include "open_manipulator_msgs/SetJointPosition.h"
#include "open_manipulator_msgs/SetKinematicsPose.h"
#include "open_manipulator_msgs/SetActuatorState.h"

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/planning_interface/planning_interface.h>

#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>

#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/ExecuteTrajectoryActionGoal.h>
#include <moveit_msgs/MoveGroupActionGoal.h>


#include <geometry_msgs/PoseStamped.h>


#define NUM_OF_JOINT_AND_TOOL 7

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace ur_rh_gui {

/*****************************************************************************
** Class
*****************************************************************************/

class QNode : public QThread {
    Q_OBJECT
public:
	QNode(int argc, char** argv );
	virtual ~QNode();
	bool init();
	void run();

	/*********************
	** Logging
	**********************/
	enum LogLevel {
	         Debug,
	         Info,
	         Warn,
	         Error,
	         Fatal
	 };

	QStringListModel* loggingModel() { return &logging_model; }
	void log( const LogLevel &level, const std::string &msg);

  void manipulatorStatesCallback(const open_manipulator_msgs::OpenManipulatorState::ConstPtr &msg);
  void jointStatesCallback(const sensor_msgs::JointState::ConstPtr &msg);

  std::vector<double> getPresentJointAngle();
  std::vector<double> getPresentKinematicsPosition();
  Eigen::Quaterniond getPresentKinematicsOrientation();
  Eigen::Vector3d getPresentKinematicsOrientationRPY();
  bool getOpenManipulatorMovingState();
  bool getOpenManipulatorActuatorState();
  bool getWithGripperState();

  void setOption(std::string opt);
  bool setJointSpacePath(std::vector<double> joint_angle, double path_time);
  bool setTaskSpacePath(std::vector<double> kinematics_pose, double path_time);
  bool setToolControl(std::vector<double> joint_angle);
  bool setActuatorState(bool actuator_state);

Q_SIGNALS:
  void rosShutdown();

private:
	int init_argc;
	char** init_argv;
  QStringListModel logging_model;

  // ROS Publishers
  ros::Publisher open_manipulator_option_pub_;

  // ROS Subscribers
  ros::Subscriber open_manipulator_states_sub_;
  ros::Subscriber open_manipulator_joint_states_sub_;

  // ROS Service Clients
  ros::ServiceClient goal_joint_space_path_client_;
  ros::ServiceClient goal_task_space_path_position_only_client_;
  ros::ServiceClient goal_task_space_path_client_;
  ros::ServiceClient goal_tool_control_client_;
  ros::ServiceClient set_actuator_state_client_;

  std::vector<double> present_joint_angle_;
  std::vector<double> present_kinematics_position_;
  Eigen::Quaterniond present_kinematics_orientation_;
  Eigen::Vector3d present_kinematics_orientation_rpy_;
  open_manipulator_msgs::KinematicsPose kinematics_pose_;

  bool open_manipulator_is_moving_;
  bool open_manipulator_actuator_enabled_;

  // MoveIt! interface
  moveit::planning_interface::MoveGroupInterface* move_group_;
};

}  // namespace ur_rh_gui

#endif /* UR_RH_GUI_QNODE_HPP_ */