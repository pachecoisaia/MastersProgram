#ifndef logger_h
#define logger_h
#include <Arduino.h>

enum class LogLevel {
  DEBUG = 0,
  INFO = 1,
  WARNING = 2,
  ERROR = 3
};

class Logger {
  public:
    static void init(unsigned long baud_rate = 9600);
    
    // Core logging function with timestamp and location info
    static void log(LogLevel level, const char* class_name, const char* function_name, const char* message);
    
    // Convenience functions
    static void log_debug(const char* class_name, const char* function_name, const char* message);
    static void log_info(const char* class_name, const char* function_name, const char* message);
    static void log_warning(const char* class_name, const char* function_name, const char* message);
    static void log_error(const char* class_name, const char* function_name, const char* message);
    
    static void set_log_level(LogLevel level);
    
  private:
    static LogLevel current_level;
    static const char* level_to_string(LogLevel level);
    static unsigned long get_timestamp_ms();
};

// Convenience macros for logging with class and function name
#define LOG_DEBUG(msg) Logger::log_debug(this->class_name, __FUNCTION__, msg)
#define LOG_INFO(msg) Logger::log_info(this->class_name, __FUNCTION__, msg)
#define LOG_WARNING(msg) Logger::log_warning(this->class_name, __FUNCTION__, msg)
#define LOG_ERROR(msg) Logger::log_error(this->class_name, __FUNCTION__, msg)

#endif
