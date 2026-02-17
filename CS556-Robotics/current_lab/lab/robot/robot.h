#ifndef robot_h
#define robot_h
#include "drivetrain/differential_drive.h"
#include "sensors/sonar.h"
#include "actuators/servo_controller.h"
#include "display/display.h"
#include "navigator/navigator.h"
#include "display/display.h"

// ============================================================
// POLOLU 3PI+ ROBOT CONTROL
// ============================================================
//
// Purpose: Robot initialization interface for Pololu 3pi+ robot with sensors and actuators
//
// Architecture:
//   - DifferentialDrive (public 'drive' member): Complete motor and motion control
//     - Low-level control: Direct wheel speed commands - set_wheel_speeds(left, right)
//     - Basic motion: Simple directional commands without distance/speed parameters
//     - High-level motion: Distance-based movement with speed control
//     - Curved motion: Combined forward/backward movement with simultaneous turning
//     - Configuration: Setup functions for motor flipping and turn speed ratios
//     - Stop: halt()
//   
//   - Sonar (public 'sonar' member): Ultrasonic distance sensing
//     - Distance measurement: Single and averaged readings in cm
//     - Configuration: Pin assignment, timeout, sample count
//     - Validation: Range checking and error handling
//   
//   - ServoController (public 'servo' member): Servo motor positioning
//     - Position control: Angle-based positioning (0-180°)
//     - Smooth movement: Speed-controlled transitions
//     - Sweep functions: Automated scanning patterns
//     - Configuration: Pin assignment, speed control
//
//   - Display (public 'display' member): OLED display helpers
//     - Encoder printing
//     - Pose (odometry) printing
//   
//   - Robot: Robot initialization with configuration
//     - Constructors initialize all subsystems (drive, navigator, sonar, servo, display)
//     - Exposes public members for all subsystem access
//
// Usage:
//   Robot robot;
//   robot.drive->move_forward(1.0, 0.2);        // Motion through drive
//   robot.sonar->read_distance_cm();            // Distance sensing through sonar
//   robot.servo->move_to_angle(90);             // Servo positioning
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
    
    
    // Public subsystem abstractions
    DifferentialDrive* drive;   // Drivetrain control
    Navigator* navigator;        // Encoders + odometry pose tracking
    Sonar* sonar;               // Distance sensor
    ServoController* servo;     // Servo actuator
    Display* display;           // OLED display helper
};

#endif
