#include "logger.h"

#undef CLASS_NAME
#define CLASS_NAME "Logger"

LogLevel Logger::current_level = DEFAULT_LOG_LEVEL;
unsigned long Logger::baud_rate_ = DEFAULT_BAUD_RATE;

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
  Serial.begin(baud_rate);
  while (!Serial) {}  // Wait for USB serial connection (ATmega32U4)
  delay(15);
}

// Non-static configure() override from Configurable
void Logger::configure() {
  configure(baud_rate_, current_level);
}

// Static configure() with parameters
void Logger::configure(unsigned long baud_rate, LogLevel level) {
  baud_rate_ = baud_rate;
  init(baud_rate);
  delay(1000);  // Wait for serial to be ready
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

unsigned long Logger::get_timestamp_ms() {
  return millis();
}

void Logger::log(LogLevel level, const char* class_name, const char* function_name, const char* message) {
  if (level >= current_level) {
    unsigned long timestamp = get_timestamp_ms();
    Serial.print("[");
    Serial.print(timestamp);
    Serial.print("ms] [");
    Serial.print(level_to_string(level));
    Serial.print("] ");
    Serial.print(class_name);
    Serial.print("::");
    Serial.print(function_name);
    Serial.print("() - ");
    Serial.println(message);
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
