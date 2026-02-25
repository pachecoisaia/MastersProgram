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

void Odometry::update_odom(int left_counts, int right_counts, float &x, float &y, float &theta) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Updating odometry");

  double pi = 3.14159265358979323846;

  double delta_l = (double) ((left_counts - _left_encoder_counts_prev) * pi * _diaL) / (_nL * _gearRatio);
  double delta_r = (double) ((right_counts - _right_encoder_counts_prev) * pi * _diaR) / (_nR * _gearRatio);

  _theta += (delta_r - delta_l) / _w;
  
  _y = ((float) (float)(delta_l + delta_r)/2) * sin(_theta);
  _x = ((float) (float)(delta_l + delta_r)/2) * cos(_theta);
  

  theta = _theta;
  y += _y;
  x += _x;
  
  
  _left_encoder_counts_prev = left_counts;
  _right_encoder_counts_prev = right_counts;

}

void Odometry::update_odom_imu(int left_counts, int right_counts, float &x, float &y, float &theta) {
  Logger::log_info(CLASS_NAME, __FUNCTION__, "Updating odometry (IMU-assisted)");

  double pi = 3.14159265358979323846;

  double delta_l = (double) ((left_counts - _left_encoder_counts_prev) * pi * _diaL) / (_nL * _gearRatio);
  double delta_r = (double) ((right_counts - _right_encoder_counts_prev) * pi * _diaR) / (_nR * _gearRatio);


  _imu.readGyro();
  float angleRate = (_imu.g.z - _IMUavg_error);
  _theta += angleRate * 0.0001;

  _y = ((float) (float)(delta_l + delta_r) /2) * sin(_theta);
  _x = ((float) (float)(delta_l + delta_r) /2) * cos(_theta);

  theta = _theta;
  y += _y;
  x += _x;

  _left_encoder_counts_prev = left_counts;
  _right_encoder_counts_prev = right_counts;
}
