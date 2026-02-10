#ifndef servo_controller_h
#define servo_controller_h

#include <Pololu3piPlus32U4.h>
#include <Servo.h>
#include "../configurable.h"
#include "../utils/logger.h"
using namespace Pololu3piPlus32U4;

// ============================================================
// SERVO CONTROLLER ABSTRACTION
// ============================================================
//
// Purpose: Encapsulate servo motor control for sensor positioning
//
// Hardware:
//   - Standard hobby servo motor (e.g., SG90, MG90S)
//   - PWM control signal (typically 50Hz, 1-2ms pulse width)
//   - Operating voltage: 4.8V-6V
//   - Rotation range: 0° to 180°
//
// Description:
//   A servo motor controller that provides precise angular positioning
//   for mounting and orienting sensors (like the sonar sensor). Servo
//   motors use PWM signals to maintain a specific angular position.
//
// Working Principle:
//   1. Servo receives PWM signal on control pin
//   2. Pulse width determines target angle:
//      - 1.0ms pulse → 0° position
//      - 1.5ms pulse → 90° position (center)
//      - 2.0ms pulse → 180° position
//   3. Internal feedback loop maintains position
//
// Mathematical Model:
//   - Angle (degrees) = 0 to 180
//   - Pulse width (ms) = 1.0 + (angle / 180) * 1.0
//   - PWM frequency: 50Hz (20ms period)
//
// Common Applications:
//   - Sensor panning and tilting
//   - Gripper control
//   - Camera positioning
//   - Directional antenna or sensor array
//
// This class provides a clean interface to control servo position
// with smooth transitions, speed control, and safety limits.
//
// ============================================================

// Default configuration constants
const int DEFAULT_SERVO_PIN = 5;           // GPIO pin for servo control
const int DEFAULT_SERVO_ANGLE = 90;        // Default center position (degrees)
const int MIN_SERVO_ANGLE = 0;             // Minimum angle (degrees)
const int MAX_SERVO_ANGLE = 180;           // Maximum angle (degrees)
const int DEFAULT_SERVO_SPEED = 60;        // Default speed in degrees/second
const int DEFAULT_SERVO_STEP_DELAY = 15;   // Delay between steps in ms
const int DEFAULT_SETTLING_TIME_MS = 200;  // Time for servo to reach position

class ServoController : public Configurable {
  public:
    // Purpose: Initialize servo controller
    // Description: Creates instance with default configuration
    // Args: None
    // Return: void
    ServoController();
    
    // ========== CONFIGURATION ==========
    
    // Purpose: Configure the servo controller with default settings
    // Description: Applies default configuration for pin, angle, speed
    // Args: None
    // Return: void
    void configure() override;
    
    // Purpose: Attach servo to a GPIO pin
    // Description: Initializes the servo on the specified pin
    // Args: pin - GPIO pin number
    // Return: void
    void attach(int pin);
    
    // Purpose: Detach servo from pin (disable control)
    // Description: Releases the servo, allowing manual movement
    // Args: None
    // Return: void
    void detach();
    
    // Purpose: Check if servo is attached
    // Description: Returns attachment status
    // Args: None
    // Return: bool - true if attached, false otherwise
    bool is_attached();
    
    // Purpose: Set servo speed for smooth movements
    // Description: Controls how fast servo moves between positions
    // Args: degrees_per_second - speed in degrees/second (1-180)
    // Return: void
    void set_speed(int degrees_per_second);
    
    // Purpose: Get current servo speed setting
    // Description: Returns the speed in degrees/second
    // Args: None
    // Return: int - speed in degrees/second
    int get_speed();
    
    // ========== POSITION CONTROL ==========
    
    // Purpose: Set servo to a specific angle instantly
    // Description: Commands servo to target angle without smooth transition
    // Args: angle - target angle in degrees (0-180)
    // Return: void
    void set_angle(int angle);
    
    // Purpose: Get current servo angle
    // Description: Returns the last commanded angle
    // Args: None
    // Return: int - current angle in degrees
    int get_angle();
    
    // Purpose: Move servo to angle with smooth transition
    // Description: Gradually moves servo to target angle at set speed
    //   Blocks until movement is complete
    // Args: target_angle - target angle in degrees (0-180)
    // Return: void
    void move_to_angle(int target_angle);
    
    // Purpose: Move servo to center position (90°)
    // Description: Returns servo to center with smooth transition
    // Args: None
    // Return: void
    void center();
    
    // Purpose: Move servo to minimum position (0°)
    // Description: Moves servo to leftmost position
    // Args: None
    // Return: void
    void move_to_min();
    
    // Purpose: Move servo to maximum position (180°)
    // Description: Moves servo to rightmost position
    // Args: None
    // Return: void
    void move_to_max();
    
    // ========== SWEEP FUNCTIONS ==========
    
    // Purpose: Sweep servo back and forth between two angles
    // Description: Continuously moves between min and max angles
    // Args: min_angle - minimum angle for sweep (0-180)
    //       max_angle - maximum angle for sweep (0-180)
    //       num_sweeps - number of complete back-and-forth cycles
    // Return: void
    void sweep(int min_angle, int max_angle, int num_sweeps);
    
    // Purpose: Sweep servo across full range
    // Description: Sweeps from 0° to 180° and back
    // Args: num_sweeps - number of complete cycles
    // Return: void
    void sweep_full_range(int num_sweeps);
    
    // ========== UTILITY FUNCTIONS ==========
    
    // Purpose: Validate angle is within allowed range
    // Description: Checks if angle is between MIN and MAX
    // Args: angle - angle to validate
    // Return: bool - true if valid, false otherwise
    bool is_valid_angle(int angle);
    
    // Purpose: Constrain angle to valid range
    // Description: Clamps angle between MIN and MAX
    // Args: angle - angle to constrain
    // Return: int - constrained angle
    int constrain_angle(int angle);
    
  private:
    Servo servo;                  // Arduino Servo object
    int current_angle;            // Current servo position in degrees
    int pin;                      // GPIO pin for servo
    int speed_degrees_per_sec;    // Speed for smooth movements
    bool attached;                // Attachment status
    
    // Purpose: Calculate delay between steps for smooth movement
    // Description: Computes step delay based on speed setting
    // Args: None
    // Return: int - delay in milliseconds
    int calculate_step_delay();
};

#endif
