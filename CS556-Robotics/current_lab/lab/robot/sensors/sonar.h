#ifndef sonar_h
#define sonar_h

#include <Pololu3piPlus32U4.h>
#include "../configurable.h"
#include "../utils/logger.h"
using namespace Pololu3piPlus32U4;

// ============================================================
// SONAR SENSOR ABSTRACTION
// ============================================================
//
// Purpose: Encapsulate ultrasonic distance sensor (HC-SR04 or similar)
//
// Hardware:
//   - Ultrasonic sonar sensor (HC-SR04 compatible)
//   - Single pin interface (combined trigger/echo)
//   - Operating range: 2cm to 400cm
//   - Measurement angle: ~15 degrees cone
//
// Description:
//   An ultrasonic distance sensor that measures distance by emitting ultrasonic
//   pulses and measuring the time it takes for the echo to return. The sensor
//   uses a single GPIO pin for both triggering measurements and reading echoes.
//
// Working Principle:
//   1. Send 5μs trigger pulse (HIGH) on the pin
//   2. Sensor emits 8 ultrasonic pulses at 40kHz
//   3. Pin switched to INPUT mode to receive echo
//   4. Measure duration of HIGH pulse returned
//   5. Distance = (duration / 2) / speed_of_sound
//      where speed_of_sound ≈ 29 μs/cm (at room temperature)
//
// Mathematical Model:
//   - Sound travels at ~343 m/s (or ~0.0343 cm/μs)
//   - Round-trip time t = 2 * distance / speed
//   - Distance (cm) = t / 58 or t / 29 / 2
//
// Accuracy:
//   - Best accuracy: 10cm to 200cm range
//   - Resolution: ~0.3cm
//   - Affected by: temperature, humidity, surface angle, material
//
// This class provides a clean interface to read distance measurements
// with proper configuration and error handling.
//
// ============================================================

// Default configuration constants
const int DEFAULT_SONAR_PIN = 4;              // GPIO pin for sonar sensor
const unsigned long DEFAULT_TIMEOUT_US = 30000; // 30ms timeout (~5 meters max range)
const float MIN_VALID_DISTANCE_CM = 2.0f;     // Minimum measurable distance
const float MAX_VALID_DISTANCE_CM = 400.0f;   // Maximum measurable distance
const int DEFAULT_NUM_SAMPLES = 3;            // Number of samples for averaging

class Sonar : public Configurable {
  public:
    // Purpose: Initialize sonar sensor
    // Description: Creates instance with default pin configuration
    // Args: None
    // Return: void
    Sonar();
    
    // ========== CONFIGURATION ==========
    
    // Purpose: Configure the sonar sensor with default settings
    // Description: Applies default configuration for pin, timeout, and sampling
    // Args: None
    // Return: void
    void configure() override;
    
    // Purpose: Set the GPIO pin for sonar sensor
    // Description: Configures which pin to use for trigger/echo
    // Args: pin - GPIO pin number (0-31)
    // Return: void
    void set_pin(int pin);
    
    // Purpose: Get the current GPIO pin configuration
    // Description: Returns the pin number being used
    // Args: None
    // Return: int - pin number
    int get_pin();
    
    // Purpose: Set timeout for distance measurement
    // Description: Maximum time to wait for echo return in microseconds
    // Args: timeout_us - timeout in microseconds
    // Return: void
    void set_timeout(unsigned long timeout_us);
    
    // Purpose: Get the current timeout setting
    // Description: Returns the timeout value in microseconds
    // Args: None
    // Return: unsigned long - timeout in microseconds
    unsigned long get_timeout();
    
    // Purpose: Set number of samples for averaging
    // Description: More samples = smoother but slower readings
    // Args: num_samples - number of samples to average (1-10)
    // Return: void
    void set_num_samples(int num_samples);
    
    // ========== MEASUREMENT FUNCTIONS ==========
    
    // Purpose: Read distance from sonar sensor (single measurement)
    // Description: Performs one distance measurement and returns result in cm
    // Args: None
    // Return: float - distance in centimeters, or -1.0 if error/timeout
    float read_distance_cm();
    
    // Purpose: Read distance with averaging (multiple samples)
    // Description: Takes multiple measurements and returns the average
    //   Filters out invalid readings automatically
    // Args: None
    // Return: float - averaged distance in centimeters, or -1.0 if all samples invalid
    float read_distance_averaged_cm();
    
    // Purpose: Check if a distance reading is within valid range
    // Description: Validates if reading is between min and max measurable distance
    // Args: distance_cm - distance value to validate
    // Return: bool - true if valid, false if out of range
    bool is_valid_reading(float distance_cm);
    
  private:
    int pin;                      // GPIO pin for sonar sensor
    unsigned long timeout_us;     // Timeout for pulse measurement
    int num_samples;              // Number of samples for averaging
    
    // Purpose: Send trigger pulse to initiate measurement
    // Description: Sends 5μs HIGH pulse on the pin
    // Args: None
    // Return: void
    void trigger_measurement();
    
    // Purpose: Read echo pulse duration
    // Description: Measures the duration of the echo pulse in microseconds
    // Args: None
    // Return: unsigned long - pulse duration in microseconds, 0 if timeout
    unsigned long read_echo_duration();
    
    // Purpose: Convert pulse duration to distance
    // Description: Converts echo time to distance in centimeters
    // Args: duration_us - pulse duration in microseconds
    // Return: float - distance in centimeters
    float duration_to_distance(unsigned long duration_us);
};

#endif
