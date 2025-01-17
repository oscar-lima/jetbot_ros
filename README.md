# jetbot_ros

ROS 1 noetic jetbot wrapper, this repository builds on top of https://github.com/dusty-nv/jetbot_ros melodic branch, but was migrated and tested on noetic on a real jetbot.

Check official jetbot documentation under:

https://jetbot.org/master/
 
### Run JetBot

If you have a real JetBot, you can start the camera / motors like so:

``` bash
roscore
rosrun jetbot_ros jetbot_camera
rosrun jetbot_ros jetbot_motors.py
```

The camera node publishes images under the topic ```/jetbot_ros/raw```

Those images could be visulized e.g. by using rviz

To move the motors, the jetbot_motors node subscribes to the topic ```/jetbot_motors/cmd_str```

where you can publish a string with the following options: "left", "right", "forward", "backward", "stop"

### Launch Gazebo

This is work in progress and not yet ported to noetic but is comming soon ; )

### Test Teleop

This is work in progress and not yet ported to noetic but is comming soon ; )

jetbot gamepad

from: http://underpop.online.fr/2/2-4ghz-wireless-gamepad-controller-joystick-for-pc-raspberry-pi-retropie-android-smart-tv-box-tablet-pc-ps3-nespi-case-2-pcs/
the name of the gamepad is:

    GeeekStudio 2.4G Wireless Gamepad Game

The joypad is recognized as a generic Linux joystick, therefore http://wiki.ros.org/joy can be used.

Once you plug in the USB receiver it will show in the system like:

    ls /dev/input/js0

or

    ls /dev/input/js*

Then simply launch it with:

    roslaunch jetbot_ros teleop_joypad.launch
