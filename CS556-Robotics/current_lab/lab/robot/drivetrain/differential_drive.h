#ifndef differential_drive_h
#define differential_drive_h

#include <Pololu3piPlus32U4.h>
#include "../configurable.h"
#include "../utils/logger.h"
using namespace Pololu3piPlus32U4;

// ============================================================
// DIFFERENTIAL DRIVE ABSTRACTION
// ============================================================
//
// Purpose: Encapsulate the differential drive mechanism with 2 independently controlled DC motors
//
// Hardware:
//   - 2 DC Motors: Left wheel motor and right wheel motor
//   - Controlled via Pololu3piPlus32U4::Motors class
//   - Each motor speed range: -400 (full reverse) to +400 (full forward)
//   - setSpeeds(leftSpeed, rightSpeed) controls both motors
//   - Wheelbase (L): Default 96mm for Pololu 3pi+ (distance between wheels)
//
// Description: 
//   A differential drive is a locomotion system with two independently controlled DC motors
//   (one for left wheel, one for right wheel) on opposite sides of the robot. By varying 
//   the speeds of the left and right motors, we can achieve:
//     - Forward/backward motion (both motors same speed, same direction)
//     - In-place rotation (motors same speed, opposite directions)
//     - Curved motion (motors different speeds, same direction)
//
// Motor Speed Convention:
//   - Positive speed: Motor spins forward
//   - Negative speed: Motor spins backward
//   - Speed range: -400 to +400 (values outside this range are clamped)
//
// Mathematical Model:
//   - Left motor speed: V_left (mm/s)
//   - Right motor speed: V_right (mm/s)
//   - Robot forward speed: V = (V_left + V_right) / 2
//   - Robot angular speed: ω = (V_right - V_left) / wheel_base
//   - Rotation angle: θ = (2*v/L) * t, where v is wheel speed, L is wheelbase, t is time
//
// This class provides a clean interface to control the two DC motors through
// the abstraction of differential drive semantics rather than raw motor values.
//
// ============================================================

// Enum to specify turn mode: angle-based or duration-based
enum class TurnMode {
  ANGLE,      // Rotation based on angle in radians
  DURATION    // Rotation based on time duration in seconds
};

// Default configuration constants for Pololu 3pi+ robot
// Updated to match the provided wheelbase (distance between wheel centers).
const float DEFAULT_WHEELBASE_MM = 98.0f;  // Distance between left and right wheels [mm]
const float DEFAULT_TURN_SPEED_RATIO = 0.5f;  // Inner wheel speed as fraction of outer wheel speed
const bool DEFAULT_FLIP_LEFT_MOTOR = false;  // Set to true if left motor is wired backwards
const bool DEFAULT_FLIP_RIGHT_MOTOR = false;  // Set to true if right motor is wired backwards

class DifferentialDrive : public Configurable {
  public:
    // Purpose: Initialize differential drive with Pololu Motors
    // Description: Creates instance with reference to the robot's Motors controller
    // Args: None
    // Return: void
    DifferentialDrive();
    
    // ========== CONFIGURATION ==========
    
    // Purpose: Configure the differential drive with default settings
    // Description: Applies default configuration for turn speed ratio, wheelbase, and motor directions
    // Args: None
    // Return: void
    void configure() override;
    
    // ========== MOTION PRIMITIVES ==========
    
    // Purpose: Move robot straight forward for a specified distance
    // Description: Drives both wheels forward at identical speed
    // Args: distance_m - distance to travel in meters (positive)
    //       speed_m_per_s - forward speed in m/s (0.0 to 0.4)
    // Return: void
    void move_forward(float distance_m, float speed_m_per_s);
    
    // Purpose: Move robot straight backward for a specified distance
    // Description: Drives both wheels backward at identical speed
    // Args: distance_m - distance to travel in meters (positive)
    //       speed_m_per_s - backward speed in m/s (0.0 to 0.4)
    // Return: void
    void move_backward(float distance_m, float speed_m_per_s);
    
    // Purpose: Rotate robot counterclockwise in place for a specified duration
    // Description: Left wheel backward, right wheel forward
    //   Default behavior matches TurnMode::DURATION
    // Args: duration_s - rotation duration in seconds
    //       speed_m_per_s - rotation speed in m/s (0.0 to 0.4)
    // Return: void
    void turn_left(float duration_s, float speed_m_per_s);
    
    // Purpose: Rotate robot counterclockwise with mode selection
    // Description: Left wheel backward, right wheel forward
    //   Mode switches between angle-based and duration-based control
    // Args: thetaOrTime - rotation angle (rad) if ANGLE mode, or duration (s) if DURATION mode
    //       speed_m_per_s - rotation speed in m/s (0.0 to 0.4)
    //       mode - TurnMode::ANGLE or TurnMode::DURATION (default: DURATION)
    // Return: void
    void turn_left(float thetaOrTime, float speed_m_per_s, TurnMode mode = TurnMode::DURATION);
    
    // Purpose: Rotate robot clockwise in place for a specified duration
    // Description: Right wheel backward, left wheel forward
    //   Default behavior matches TurnMode::DURATION
    // Args: duration_s - rotation duration in seconds
    //       speed_m_per_s - rotation speed in m/s (0.0 to 0.4)
    // Return: void
    void turn_right(float duration_s, float speed_m_per_s);
    
    // Purpose: Rotate robot clockwise with mode selection
    // Description: Right wheel backward, left wheel forward
    //   Mode switches between angle-based and duration-based control
    // Args: thetaOrTime - rotation angle (rad) if ANGLE mode, or duration (s) if DURATION mode
    //       speed_m_per_s - rotation speed in m/s (0.0 to 0.4)
    //       mode - TurnMode::ANGLE or TurnMode::DURATION (default: DURATION)
    // Return: void
    void turn_right(float thetaOrTime, float speed_m_per_s, TurnMode mode = TurnMode::DURATION);
    
    // Purpose: Move robot forward while turning left
    // Description: Left wheel reduced speed, right wheel forward speed
    //   Inner wheel speed controlled by turn_speed_ratio
    // Args: distance_m - distance to travel in meters (positive)
    //       speed_m_per_s - outer wheel speed in m/s (0.0 to 0.4)
    // Return: void
    void move_forward_turning_left(float distance_m, float speed_m_per_s);
    
    // Purpose: Move robot forward while turning right
    // Description: Right wheel reduced speed, left wheel forward speed
    //   Inner wheel speed controlled by turn_speed_ratio
    // Args: distance_m - distance to travel in meters (positive)
    //       speed_m_per_s - outer wheel speed in m/s (0.0 to 0.4)
    // Return: void
    void move_forward_turning_right(float distance_m, float speed_m_per_s);
    
    // Purpose: Move robot backward while turning left
    // Description: Left wheel reduced speed backward, right wheel backward speed
    //   Inner wheel speed controlled by turn_speed_ratio
    // Args: distance_m - distance to travel in meters (positive)
    //       speed_m_per_s - outer wheel speed in m/s (0.0 to 0.4)
    // Return: void
    void move_backward_turning_left(float distance_m, float speed_m_per_s);
    
    // Purpose: Move robot backward while turning right
    // Description: Right wheel reduced speed backward, left wheel backward speed
    //   Inner wheel speed controlled by turn_speed_ratio
    // Args: distance_m - distance to travel in meters (positive)
    //       speed_m_per_s - outer wheel speed in m/s (0.0 to 0.4)
    // Return: void
    void move_backward_turning_right(float distance_m, float speed_m_per_s);

    // Continuous low-level helpers (non-blocking, no delay): caller must halt() when done.
    void drive_forward_unbounded(int speed_mm_per_s);
    void drive_backward_unbounded(int speed_mm_per_s);
    void turn_left_unbounded(int speed_mm_per_s);
    void turn_right_unbounded(int speed_mm_per_s);
    
    // ========== CONFIGURATION ==========
    
    // Purpose: Configure the turn speed reduction factor
    // Description: Sets the ratio that reduces inner wheel speed during turning (0.5 = half speed for inner wheel)
    // Args: ratio - turn speed factor [0.0, 1.0] where 1.0 = full speed, 0.0 = no turn movement
    // Return: void
    // Throws: std::out_of_range if ratio is outside valid bounds
    void set_turn_speed_ratio(float ratio);
    
    // Purpose: Get the turn speed reduction factor
    // Description: Returns the ratio that reduces inner wheel speed during turning
    // Args: None
    // Return: float - turn speed factor [0.0, 1.0]
    float get_turn_speed_ratio();
    
    // Purpose: Configure the wheelbase (distance between left and right wheels)
    // Description: Sets the wheelbase used in rotation calculations
    //   Default is 96mm for Pololu 3pi+, but can be adjusted for calibration
    // Args: wheelbase_mm - wheelbase in millimeters (positive value)
    // Return: void
    // Throws: std::out_of_range if wheelbase_mm is non-positive
    void set_wheelbase(float wheelbase_mm);
    
    // Purpose: Get the current wheelbase configuration
    // Description: Returns the wheelbase used in rotation calculations
    // Args: None
    // Return: float - wheelbase in millimeters
    float get_wheelbase();
    
    // Purpose: Flip the direction of the left DC motor
    // Description: Inverts the polarity of the left motor so positive speed = opposite direction
    //   Use this if the motor is wired backwards and you want positive speed to mean forward
    // Args: flip - true to invert left motor direction, false for standard polarity
    // Return: void
    void flip_left_motor(bool flip);
    
    // Purpose: Flip the direction of the right DC motor
    // Description: Inverts the polarity of the right motor so positive speed = opposite direction
    //   Use this if the motor is wired backwards and you want positive speed to mean forward
    // Args: flip - true to invert right motor direction, false for standard polarity
    // Return: void
    void flip_right_motor(bool flip);
    
    // Purpose: Stop all motors immediately
    // Description: Sets both motor speeds to 0
    // Args: None
    // Return: void
    void halt();
    
  private:
    // ========== LOW-LEVEL MOTOR CONTROL ==========
    
    // Purpose: Set motor speeds for the left and right wheels
    // Description: Directly controls the left and right wheel DC motors in their raw speed range
    //   Positive = forward, Negative = backward
    // Args: left_speed_mm_per_s - left motor speed (typically -400 to 400)
    //       right_speed_mm_per_s - right motor speed (typically -400 to 400)
    // Return: void
    void set_wheel_speeds(int left_speed_mm_per_s, int right_speed_mm_per_s);
    
    // Purpose: Move both wheels forward at the same speed
    // Description: Drives both DC motors at identical forward speed for straight-line motion
    // Args: speed_mm_per_s - forward speed of both motors (typically 0 to 400)
    // Return: void
    void drive_forward(int speed_mm_per_s);
    
    // Purpose: Move both wheels backward at the same speed
    // Description: Drives both DC motors at identical backward speed for straight-line motion
    // Args: speed_mm_per_s - backward speed of both motors (typically 0 to 400)
    // Return: void
    void drive_backward(int speed_mm_per_s);
    
    // Purpose: Rotate in place counterclockwise (low-level)
    // Description: Runs left DC motor backward and right DC motor forward at same speed
    // Args: speed_mm_per_s - rotational speed (typically 0 to 400)
    // Return: void
    void turn_left_low_level(int speed_mm_per_s);
    
    // Purpose: Rotate in place clockwise (low-level)
    // Description: Runs right DC motor backward and left DC motor forward at same speed
    // Args: speed_mm_per_s - rotational speed (typically 0 to 400)
    // Return: void
    void turn_right_low_level(int speed_mm_per_s);
    
    // Helper methods for duration-based turning
    void turn_left_duration(float duration_s, float speed_m_per_s);
    void turn_right_duration(float duration_s, float speed_m_per_s);
    
    // Helper methods for angle-based turning
    void turn_left_angle(float angle_rad, float speed_m_per_s);
    void turn_right_angle(float angle_rad, float speed_m_per_s);
    
    // ========== DATA MEMBERS ==========
    
    // Two independently controlled DC motors (left and right wheels)
    Motors motors;                // Pololu Motors class controlling both DC motors
    float turn_speed_ratio;       // Inner wheel speed multiplier [0.0, 1.0]
    float wheelbase_mm;           // Distance between left and right wheels (mm)
};

#endif
