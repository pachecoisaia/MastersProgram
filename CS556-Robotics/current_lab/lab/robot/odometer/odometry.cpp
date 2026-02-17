#include "odometry.h"

#include "../utils/logger.h"
#include "../utils/util.h"

#undef CLASS_NAME
#define CLASS_NAME "Odometry"

// ============================================================
// ODOMETRY (PURE KINEMATICS)
// ============================================================
// Responsibilities:
//   - Integrate pose (x, y, theta) from wheel encoder count deltas
//   - Optionally integrate heading from IMU gyro z-rate (IMU-assisted)

// ========== CONSTRUCTION ==========

Odometry::Odometry() {
  wheelDiameterLM = 0.0f;
  wheelDiameterRM = 0.0f;
  trackWidthM = 0.0f;
  encoderLeftCountsPerMotorRev = 0;
  encoderRightCountsPerMotorRev = 0;
  motorRevsPerWheelRev = 0;
}

// ========== CONFIGURATION ==========

void Odometry::configure() {
  wheelDiameterLM = WHEEL_DIAMETER_L_M;
  wheelDiameterRM = WHEEL_DIAMETER_R_M;
  trackWidthM = TRACK_WIDTH_M;
  encoderLeftCountsPerMotorRev = ENCODER_LEFT_COUNTS_PER_MOTOR_REV;
  encoderRightCountsPerMotorRev = ENCODER_RIGHT_COUNTS_PER_MOTOR_REV;
  motorRevsPerWheelRev = MOTOR_REVS_PER_WHEEL_REV;
}

// ========== UPDATE (ENCODERS ONLY) ==========
// Wheel kinematics provide both translation and heading.
void Odometry::update_odom(int deltaLeftCounts, int deltaRightCounts, float &x, float &y, float &theta) {
	Logger::log_info(CLASS_NAME, __FUNCTION__, "Updating odometry");

  // Encoder count deltas since last update.
  int leftCountsDelta = deltaLeftCounts;
  int rightCountsDelta = deltaRightCounts;

  // Counts per wheel revolution = counts per motor revolution * motor revs per wheel rev.
  float leftCountsPerWheelRev = compute_ticks_per_rev(encoderLeftCountsPerMotorRev, motorRevsPerWheelRev);
  float rightCountsPerWheelRev = compute_ticks_per_rev(encoderRightCountsPerMotorRev, motorRevsPerWheelRev);

  // Δr = π * countsR * wheelDiameterR  / countsPerWheelRevR
  float delta_r = compute_delta_r(rightCountsDelta, wheelDiameterRM, rightCountsPerWheelRev);

  // Δl = π * countsL * wheelDiameterL / countsPerWheelRevL
  float delta_l = compute_delta_l(leftCountsDelta, wheelDiameterLM, leftCountsPerWheelRev);

  // θ = (Δr - Δl) / W
  float d_theta = compute_d_theta(delta_r, delta_l, trackWidthM);

#if DEAD_RECKONING
  // IMU-calibrated systematic correction (see odometry.h).
  d_theta += (float)IMUAVG_ERROR;
#endif

  theta += d_theta;

  // Δx = (Δl + Δr) / 2 * cos θ
  float delta_x = compute_delta_x(delta_l, delta_r, theta);
  x += delta_x;

  // Δy = (Δl + Δr) / 2 * sin θ
  float delta_y = compute_delta_y(delta_l, delta_r, theta);
  y += delta_y;
}

// ========== UPDATE (IMU-ASSISTED) ==========
// Wheel kinematics provide translation; gyro provides heading.

void Odometry::update_odom_imu(int deltaLeftCounts,
                              int deltaRightCounts,
                              float gyroZDegPerSec,
                              float dtSeconds,
                              float &x,
                              float &y,
                              float &theta) {
	Logger::log_info(CLASS_NAME, __FUNCTION__, "Updating odometry (IMU-assisted)");

  int leftCountsDelta = deltaLeftCounts;
  int rightCountsDelta = deltaRightCounts;

  float leftCountsPerWheelRev = compute_ticks_per_rev(encoderLeftCountsPerMotorRev, motorRevsPerWheelRev);
  float rightCountsPerWheelRev = compute_ticks_per_rev(encoderRightCountsPerMotorRev, motorRevsPerWheelRev);

  float delta_r = compute_delta_r(rightCountsDelta, wheelDiameterRM, rightCountsPerWheelRev);
  float delta_l = compute_delta_l(leftCountsDelta, wheelDiameterLM, leftCountsPerWheelRev);

  // Heading update from gyro z-rate.
  // Any gyro bias calibration is handled upstream (Navigator::configure()).
  float d_theta = compute_dtheta_imu(gyroZDegPerSec, dtSeconds);
  theta += d_theta;

  float delta_x = compute_delta_x(delta_l, delta_r, theta);
  x += delta_x;

  float delta_y = compute_delta_y(delta_l, delta_r, theta);
  y += delta_y;
}
