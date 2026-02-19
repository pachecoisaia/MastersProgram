#include "logger.h"

#if defined(__AVR_ATmega32U4__)
#include <avr/io.h>
#endif
#include <stdio.h>

#undef CLASS_NAME
#define CLASS_NAME "Logger"

LogLevel Logger::current_level = DEFAULT_LOG_LEVEL;
unsigned long Logger::baud_rate_ = DEFAULT_BAUD_RATE;
bool Logger::serial_enabled_ = false;
static const size_t LOG_LINE_MAX = 128;

// Blocking-but-bounded write: push the whole line in chunks based on availableForWrite().
static void write_all_serial(const char* line, size_t len) {
  size_t sent = 0;
  while (sent < len) {
    int avail = Serial.availableForWrite();
    if (avail <= 0) {
      continue;
    }
    size_t chunk = (len - sent < (size_t)avail) ? (len - sent) : (size_t)avail;
    Serial.write((const uint8_t*)(line + sent), chunk);
    sent += chunk;
  }
}

// Ensure Serial is ready; return false if USB is absent and Serial cannot start.
bool Logger::ensure_serial_ready(unsigned long baud_rate) {
  if (serial_enabled_) {
    return true;
  }
  Logger::init(baud_rate);
  return serial_enabled_;
}

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
    if (!Logger::ensure_serial_ready(baud_rate_)) {
      return;
    }

    // Format once into a buffer and send it in full (blocking until buffered by HW).
    char line[LOG_LINE_MAX];
    int written = snprintf(line, sizeof(line), "[%s] %s::%s() - %s\n",
                           level_to_string(level), class_name, function_name, message);
    size_t line_len = (written < 0) ? 0 : (written >= (int)sizeof(line) ? sizeof(line) - 1 : (size_t)written);

    write_all_serial(line, line_len);
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
