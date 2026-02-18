#include "logger.h"

#if defined(__AVR_ATmega32U4__)
#include <avr/io.h>
#endif

#undef CLASS_NAME
#define CLASS_NAME "Logger"

LogLevel Logger::current_level = DEFAULT_LOG_LEVEL;
unsigned long Logger::baud_rate_ = DEFAULT_BAUD_RATE;
bool Logger::serial_enabled_ = false;

static bool usb_power_present_() {
#if defined(__AVR_ATmega32U4__)
  // Matches Pololu3piPlus32U4::usbPowerPresent(): USBSTA >> VBUS & 1
  return ((USBSTA >> VBUS) & 1) != 0;
#else
  // On non-32U4 targets, assume serial is available.
  return true;
#endif
}

// Singleton instance
Logger& Logger::instance() {
  static Logger instance;
  return instance;
}

// Private constructor
Logger::Logger() {
  // Constructor body (if needed in future)
}

void Logger::init(unsigned long baud_rate) {
  // Non-blocking: only start Serial if the robot has USB power.
  if (!usb_power_present_()) {
    serial_enabled_ = false;
    return;
  }

  // Avoid reinitializing if we've already started it.
  if (!serial_enabled_) {
    Serial.begin(baud_rate);
    serial_enabled_ = true;
  }
}

// Non-static configure() override from Configurable
void Logger::configure() {
  configure(baud_rate_, current_level);
}

// Static configure() with parameters
void Logger::configure(unsigned long baud_rate, LogLevel level) {
  baud_rate_ = baud_rate;
  init(baud_rate);
  set_log_level(level);
  log_info(CLASS_NAME, __FUNCTION__, "Logger configured");
}

const char* Logger::level_to_string(LogLevel level) {
  switch (level) {
    case LogLevel::DEBUG:
      return "DEBUG";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::ERROR:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}

void Logger::log(LogLevel level, const char* class_name, const char* function_name, const char* message) {
  if (level >= current_level) {
    // If we booted without USB, allow logging to start later
    // when USB is plugged in (without blocking).
    if (!serial_enabled_) {
      init(baud_rate_);
      if (!serial_enabled_) {
        return;
      }
    }

    Serial.print("[");
    Serial.print(level_to_string(level));
    Serial.print("] ");
    Serial.print(class_name);
    Serial.print("::");
    Serial.print(function_name);
    Serial.print("() - ");
    Serial.println(message);
    Serial.flush();
  }
}

void Logger::log_debug(const char* class_name, const char* function_name, const char* message) {
  log(LogLevel::DEBUG, class_name, function_name, message);
}

void Logger::log_info(const char* class_name, const char* function_name, const char* message) {
  log(LogLevel::INFO, class_name, function_name, message);
}

void Logger::log_warning(const char* class_name, const char* function_name, const char* message) {
  log(LogLevel::WARNING, class_name, function_name, message);
}

void Logger::log_error(const char* class_name, const char* function_name, const char* message) {
  log(LogLevel::ERROR, class_name, function_name, message);
}

void Logger::set_log_level(LogLevel level) {
  current_level = level;
}
