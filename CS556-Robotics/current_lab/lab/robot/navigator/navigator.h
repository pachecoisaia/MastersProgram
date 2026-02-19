#ifndef navigator_h
#define navigator_h

#include <Pololu3piPlus32U4.h>
#include <stdint.h>

#include "../configurable.h"
#include "../odometer/odometry.h"

using namespace Pololu3piPlus32U4;

// Enable IMU-assisted dead reckoning in Navigator.
// When enabled, Navigator will read gyro z-rate and call Odometry::update_odom_imu.
#ifndef USE_IMU
#define USE_IMU 0
#endif

// Gyro sensitivity conversion from raw LSB to deg/s.
// If you use IMU::configureForTurnSensing() (FS_G = 2000 dps), sensitivity is ~0.07 dps/LSB.
// If you use IMU::enableDefault() (FS_G = 245 dps), sensitivity is ~0.00875 dps/LSB.
#ifndef GYRO_SENS_DPS_PER_LSB
#define GYRO_SENS_DPS_PER_LSB 0.07f  // [deg/s per LSB]
#endif

// Gyro bias calibration parameters.
#ifndef IMU_CAL_SAMPLES
#define IMU_CAL_SAMPLES 200  // [samples]
#endif

#ifndef IMU_CAL_DELAY_MS
#define IMU_CAL_DELAY_MS 2   // [ms]
#endif

class Navigator : public Configurable {
public:
  Navigator();

  void configure() override;

  // Pulls latest encoder deltas (with reset) and updates pose via odometry.
  void update();

  float getX() const;
  float getY() const;
  float getTheta() const;

  int64_t getLeftEncoderCount() const;
  int64_t getRightEncoderCount() const;

  // Raw (16-bit, can overflow) reads from encoder hardware.
  int16_t readLeftEncoderRaw() const;
  int16_t readRightEncoderRaw() const;

  // Resets the cumulative (32-bit) encoder totals and clears pending hardware deltas.
  void reset();

private:
  Odometry odometry;

#if USE_IMU
  IMU imu;
  bool imuEnabled = false;
  unsigned long lastImuMicros = 0;
  float gyroZBiasDegPerSec = 0.0f;
#endif

  int64_t encCountsLeft = 0;
  int64_t encCountsRight = 0;

  float x = 0.0f;
  float y = 0.0f;
  float theta = 0.0f;
};

#endif
