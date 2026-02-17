#include "sonar.h"
#include "../utils/logger.h"
#include "../utils/util.h"

#undef CLASS_NAME
#define CLASS_NAME "Sonar"

Sonar::Sonar() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Initialized");
  pin = DEFAULT_SONAR_PIN;
  timeout_us = DEFAULT_TIMEOUT_US;
  num_samples = DEFAULT_NUM_SAMPLES;
}

// ========== CONFIGURATION ==========

void Sonar::configure() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Configuring sonar sensor");
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Pin: " + String(DEFAULT_SONAR_PIN)).c_str());
  set_pin(DEFAULT_SONAR_PIN);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Timeout: " + String(DEFAULT_TIMEOUT_US) + " us").c_str());
  set_timeout(DEFAULT_TIMEOUT_US);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Samples: " + String(DEFAULT_NUM_SAMPLES)).c_str());
  set_num_samples(DEFAULT_NUM_SAMPLES);
  
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Configuration complete");
}

void Sonar::set_pin(int pin) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Setting pin to " + String(pin)).c_str());
  if (pin >= 0 && pin <= 31) {
    this->pin = pin;
  } else {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid pin number");
  }
}

int Sonar::get_pin() {
  return pin;
}

void Sonar::set_timeout(unsigned long timeout_us) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Setting timeout to " + String(timeout_us) + " us").c_str());
  this->timeout_us = timeout_us;
}

unsigned long Sonar::get_timeout() {
  return timeout_us;
}

void Sonar::set_num_samples(int num_samples) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Setting num_samples to " + String(num_samples)).c_str());
  if (num_samples >= 1 && num_samples <= 10) {
    this->num_samples = num_samples;
  } else {
    Logger::log_error(CLASS_NAME, __FUNCTION__, "Invalid num_samples (must be 1-10)");
  }
}

// ========== MEASUREMENT FUNCTIONS ==========

float Sonar::read_distance_cm() {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Reading distance");
  
  trigger_measurement();
  unsigned long duration = read_echo_duration();
  
  if (duration == 0) {
    Logger::log_warning(CLASS_NAME, __FUNCTION__, "Timeout - no echo received");
    return -1.0f;
  }
  
  float distance = duration_to_distance(duration);
  
  if (!is_valid_reading(distance)) {
    Logger::log_warning(CLASS_NAME, __FUNCTION__, ("Invalid reading: " + String(distance) + " cm").c_str());
    return -1.0f;
  }
  
  Logger::log_debug(CLASS_NAME, __FUNCTION__, ("Distance: " + String(distance) + " cm").c_str());
  return distance;
}

float Sonar::read_distance_averaged_cm() {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Reading averaged distance");
  
  float sum = 0.0f;
  int valid_count = 0;
  
  for (int i = 0; i < num_samples; i++) {
    float distance = read_distance_cm();
    if (distance > 0) {
      sum += distance;
      valid_count++;
    }
    delay(10); // Small delay between samples
  }
  
  if (valid_count == 0) {
    Logger::log_warning(CLASS_NAME, __FUNCTION__, "All samples invalid");
    return -1.0f;
  }
  
  float average = sum / valid_count;
  Logger::log_info(CLASS_NAME, __FUNCTION__, ("Averaged distance: " + String(average) + " cm (from " + String(valid_count) + " samples)").c_str());
  return average;
}

bool Sonar::is_valid_reading(float distance_cm) {
  return (distance_cm >= MIN_VALID_DISTANCE_CM && distance_cm <= MAX_VALID_DISTANCE_CM);
}

// ========== PRIVATE HELPER FUNCTIONS ==========

void Sonar::trigger_measurement() {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Triggering measurement");
  
  // Set pin to output and send trigger pulse
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
}

unsigned long Sonar::read_echo_duration() {
  Logger::log_debug(CLASS_NAME, __FUNCTION__, "Reading echo");
  
  // Set pin to input and measure pulse duration
  pinMode(pin, INPUT);
  unsigned long duration = pulseIn(pin, HIGH, timeout_us);
  
  return duration;
}

float Sonar::duration_to_distance(unsigned long duration_us) {
  // Speed of sound: ~343 m/s = 0.0343 cm/μs
  // Distance = (duration / 2) / 29.0 (where 29 ≈ 1/0.0343)
  float distance_cm = duration_us / 29.0f / 2.0f;
  return distance_cm;
}
