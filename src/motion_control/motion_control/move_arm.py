import rclpy
from rclpy.node import Node
from moveit_commander import MoveGroupCommander
from geometry_msgs.msg import PoseStamped
import time


class MotionControl(Node):

    def __init__(self):
        super().__init__("motion_control")

        # Change "arm" if your group name is different
        self.arm = MoveGroupCommander("arm")

        self.arm.set_planning_time(5.0)

        time.sleep(2)

        self.test_motion()

    def test_motion(self):

        self.get_logger().info("Moving arm...")

        pose = PoseStamped()
        pose.header.frame_id = "world"

        pose.pose.position.x = 0.35
        pose.pose.position.y = 0.0
        pose.pose.position.z = 0.30

        pose.pose.orientation.w = 1.0

        self.arm.set_pose_target(pose)
        self.arm.go(wait=True)

        self.get_logger().info("Motion done")


def main():
    rclpy.init()
    node = MotionControl()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
