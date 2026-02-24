#include <rclcpp/rclcpp.hpp>
#include <moveit/move_group_interface/move_group_interface.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <random>

using namespace std::chrono_literals;

class PickPlaceCommander {
public:
    PickPlaceCommander(const rclcpp::Node::SharedPtr& node) : node_(node) {
        // Initialize MoveGroupInterfaces for your specific groups
        arm_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node_, "arm");
        gripper_group_ = std::make_shared<moveit::planning_interface::MoveGroupInterface>(node_, "gripper");

        // Set speed limits
        arm_group_->setMaxVelocityScalingFactor(0.5);
        arm_group_->setMaxAccelerationScalingFactor(0.5);
    }

    void spawnObject() {
        // Generate a random X and Y position for the box
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist_x(0.4, 0.6); // Between 40cm and 60cm
        std::uniform_real_distribution<> dist_y(-0.2, 0.2); // Left or right

        double random_x = dist_x(gen);
        double random_y = dist_y(gen);

        moveit_msgs::msg::CollisionObject box;
        box.header.frame_id = "base_link";
        box.id = "pick_box";

        shape_msgs::msg::SolidPrimitive primitive;
        primitive.type = primitive.BOX;
        primitive.dimensions = {0.05, 0.05, 0.05}; // 5cm cube

        geometry_msgs::msg::Pose box_pose;
        box_pose.orientation.w = 1.0;
        box_pose.position.x = random_x;
        box_pose.position.y = random_y;
        box_pose.position.z = 0.025; // Half of box height

        box.primitives.push_back(primitive);
        box.primitive_poses.push_back(box_pose);
        box.operation = box.ADD;

        psi_.applyCollisionObject(box);
        RCLCPP_INFO(node_->get_logger(), "Spawned box at X: %.2f, Y: %.2f", random_x, random_y);
        
        // Save coordinates for the pick sequence
        target_x_ = random_x;
        target_y_ = random_y;
    }

    void executeSequence() {
        RCLCPP_INFO(node_->get_logger(), "--- Starting Sequence ---");

        // 1. Open Gripper (Uses your SRDF name 'open')
        RCLCPP_INFO(node_->get_logger(), "Opening Gripper...");
        gripper_group_->setNamedTarget("open");
        execute(gripper_group_);

        // 2. Move to Pre-Grasp position (above the box)
        RCLCPP_INFO(node_->get_logger(), "Moving to Pre-Grasp...");
        moveToPose(target_x_, target_y_, 0.2, 0.0, M_PI, 0.0); 

        // 3. Move down to grasp (Cartesian path for straight movement)
        RCLCPP_INFO(node_->get_logger(), "Lowering...");
        moveToPose(target_x_, target_y_, 0.12, 0.0, M_PI, 0.0);

        // 4. Close Gripper (Uses the 'close' state we added to SRDF)
        RCLCPP_INFO(node_->get_logger(), "Grasping...");
        gripper_group_->setNamedTarget("close");
        execute(gripper_group_);
        
        // Attach the object to the robot
        arm_group_->attachObject("pick_box", "Gripper_Base_1");

        // 5. Lift up
        RCLCPP_INFO(node_->get_logger(), "Lifting...");
        moveToPose(target_x_, target_y_, 0.3, 0.0, M_PI, 0.0);

        // 6. Return Home
        RCLCPP_INFO(node_->get_logger(), "Returning Home...");
        arm_group_->setNamedTarget("home");
        execute(arm_group_);

        RCLCPP_INFO(node_->get_logger(), "--- Sequence Complete ---");
    }

private:
    void moveToPose(double x, double y, double z, double r, double p, double yaw) {
        tf2::Quaternion q;
        q.setRPY(r, p, yaw);

        geometry_msgs::msg::Pose target_pose;
        target_pose.position.x = x;
        target_pose.position.y = y;
        target_pose.position.z = z;
        target_pose.orientation = tf2::toMsg(q);

        arm_group_->setPoseTarget(target_pose);
        execute(arm_group_);
    }

    void execute(std::shared_ptr<moveit::planning_interface::MoveGroupInterface> group) {
        moveit::planning_interface::MoveGroupInterface::Plan plan;
        if (group->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS) {
            group->execute(plan);
        } else {
            RCLCPP_ERROR(node_->get_logger(), "Planning failed!");
        }
    }

    rclcpp::Node::SharedPtr node_;
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> arm_group_;
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> gripper_group_;
    moveit::planning_interface::PlanningSceneInterface psi_;
    double target_x_, target_y_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("moving_node", 
        rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true));

    // We need a multi-threaded executor for MoveIt
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    
    auto commander = std::make_shared<PickPlaceCommander>(node);

    // Run the logic in a separate thread so the executor can spin in the background
    std::thread run_thread([commander]() {
        rclcpp::sleep_for(2s); // Wait for system to settle
        commander->spawnObject();
        rclcpp::sleep_for(1s);
        commander->executeSequence();
    });

    executor.spin();
    run_thread.join();
    rclcpp::shutdown();
    return 0;
}
