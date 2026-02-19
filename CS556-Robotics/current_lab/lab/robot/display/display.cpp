#include <Pololu3piPlus32U4.h>
#include "display.h"

#include "../utils/logger.h"
#include "../utils/util.h"
#include <stdio.h>
#include <stdint.h>
using namespace Pololu3piPlus32U4;

#undef CLASS_NAME
#define CLASS_NAME "Display"

Display::Display() {
  lastUpdateTimeMs = static_cast<uint16_t>(millis()) - 100; // allow immediate first update
  isDisplayReady = false;
}

// Local helpers to keep OLED formatting consistent and avoid Print overload surprises.
static void print_float_line(OLED &oled, uint8_t row, const char *label, float value, uint8_t precision) {
  char buf[16];
  dtostrf(value, 0, precision, buf);
  oled.gotoXY(0, row);
  oled.print(label);
  oled.print(buf);
}

static void print_int64_line(OLED &oled, uint8_t row, const char *label, int64_t value) {
  char buf[20];
  // AVR snprintf has limited long long support; downcast to long for reliable rendering.
  snprintf(buf, sizeof(buf), "%ld", (long)value);
  oled.gotoXY(0, row);
  oled.print(label);
  oled.print(buf);
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

void Display::print_encoder(int64_t left, int64_t right) {
  ensure_oled_ready();

  if ((uint16_t)(millis() - lastUpdateTimeMs) <= DISPLAY_UPDATE_MS) {
    return;
  }
  lastUpdateTimeMs = static_cast<uint16_t>(millis());

  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Updated OLED (encoder)");


  oled.clear();

  print_int64_line(oled, 0, "L: ", left);
  print_int64_line(oled, 1, "R: ", right);

}

void Display::print_odom(float x, float y, float theta) {
  ensure_oled_ready();

  if ((uint16_t)(millis() - lastUpdateTimeMs) <= DISPLAY_UPDATE_MS) {
    return;
  }

  lastUpdateTimeMs = static_cast<uint16_t>(millis());

  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Updated OLED (odom)");

  oled.clear();

  constexpr uint8_t kPrecision = 2;

  // Keep OLED consistent with serial: theta shown in degrees.
  float thetaDeg = radians_to_degrees(theta);

  print_float_line(oled, 0, "x: ", x, kPrecision);
  print_float_line(oled, 1, "y: ", y, kPrecision);
  print_float_line(oled, 2, "theta: ", thetaDeg, kPrecision);
}

void Display::print_odom_and_encoder(float x, float y, float theta, int64_t left, int64_t right) {
  ensure_oled_ready();

  if ((uint16_t)(millis() - lastUpdateTimeMs) <= DISPLAY_UPDATE_MS) {
    return;
  }

  lastUpdateTimeMs = static_cast<uint16_t>(millis());

  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Updated OLED (odom+encoder)");

  oled.clear();

  constexpr uint8_t kPrecision = 2;

  float thetaDeg = radians_to_degrees(theta);

  print_float_line(oled, 0, "x: ", x, kPrecision);
  print_float_line(oled, 1, "y: ", y, kPrecision);
  print_float_line(oled, 2, "theta: ", thetaDeg, kPrecision);

  // Encoders can exceed 32-bit; render as int64 via snprintf to avoid base misinterpretation.
  print_int64_line(oled, 3, "L: ", left);
  print_int64_line(oled, 4, "R: ", right);
}
