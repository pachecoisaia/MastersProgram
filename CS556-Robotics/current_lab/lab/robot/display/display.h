#ifndef display_h
#define display_h

#include <Pololu3piPlus32U4.h>
#include <stdint.h>
using namespace Pololu3piPlus32U4;

// Update periods (ms)
const uint16_t DISPLAY_UPDATE_MS = 50;

class Display {
public:
  Display();

  void clear();
  void print_encoder(int64_t left, int64_t right);
  void print_odom(float x, float y, float theta);
  void print_odom_and_encoder(float x, float y, float theta, int64_t left, int64_t right);

private:
  void ensure_oled_ready();

  OLED oled;
  uint16_t lastUpdateTimeMs;
  bool isDisplayReady;
};

#endif
