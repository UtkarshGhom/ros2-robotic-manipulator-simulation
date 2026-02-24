# ROS2 Robotic Manipulator Simulation using MoveIt and Gazebo

This project implements a robotic manipulator simulation using ROS2, MoveIt 2, and Gazebo. It demonstrates robot modeling, motion planning, and manipulation in a simulated environment with full ROS2 integration.

---

## Overview

The system consists of a custom robotic arm described using URDF/Xacro, integrated with MoveIt 2 for motion planning and Gazebo for physics simulation. Custom ROS2 nodes enable motion control, scene management, and object interaction.

This project serves as a foundation for autonomous manipulation, pick-and-place tasks, and intelligent robotic control.

---

## Features

- Custom robotic arm modeled using URDF/Xacro  
- Motion planning and trajectory execution using MoveIt 2  
- Physics-based simulation using Gazebo  
- Visualization and planning using RViz2  
- Scene management for adding objects dynamically  
- Custom ROS2 nodes for arm control and motion execution  
- Modular ROS2 package structure  

---

## Project Structure


src/
├── robotic_arm_description/
│ ├── urdf/
│ ├── meshes/
│ ├── launch/
│
├── moveit_cfg/
│ ├── config/
│ ├── launch/
│
├── motion_control/
│ ├── scripts/
│ ├── move_arm.py
│
├── scene_manager/
│ ├── add_box.py


---

## Technologies Used

- ROS2 Humble
- MoveIt 2
- Gazebo Simulator
- RViz2
- Python
- URDF / Xacro
- Linux (Ubuntu 22.04)

---

## Features

- Custom robotic manipulator modeled using URDF/Xacro
- Motion planning using MoveIt 2
- Gazebo physics-based simulation
- Scene management and object spawning
- Custom ROS2 nodes for motion control
- Modular ROS2 package structure

---

## System Architecture

Robot Description → MoveIt Motion Planning → Controller → Gazebo Simulation → Visualization in RViz

---

## Installation

Clone the repository:
```bash
git clone https://github.com/UtkarshGhom/ros2-robotic-manipulator-simulation.git
``` 

Navigate to workspace:
```bash
cd ros2-robotic-manipulator-simulation
```

Build the workspace:
```bash
colcon build
```

Source the workspace:
```bash
source install/setup.bash
```
## Running the Simulation

Launch Gazebo simulation:
```bash
ros2 launch robotic_arm_description gazebo.launch.py
```
Launch MoveIt:
```bash
ros2 launch moveit_cfg demo.launch.py
```
Run motion control node:
```bash
ros2 run motion_control moving.cpp
```
## Results

The robotic manipulator successfully performs motion planning, executes trajectories, and interacts with objects within the Gazebo simulation environment.

## Applications

Robotic manipulation research

Autonomous pick-and-place systems

Industrial robot simulation

Motion planning research

Foundation for Physical AI and autonomous robots

## Future Improvements

Autonomous pick and place using perception

Integration with computer vision

Reinforcement learning based control

Real robot deployment

Integration with NVIDIA Isaac Sim

## Author

Utkarsh Ghom

B.Tech Computer Science (AI)
Robotics and Autonomous Systems Enthusiast

GitHub: https://github.com/UtkarshGhom