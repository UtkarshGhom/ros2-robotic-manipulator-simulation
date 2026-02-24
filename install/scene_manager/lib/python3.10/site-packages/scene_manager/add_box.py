import rclpy
from rclpy.node import Node
from moveit_commander import PlanningSceneInterface
from geometry_msgs.msg import PoseStamped
import time


class SceneManager(Node):

    def __init__(self):
        super().__init__("scene_manager")

        self.scene = PlanningSceneInterface()

        time.sleep(2)
        self.add_box()

    def add_box(self):

        box_pose = PoseStamped()
        box_pose.header.frame_id = "world"   # Must match your robot

        box_pose.pose.position.x = 0.4
        box_pose.pose.position.y = 0.0
        box_pose.pose.position.z = 0.05

        self.scene.add_box(
            "box",
            box_pose,
            size=(0.05, 0.05, 0.05)
        )

        self.get_logger().info("Box added to planning scene")


def main():
    rclpy.init()
    node = SceneManager()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
