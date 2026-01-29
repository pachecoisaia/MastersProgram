#ifndef robot_h
#define robot_h
#include "drivetrain/differential_drive.h"

// ============================================================
// POLOLU 3PI+ ROBOT CONTROL
// ============================================================
//
// Purpose: Robot initialization interface for Pololu 3pi+ differential drive
//
// Architecture:
//   - DifferentialDrive (public 'drive' member): Complete motor and motion control
//     - Low-level control: set_wheel_speeds(left, right)
//     - Basic motion: drive_forward(), drive_backward(), turn_left(), turn_right()
//     - High-level motion: move_forward(), move_backward(), turn_left(), turn_right()
//     - Curved motion: move_forward_turning_left/right(), move_backward_turning_left/right()
//     - Configuration: set_turn_speed_ratio(), flip_left_motor(), flip_right_motor()
//     - Stop: halt()
//   
//   - Robot: Robot initialization with configuration
//     - Constructors initialize DifferentialDrive with optional turn_speed_ratio
//     - Exposes public 'drive' member for all motion and configuration
//
// Usage:
//   Robot robot;
//   robot.drive->move_forward(1.0, 0.2);        // Motion through drive
//   robot.drive->flip_left_motor(true);         // Config through drive
//   robot.drive->set_turn_speed_ratio(0.6);     // Config through drive
//
// ============================================================
// ============================================================
class Robot {
  public:
    // ========== CONSTRUCTORS ==========
    
    // Purpose: Initialize robot with default configuration
    // Args: None
    // Return: void
    Robot();
    
    
    // Public motor and motion abstraction
    DifferentialDrive* drive;
};

#endif
