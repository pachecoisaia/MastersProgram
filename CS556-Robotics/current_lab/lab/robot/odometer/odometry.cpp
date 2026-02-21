#include "odometry.h"

#include "../utils/logger.h"
#include "../utils/util.h"

#undef CLASS_NAME
#define CLASS_NAME "Odometry"

Odometry::Odometry() {
  _diaL = DIA_L;
  _diaR = DIA_R;
  _w = W;
  _nL = N_L;
  _nR = N_R;
  _gearRatio = GEAR_RATIO;
  _deadreckoning = DEAD_RECKONING;

  _x = 0;
  _y = 0;
  _theta = 0;

  _left_encoder_counts_prev = 0;
  _right_encoder_counts_prev = 0;

  if(_deadreckoning){
    Wire.begin();
    _imu.init();
    _imu.enableDefault();

    //calibrate IMU
    int total = 0;
    for (int i = 0; i < 100; i++)
    {
      _imu.readGyro();
      total += _imu.g.z;
      delay(1);
    }
    _IMUavg_error = total / 100;  
  }
}

void Odometry::update_odom(int32_t left_counts, int32_t right_counts, float &x, float &y, float &theta) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Updating odometry");

  // Δl = π * countsL * wheelDiameterL / countsPerWheelRevL
  float delta_l = compute_delta_l((double)((left_counts - _left_encoder_counts_prev)), _diaL, _nL * _gearRatio);

  // Δr = π * countsR * wheelDiameterR  / countsPerWheelRevR
  float delta_r = compute_delta_r((double)((right_counts - _right_encoder_counts_prev)), _diaR, _nR * _gearRatio);

  _theta = (delta_r - delta_l) / _w;
  theta += _theta;

  // Δx = (Δl + Δr) / 2 * cos θ
  _x = compute_delta_x(delta_l, delta_r, theta);
  x += _x;

  // Δy = (Δl + Δr) / 2 * sin θ
  _y = compute_delta_y(delta_l, delta_r, theta);
  y += _y;

  _left_encoder_counts_prev = left_counts;
  _right_encoder_counts_prev = right_counts;
  
}

void Odometry::update_odom_imu(int32_t left_counts, int32_t right_counts, float &x, float &y, float &theta) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Updating odometry (IMU-assisted)");

  // Δl = π * countsL * wheelDiameterL / countsPerWheelRevL
  float delta_l = compute_delta_l((double)((left_counts - _left_encoder_counts_prev)), _diaL, _nL * _gearRatio);

  // Δr = π * countsR * wheelDiameterR  / countsPerWheelRevR
  float delta_r = compute_delta_r((double)((right_counts - _right_encoder_counts_prev)), _diaR, _nR * _gearRatio);

  _imu.readGyro();
  float angleRate = (_imu.g.z - _IMUavg_error);
  _theta = angleRate * 0.0001;
  theta += _theta;

  // Δx = (Δl + Δr) / 2 * cos θ
  _x = compute_delta_x(delta_l, delta_r, theta);
  x += _x;

  // Δy = (Δl + Δr) / 2 * sin θ
  _y = compute_delta_y(delta_l, delta_r, theta);
  y += _y;

  _left_encoder_counts_prev = left_counts;
  _right_encoder_counts_prev = right_counts;
}
