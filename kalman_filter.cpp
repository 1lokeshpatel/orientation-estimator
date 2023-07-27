#include "kalman_filter.h"

KalmanFilter::KalmanFilter(){

    // set values for tuneable constants
    Q_angle_term = 0.001;
    Q_bias_term = 0.003;
    R = 0.03;

    // reset all state variables
    angle = 0.0;
    bias = 0.0;

    P[0][0] = 0;
    P[0][1] = 0;
    P[1][0] = 0;
    P[1][1] = 0;
}

float KalmanFilter::estimate(float accelAngle, float gyroRate, float dt){
    
    // PREDICTION STEP
    // predict state variable
    rate = gyroRate - bias;
    angle += rate*dt;

    // predict state covariance matrix
    P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle_term);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias_term * dt;

    // CORRECTION STEP
    // calculate kalman gain
    float K[2]; // kalman gain
    K[0] = P[0][0] / (P[0][0] + R);
    K[1] = P[1][0] / (P[0][0] + R);
    
    // final update for state estimate based on measurement and kalman gain for next step
    angle += K[0] * (accelAngle - angle);
    bias += K[1] * (accelAngle - angle);

    // final state covariance for next time step
    float temp_P00 = P[0][0];
    float temp_P01 = P[0][1];
    P[0][0] -= K[0] * temp_P00;
    P[1][0] -= K[1] * temp_P00;
    P[0][1] -= K[0] * temp_P01;
    P[1][1] -= K[1] * temp_P01;

    return angle;
}

void KalmanFilter::setAngle(float newAngle){angle = newAngle;}