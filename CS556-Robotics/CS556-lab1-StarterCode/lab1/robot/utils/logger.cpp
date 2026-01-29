#include "logger.h"

LogLevel Logger::current_level = LogLevel::INFO;

void Logger::init(unsigned long baud_rate) {
  Serial.begin(baud_rate);
  delay(15);
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
