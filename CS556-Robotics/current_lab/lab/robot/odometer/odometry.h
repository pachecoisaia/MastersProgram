#ifndef Odometry_h
#define Odometry_h

#include <Pololu3piPlus32U4.h>
#include <Pololu3piPlus32U4IMU.h>

// Mechanical constants for odometry calculations
constexpr float DIA_L = 3.2f;
constexpr float DIA_R = 3.2f;
constexpr int N_L = 12;
constexpr int N_R = 12;
constexpr float W = 9.6f;
constexpr int GEAR_RATIO = 75;
constexpr bool DEAD_RECKONING = false;

class Odometry {
public:
  Odometry();

  void update_odom(int32_t left_counts, int32_t right_counts, float &x, float &y, float &theta);
  void update_odom_imu(int32_t left_counts, int32_t right_counts, float &x, float &y, float &theta);

private:
  float _diaL;
  float _diaR;
  float _w;
  int _nL;
  int _nR;
  int _gearRatio;
  bool _deadreckoning;
  double _x;
  double _y;
  double _theta;
  int _left_encoder_counts_prev;
  int _right_encoder_counts_prev;
  IMU _imu;
  float _IMUavg_error;

};


#endif