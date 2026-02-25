#ifndef navigator_h
#define navigator_h

#include <Pololu3piPlus32U4.h>
#include <stdint.h>
#include "../odometer/odometry.h"

using namespace Pololu3piPlus32U4;

class Navigator{
public:
  Navigator();

  void update();

  float getX() const;
  float getY() const;
  float getTheta() const;
  int getTotalLeftEncoderCount() const;
  int getTotalRightEncoderCount() const;

private:
  Odometry odometry;
  Encoders encoder;

  float x = 0.0f;
  float y = 0.0f;
  float theta = 0.0f;

  int totalLeftCounts = 0;
  int totalRightCounts = 0;
};

#endif
