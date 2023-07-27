class KalmanFilter {
    public:
        KalmanFilter();

        // the angle should be in degrees, rate should be in degrees per second, delta time should be in seconds
        float estimate(float accelAngle, float gyroRate, float dt);

        void setAngle(float newAngle);
    private:
        // Kalman Filter variables

        // process noise variance 
        float Q_angle_term;
        float Q_bias_term;

        // measurement noise variance
        float R;

        // kalman state variables
        float angle;
        float bias;

        float rate;

        // error covariance matrix
        float P[2][2];   
};