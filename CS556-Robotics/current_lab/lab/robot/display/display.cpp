#include <Pololu3piPlus32U4.h>
#include "display.h"

#include "../utils/logger.h"
#include <stdio.h>
using namespace Pololu3piPlus32U4;

#undef CLASS_NAME
#define CLASS_NAME "Display"

Display::Display() {
  lastUpdateTimeMs = static_cast<uint16_t>(millis()) - 100; // allow immediate first update
  isDisplayReady = false;
}

void Display::ensure_oled_ready() {
  if (isDisplayReady) {
    return;
  }

  Logger::log_info(CLASS_NAME, __FUNCTION__, "Initializing OLED");
  oled.reinitialize();
  oled.setLayout21x8();
  oled.clear();
  oled.display();
  isDisplayReady = true;

  Logger::log_info(CLASS_NAME, __FUNCTION__, "OLED ready");
}

void Display::clear() {
  ensure_oled_ready();
  oled.clear();
}

void Display::print_encoder(float left, float right) {
  ensure_oled_ready();

  if ((uint16_t)(millis() - lastUpdateTimeMs) <= DISPLAY_UPDATE_MS) {
    return;
  }
  lastUpdateTimeMs = static_cast<uint16_t>(millis());

  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Updated OLED (encoder)");


  oled.clear();

  constexpr int kPrecision = 2;

  oled.gotoXY(0, 0);
  oled.print("left: ");
  oled.print(left, kPrecision);
  
  oled.gotoXY(0, 1);
  oled.print("right: ");
  oled.print(right, kPrecision);

}

void Display::print_odom(float x, float y, float theta) {
  ensure_oled_ready();

  if ((uint16_t)(millis() - lastUpdateTimeMs) <= DISPLAY_UPDATE_MS) {
    return;
  }

  lastUpdateTimeMs = static_cast<uint16_t>(millis());

  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Updated OLED (odom)");

  oled.clear();

  constexpr int kPrecision = 2;

  oled.gotoXY(0, 0);
  oled.print("x: ");
  oled.print(x, kPrecision);
  
  oled.gotoXY(0, 1);
  oled.print("y: ");
  oled.print(y, kPrecision);

  oled.gotoXY(0, 2);
  oled.print("theta: ");
  oled.print(theta, kPrecision);
}

void Display::print_odom_and_encoder(float x, float y, float theta, float left, float right) {
  ensure_oled_ready();

  if ((uint16_t)(millis() - lastUpdateTimeMs) <= DISPLAY_UPDATE_MS) {
    return;
  }

  lastUpdateTimeMs = static_cast<uint16_t>(millis());

  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Updated OLED (odom+encoder)");

  oled.clear();

  constexpr int kPrecision = 2;

  oled.gotoXY(0, 0);
  oled.print("x: ");
  oled.print(x, kPrecision);

  oled.gotoXY(0, 1);
  oled.print("y: ");
  oled.print(y, kPrecision);

  oled.gotoXY(0, 2);
  oled.print("theta: ");
  oled.print(theta, kPrecision);

  oled.gotoXY(0, 3);
  oled.print("L: ");
  oled.print(left, kPrecision);

  oled.gotoXY(0, 4);
  oled.print("R: ");
  oled.print(right, kPrecision);
}
