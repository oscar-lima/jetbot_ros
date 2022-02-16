/*
 * Copyright (c) 2019, NVIDIA CORPORATION. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <ros/ros.h>

#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>

#include <jetson-utils/gstCamera.h>
#include <jetson-utils/videoOptions.h>
#include "image_converter.h"

// globals
gstCamera* camera = NULL;
videoOptions video_options;

imageConverter* camera_cvt = NULL;
ros::Publisher* camera_pub = NULL;


// aquire and publish camera frame
bool aquireFrame()
{
	float4* imgRGBA = NULL;

	// get the latest frame
	if( !camera->CaptureRGBA((float**)&imgRGBA, 1000) )
	{
		ROS_ERROR("failed to capture camera frame");
		return false;
	}

	// assure correct image size
	if( !camera_cvt->Resize(camera->GetWidth(), camera->GetHeight(), IMAGE_RGBA32F) )
	{
		ROS_ERROR("failed to resize camera image converter");
		return false;
	}

	// populate the message
	sensor_msgs::Image msg;

	if( !camera_cvt->Convert(msg, imageConverter::ROSOutputFormat, imgRGBA) )
	{
		ROS_ERROR("failed to convert camera frame to sensor_msgs::Image");
		return false;
	}

	// publish the message
	camera_pub->publish(msg);
	ROS_INFO("published camera frame");

	return true;
}


// node main loop
int main(int argc, char **argv)
{
	ros::init(argc, argv, "jetbot_camera");

	ros::NodeHandle nh;
	ros::NodeHandle private_nh("~");

	/*
	 * retrieve parameters
	 */
	std::string camera_device = "0";	// MIPI CSI camera by default

	private_nh.param<std::string>("device", camera_device, camera_device);

	ROS_INFO("opening camera device %s", camera_device.c_str());

	/*
	 * open camera device
	 */
	videoOptions mOptions;
	mOptions.width = 1280;
	mOptions.height = 720;
	mOptions.frameRate = 30.0;
	mOptions.bitRate = 0;
	mOptions.numBuffers = 4;
	mOptions.loop = 0;
	mOptions.rtspLatency = 2000;
	mOptions.zeroCopy = 0;
	mOptions.ioType = videoOptions::IoType::INPUT;
	mOptions.deviceType = videoOptions::DeviceType::DEVICE_CSI;
	mOptions.flipMethod = videoOptions::FlipMethod::FLIP_HORIZONTAL;
	mOptions.codec = videoOptions::Codec::CODEC_RAW;

	mOptions.resource.string = "csi://0";
	mOptions.resource.protocol = "csi";
	mOptions.resource.location = "0";
	mOptions.resource.port = 0;

	camera = gstCamera::Create(mOptions);

	if( !camera )
	{
		ROS_ERROR("failed to open camera device %s", camera_device.c_str());
		return 0;
	}


	/*
	 * create image converter
	 */
	camera_cvt = new imageConverter();

	if( !camera_cvt )
	{
		ROS_ERROR("failed to create imageConverter");
		return 0;
	}


	/*
	 * advertise publisher topics
	 */
	ros::Publisher camera_publisher = private_nh.advertise<sensor_msgs::Image>("raw", 2);
	camera_pub = &camera_publisher;


	/*
	 * start the camera streaming
	 */
	if( !camera->Open() )
	{
		ROS_ERROR("failed to start camera streaming");
		return 0;
	}


	/*
	 * start publishing video frames
	 */
	while( ros::ok() )
	{
		//if( raw_pub->getNumSubscribers() > 0 )
			aquireFrame();

		ros::spinOnce();
	}

	delete camera;
	return 0;
}

