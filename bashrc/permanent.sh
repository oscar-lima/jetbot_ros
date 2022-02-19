# git: every time you are located in a git repo it shows the branch in green color as part of the prompt
source $HOME/scripts/display_git_branch_in_prompt.sh

# handy alias
source $HOME/scripts/alias.sh

# enable ROS
# source /opt/ros/noetic/setup.bash # is called by /home/jetbot/workspace/catkin_ws/devel/setup.bash, so no need to call it twice
source $HOME/workspace/catkin_ws/devel/setup.bash # enable jetbot workspace (and ROS from /opt/ros/noetic/setup.bash)

# environment variables
source $HOME/scripts/environment.sh
