/*
 *  Gazebo - Outdoor Multi-Robot Simulator
 *  Copyright (C) 2003
 *     Nate Koenig & Andrew Howard
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/*
 @mainpage
   Desc: GazeboRosThermalCamera plugin for simulating cameras in Gazebo
   Author: John Hsu
   Date: 24 Sept 2008
   SVN info: $Id$
 @htmlinclude manifest.html
 @b GazeboRosThermalCamera plugin broadcasts ROS Image messages
 */

#include <hector_gazebo_thermal_camera/gazebo_ros_thermal_camera.h>

#include "sensors/Sensor.hh"
#include "sensors/CameraSensor.hh"
#include "sensors/SensorTypes.hh"

namespace gazebo
{

////////////////////////////////////////////////////////////////////////////////
// Constructor
GazeboRosThermalCamera::GazeboRosThermalCamera()
{
}

////////////////////////////////////////////////////////////////////////////////
// Destructor
GazeboRosThermalCamera::~GazeboRosThermalCamera()
{
}

void GazeboRosThermalCamera::Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf)
{
  CameraPlugin::Load(_parent, _sdf);
  // copying from CameraPlugin into GazeboRosThermalCameraUtils
  this->parentSensor_ = this->parentSensor;
  this->width_ = this->width;
  this->height_ = this->height;
  this->depth_ = this->depth;
  this->format_ = this->format;
  this->camera_ = this->camera;
  GazeboRosThermalCameraUtils::Load(_parent, _sdf);
}

////////////////////////////////////////////////////////////////////////////////
// Update the controller
void GazeboRosThermalCamera::OnNewFrame(const unsigned char *_image, 
    unsigned int _width, unsigned int _height, unsigned int _depth, 
    const std::string &_format)
{
  //ROS_ERROR("camera_ new frame %s %s",this->parentSensor_->GetName().c_str(),this->frameName.c_str());
  this->sensor_update_time_ = this->parentSensor_->GetLastUpdateTime();

  if (!this->parentSensor->IsActive())
  {
    if (this->imageConnectCount > 0)
      // do this first so there's chance for sensor to run 1 frame after activate
      this->parentSensor->SetActive(true);
  }
  else
  {
    if (this->imageConnectCount > 0)
    {
      common::Time cur_time = this->world->GetSimTime();
      if (cur_time - this->last_update_time_ >= this->update_period_)
      {
        this->PutCameraData(_image);
        this->last_update_time_ = cur_time;
      }
    }
  }
}

// Register this plugin with the simulator
GZ_REGISTER_SENSOR_PLUGIN(GazeboRosThermalCamera)

}