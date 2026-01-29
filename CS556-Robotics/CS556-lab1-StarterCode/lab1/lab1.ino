#include <Pololu3piPlus32U4.h>
#include "robot/robot.h"
#include "robot/utils/logger.h"
using namespace Pololu3piPlus32U4;

// Test parameters
const float SPEED = 0.1;        // 0.1 m/s
const float DURATION = 0.94;    // 0.94 seconds
const float DISTANCE = 1.0;     // 1 meter

// Initialize robot instance
Robot robot = Robot();

void setup() {
  // Initialize Logger
  Logger::init(9600);
  Logger::set_log_level(LogLevel::INFO);
  delay(1000);
  
  // Initialize robot drive system
  if (robot.drive == nullptr) {
    Logger::log_error(__FUNCTION__, "Robot drive system not initialized!");
    while(1);  // Halt if initialization failed
  }
  
  Logger::log_info(__FUNCTION__, "=== Configuring Robot Drivetrain ===");
  Logger::log_info(__FUNCTION__, "Robot initialized successfully");
  
  // ========== DRIVETRAIN CONFIGURATION EXAMPLES ==========
  
  // Example 1: Set turn speed ratio (inner wheel speed during curved motion)
  // Range: 0.0 to 1.0, where 1.0 = full speed, 0.0 = no movement
  // Default is 0.5 (inner wheel runs at 50% of outer wheel speed)
  Logger::log_info(__FUNCTION__, "Configuring turn speed ratio to 0.5");
  robot.drive->set_turn_speed_ratio(0.5);
  float current_ratio = robot.drive->get_turn_speed_ratio();
  Logger::log_info(__FUNCTION__, "Current turn speed ratio: " + String(current_ratio));
  
  // Example 2: Set wheelbase (distance between left and right wheels)
  // Default is 96mm for Pololu 3pi+
  // Adjust this for calibration if rotation angles are inaccurate
  Logger::log_info(__FUNCTION__, "Configuring wheelbase to 96.0 mm");
  robot.drive->set_wheelbase(96.0);
  float current_wheelbase = robot.drive->get_wheelbase();
  Logger::log_info(__FUNCTION__, "Current wheelbase: " + String(current_wheelbase) + " mm");
  
  // Example 3: Flip motor directions if wired backwards
  // Set to true if a motor spins in the wrong direction
  Logger::log_info(__FUNCTION__, "Checking motor directions (not flipping)");
  robot.drive->flip_left_motor(false);   // false = normal direction
  robot.drive->flip_right_motor(false);  // false = normal direction
  Logger::log_info(__FUNCTION__, "Left motor: normal direction");
  Logger::log_info(__FUNCTION__, "Right motor: normal direction");
  
  // Optional: If one motor is wired backwards, uncomment one of these:
  // robot.drive->flip_left_motor(true);   // Flip left motor direction
  // robot.drive->flip_right_motor(true);  // Flip right motor direction
  
  Logger::log_info(__FUNCTION__, "=== Configuration Complete ===");
  Logger::log_info(__FUNCTION__, "Test Parameters: Speed=0.1m/s, Duration=0.94s, Distance=1m");
  
  delay(2000);  // Give time to read
}

void loop(){
    // Test 1: Move forward
    Logger::log_info(__FUNCTION__, "1. Moving forward 1 meter at 0.1 m/s");
    robot.drive->move_forward(DISTANCE, SPEED);
    delay(2000);
    
    // Test 2: Move backwards
    Logger::log_info(__FUNCTION__, "2. Moving backward 1 meter at 0.1 m/s");
    robot.drive->move_backward(DISTANCE, SPEED);
    delay(2000);
    
    // Test 3: Turn right (in place) - using duration
    Logger::log_info(__FUNCTION__, "3. Turning right for 0.94 seconds at 0.1 m/s");
    robot.drive->turn_right(DURATION, SPEED, TurnMode::DURATION);
    delay(2000);
    
    // Test 4: Turn left (in place) - using duration
    Logger::log_info(__FUNCTION__, "4. Turning left for 0.94 seconds at 0.1 m/s");
    robot.drive->turn_left(DURATION, SPEED, TurnMode::DURATION);
    delay(2000);
    
    // Test 5: Move forward while turning right
    Logger::log_info(__FUNCTION__, "5. Moving forward while turning right for 1 meter at 0.1 m/s");
    robot.drive->move_forward_turning_right(DISTANCE, SPEED);
    delay(2000);
    
    // Test 6: Move forward while turning left
    Logger::log_info(__FUNCTION__, "6. Moving forward while turning left for 1 meter at 0.1 m/s");
    robot.drive->move_forward_turning_left(DISTANCE, SPEED);
    delay(2000);
    
    // Test 7: Move backward while turning left
    Logger::log_info(__FUNCTION__, "7. Moving backward while turning left for 1 meter at 0.1 m/s");
    robot.drive->move_backward_turning_left(DISTANCE, SPEED);
    delay(2000);
    
    // Test 8: Move backward while turning right
    Logger::log_info(__FUNCTION__, "8. Moving backward while turning right for 1 meter at 0.1 m/s");
    robot.drive->move_backward_turning_right(DISTANCE, SPEED);
    delay(2000);
    
    // Halt at the end
    Logger::log_info(__FUNCTION__, "All tests complete. Halting.");
    robot.drive->halt();
    
    // Wait before repeating
    delay(5000);
    Logger::log_info(__FUNCTION__, "=== Repeating tests ===");
}
