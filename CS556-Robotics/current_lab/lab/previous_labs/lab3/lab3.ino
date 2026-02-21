#include <Pololu3piPlus32U4.h>
#include <Servo.h>
#include <Wire.h>
#include <FastGPIO.h>

#include "robot/actuators/servo_controller.h"
#include "robot/display/display.h"
#include "robot/drivetrain/differential_drive.h"
#include "robot/navigator/navigator.h"
#include "robot/navigator/navigator_tests.h"
#include "robot/odometer/odometry.h"
#include "robot/sensors/sonar.h"
#include "robot/utils/logger.h"
#include "robot/utils/util.h"
#include "robot/robot.h"
#include "robot/configurable.h"


#include "robot/actuators/servo_controller.cpp"
#include "robot/display/display.cpp"
#include "robot/drivetrain/differential_drive.cpp"
#include "robot/navigator/navigator.cpp"
#include "robot/navigator/navigator_tests.cpp"
#include "robot/odometer/odometry.cpp"
#include "robot/sensors/sonar.cpp"
#include "robot/utils/logger.cpp"
#include "robot/utils/util.cpp"
#include "robot/robot.cpp"

using namespace Pololu3piPlus32U4;

#undef CLASS_NAME
#define CLASS_NAME "lab3"

static const long BAUD_RATE = 9600;

// Initialize robot instance
Robot robot = Robot();

void setup() {
  Logger::configure(BAUD_RATE, LogLevel::INFO);
  robot.drive->configure();

  delay(20);
}

void loop() {  
  //test_2_1_test_encoders_still();
  //test_2_2a_move_forward_1m();
  //test_2_2b_move_backward_1m();
  //test_2_2c_turn_left_90();
  //test_3_2a_straight_line_15m();
  //test_3_2b_square_clockwise();
  test_3_2e_square_counter_clockwise();
}
