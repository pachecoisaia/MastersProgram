#include "navigator.h"

#include <Arduino.h>
#include <stdio.h>

#include <Pololu3piPlus32U4IMU.h>

#include "../utils/logger.h"

#undef CLASS_NAME
#define CLASS_NAME "Navigator"


Navigator::Navigator()
  : odometry() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Initialized");

  Encoders::init();
  Encoders::getCountsAndResetLeft();
  Encoders::getCountsAndResetRight();

  x=0.0f;
  y=0.0f;
  theta=0.0f;
}

void Navigator::update() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Updating position");

  int16_t encoderLeft =  Encoders::getCountsLeft();
  int16_t encoderRight = Encoders::getCountsRight();

  odometry.update_odom(static_cast<int32_t>(encoderLeft),
                       static_cast<int32_t>(encoderRight),
                       x,
                       y,
                       theta);
  
}

float Navigator::getX() const { return x; }
float Navigator::getY() const { return y; }
float Navigator::getTheta() const { return theta; }

int Navigator::getLeftEncoderCount() const {
  return Encoders::getCountsLeft();
}
int Navigator::getRightEncoderCount() const {
  return Encoders::getCountsRight();
}
