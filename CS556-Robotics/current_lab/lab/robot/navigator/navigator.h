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
  int getLeftEncoderCount() const;
  int getRightEncoderCount() const;

private:
  Odometry odometry;

  float x = 0.0f;
  float y = 0.0f;
  float theta = 0.0f;
};

#endif
