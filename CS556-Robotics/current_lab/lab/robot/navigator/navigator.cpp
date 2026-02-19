#include "navigator.h"

#include <Arduino.h>
#include <stdio.h>

// Per Pololu docs, include this header in a cpp/ino to enable IMU functionality.
#include <Pololu3piPlus32U4IMU.h>

#include "../utils/logger.h"

#undef CLASS_NAME
#define CLASS_NAME "Navigator"

// ============================================================
// NAVIGATOR (HARDWARE-FACING POSITION TRACKING)
// ============================================================
// Responsibilities:
//   - Read encoder count deltas from hardware
//   - (Optional) Read gyro z-rate from IMU and calibrate bias
//   - Delegate position integration to Odometry

// ========== CONSTRUCTION ==========

Navigator::Navigator()
  : odometry() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Initialized");
}

// ========== CONFIGURATION ==========

void Navigator::configure() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Configuring navigator");

  // ----- Encoders -----
  // Ensure encoder interrupts are initialized.
  Encoders::init();

  // Clear any pending deltas so the first update starts clean.
  Encoders::getCountsAndResetLeft();
  Encoders::getCountsAndResetRight();

#if USE_IMU
  // ----- IMU -----
  // Initialize the I2C bus and IMU, then calibrate gyro bias while stationary.
  Wire.begin();
  imuEnabled = imu.init();
  if (imuEnabled) {
    imu.configureForTurnSensing();

    Logger::log_info(CLASS_NAME, __FUNCTION__, "IMU enabled; calibrating gyro bias");

    long sumZ = 0;
    for (int i = 0; i < (int)IMU_CAL_SAMPLES; ++i) {
      imu.readGyro();
      sumZ += (long)imu.g.z;
      delay(IMU_CAL_DELAY_MS);
    }

    float avgRawZ = (float)sumZ / (float)IMU_CAL_SAMPLES;
    gyroZBiasDegPerSec = avgRawZ * (float)GYRO_SENS_DPS_PER_LSB;

    char msg[96];
    snprintf(msg, sizeof(msg), "Gyro Z bias calibrated: %.4f deg/s", (double)gyroZBiasDegPerSec);
    Logger::log_info(CLASS_NAME, __FUNCTION__, msg);

    lastImuMicros = micros();
  } else {
    Logger::log_warning(CLASS_NAME, __FUNCTION__, "IMU init failed; falling back to encoders only");
  }
#endif

  // ----- Odometry -----

  odometry.configure();

  Logger::log_info(CLASS_NAME, __FUNCTION__, "Configuration complete");
}

// ========== UPDATE ==========

void Navigator::update() {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Updating position");

  // Grab per-interval deltas and reset hardware counters to avoid double-counting across updates.
  int16_t deltaLeft = Encoders::getCountsAndResetLeft();
  int16_t deltaRight = Encoders::getCountsAndResetRight();

  // Maintain cumulative totals for logging/telemetry.
  encCountsLeft += deltaLeft;
  encCountsRight += deltaRight;

  // ----- Position integration -----
  // Odometry consumes encoder count deltas since last update.

  // Use IMU if enabled and calibrated, use gyro-assisted odometry for better heading accuracy.
#if USE_IMU
  if (imuEnabled) {
    unsigned long now = micros();
    float dt = (now - lastImuMicros) * 1e-6f;
    lastImuMicros = now;

    if (dt <= 0.0f) {
      dt = 1e-3f;
    }

    imu.readGyro();
    float gyroZDegPerSec = (float)imu.g.z * (float)GYRO_SENS_DPS_PER_LSB;
    gyroZDegPerSec -= gyroZBiasDegPerSec;

    odometry.update_odom_imu(static_cast<int32_t>(deltaLeft),
                static_cast<int32_t>(deltaRight),
                            gyroZDegPerSec,
                            dt,
                            x,
                            y,
                            theta);
    return;
  }
#endif

// Use encoder-only odometry if IMU is not enabled or failed to initialize.
  odometry.update_odom(static_cast<int32_t>(deltaLeft),
                       static_cast<int32_t>(deltaRight),
                       x,
                       y,
                       theta);
  
}

float Navigator::getX() const { return x; }
float Navigator::getY() const { return y; }
float Navigator::getTheta() const { return theta; }

int64_t Navigator::getLeftEncoderCount() const { return encCountsLeft; }
int64_t Navigator::getRightEncoderCount() const { return encCountsRight; }

int16_t Navigator::readLeftEncoderRaw() const { return Encoders::getCountsLeft(); }
int16_t Navigator::readRightEncoderRaw() const { return Encoders::getCountsRight(); }

void Navigator::reset() {
  encCountsLeft = 0;
  encCountsRight = 0;
  x=0.0f;
  y=0.0f;
  theta=0.0f;

  // Clear any deltas that might have accumulated in hardware since the last update.
  Encoders::getCountsAndResetLeft();
  Encoders::getCountsAndResetRight();
}