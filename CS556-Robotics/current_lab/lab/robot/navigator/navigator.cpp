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

  //Encoders::init();
  encoder = Encoders();
  encoder.init();

  totalLeftCounts = 0;
  totalRightCounts = 0;

  x=0.0f;
  y=0.0f;
  theta=0.0f;
}

void Navigator::update() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Updating position");

  int16_t encoderLeft =  encoder.getCountsAndResetLeft();
  int16_t encoderRight = encoder.getCountsAndResetRight();

  totalLeftCounts += encoderLeft;
  totalRightCounts += encoderRight;

  odometry.update_odom(totalLeftCounts,
                       totalRightCounts,
                       x,
                       y,
                       theta);
  
}

float Navigator::getX() const { return x; }
float Navigator::getY() const { return y; }
float Navigator::getTheta() const { return theta; }

int Navigator::getTotalLeftEncoderCount() const {
  return totalLeftCounts;
}
int Navigator::getTotalRightEncoderCount() const {
  return totalRightCounts;
}
