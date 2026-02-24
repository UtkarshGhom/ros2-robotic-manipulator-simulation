#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

from moveit_msgs.msg import CollisionObject
from shape_msgs.msg import SolidPrimitive
from geometry_msgs.msg import Pose


class SpawnBox(Node):

    def __init__(self):
        super().__init__("spawn_box_node")

        self.publisher = self.create_publisher(
            CollisionObject,
            "/collision_object",
            10
        )

        self.timer = self.create_timer(2.0, self.spawn_box)

        self.sent = False

    def spawn_box(self):

        if self.sent:
            return

        box = CollisionObject()

        box.header.frame_id = "world"
        box.id = "box1"

        primitive = SolidPrimitive()
        primitive.type = SolidPrimitive.BOX
        primitive.dimensions = [0.05, 0.05, 0.05]  # 5cm cube

        pose = Pose()
        pose.position.x = 0.3
        pose.position.y = 0.0
        pose.position.z = 0.1
        pose.orientation.w = 1.0

        box.primitives.append(primitive)
        box.primitive_poses.append(pose)

        box.operation = CollisionObject.ADD

        self.publisher.publish(box)

        self.get_logger().info("Box spawned in planning scene")

        self.sent = True


def main():

    rclpy.init()

    node = SpawnBox()

    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()

