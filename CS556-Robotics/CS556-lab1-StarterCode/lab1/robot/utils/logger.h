#ifndef logger_h
#define logger_h
#include <Arduino.h>
#include "../configurable.h"

enum class LogLevel {
  DEBUG = 0,
  INFO = 1,
  WARNING = 2,
  ERROR = 3
};

// Default configuration constants for Logger
const unsigned long DEFAULT_BAUD_RATE = 9600;
const LogLevel DEFAULT_LOG_LEVEL = LogLevel::INFO;

class Logger : public Configurable {
  public:
    // Singleton instance access
    static Logger& instance();
    
    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    static void init(unsigned long baud_rate = DEFAULT_BAUD_RATE);
    void configure() override;  // Non-static override from Configurable
    static void configure(unsigned long baud_rate = DEFAULT_BAUD_RATE, LogLevel level = DEFAULT_LOG_LEVEL);  // Static convenience method
    
    // Core logging function with timestamp and location info
    static void log(LogLevel level, const char* class_name, const char* function_name, const char* message);
    
    // Convenience functions
    static void log_debug(const char* class_name, const char* function_name, const char* message);
    static void log_info(const char* class_name, const char* function_name, const char* message);
    static void log_warning(const char* class_name, const char* function_name, const char* message);
    static void log_error(const char* class_name, const char* function_name, const char* message);
    
    static void set_log_level(LogLevel level);
    
  private:
    Logger();  // Private constructor for singleton
    static LogLevel current_level;
    static unsigned long baud_rate_;
    static const char* level_to_string(LogLevel level);
    static unsigned long get_timestamp_ms();
};

// Convenience macros for logging with class and function name
#define LOG_DEBUG(msg) Logger::log_debug(this->class_name, __FUNCTION__, msg)
#define LOG_INFO(msg) Logger::log_info(this->class_name, __FUNCTION__, msg)
#define LOG_WARNING(msg) Logger::log_warning(this->class_name, __FUNCTION__, msg)
#define LOG_ERROR(msg) Logger::log_error(this->class_name, __FUNCTION__, msg)

#endif
