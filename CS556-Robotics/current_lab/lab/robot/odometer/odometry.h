#ifndef Odometry_h
#define Odometry_h

#include "../configurable.h"

// Default odometry configuration for Lab 3
// NOTE: These are used directly in the kinematics math.
// This project drives distances/speeds in meters, so odometry geometry is in meters.

// Wheel diameters [m]
#define WHEEL_DIAMETER_L_M 0.032f
#define WHEEL_DIAMETER_R_M 0.032f

// Encoder counts per motor shaft revolution [counts/rev]
#define ENCODER_LEFT_COUNTS_PER_MOTOR_REV 12
#define ENCODER_RIGHT_COUNTS_PER_MOTOR_REV 12

// Track width (wheel center-to-center) [m]
#define TRACK_WIDTH_M 0.098f

// Gear ratio: motor revolutions per wheel revolution [dimensionless]
// (Counts per wheel revolution = counts per motor revolution * motor revs per wheel rev)
#define MOTOR_REVS_PER_WHEEL_REV 75

// Dead-reckoning / IMU calibration support.
//
// If you computed an average heading error using the IMU as a reference
// (e.g., IMU angle - wheel-odometry angle), you can apply it here to reduce
// systematic turn drift during dead reckoning.
//
// Set DEAD_RECKONING to 1 to enable the correction.
// Units: radians per update (additive correction applied to wheel-based d_theta).
// Note: This correction is only applied in update_odom() (encoder-only heading).
// If you use update_odom_imu(), gyro bias should be handled upstream (Navigator IMU calibration).
#ifndef DEAD_RECKONING
#define DEAD_RECKONING 0
#endif

#ifndef IMUAVG_ERROR
#define IMUAVG_ERROR 0.0f
#endif

// Pure kinematics odometry (no hardware I/O).
// Consumes wheel encoder count *deltas* and integrates pose.
class Odometry : public Configurable {
public:
  Odometry();
  void configure() override;

  // Update pose from encoder count deltas since the last call.
  void update_odom(int deltaLeftCounts, int deltaRightCounts, float &x, float &y, float &theta);

  // Optional IMU-assisted update.
  // gyroZDegPerSec: gyro z-rate in deg/s
  // dtSeconds: timestep in seconds
  void update_odom_imu(int deltaLeftCounts,
                       int deltaRightCounts,
                       float gyroZDegPerSec,
                       float dtSeconds,
                       float &x,
                       float &y,
                       float &theta);

private:
  float wheelDiameterLM;
  float wheelDiameterRM;
  float trackWidthM;
  int encoderLeftCountsPerMotorRev;
  int encoderRightCountsPerMotorRev;
  int motorRevsPerWheelRev;
};

#endif
