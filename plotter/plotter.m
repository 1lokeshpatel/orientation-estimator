% Specify the file path
file_path = 'data/sensorDataPitch.csv';

% Read the CSV file
data = readmatrix(file_path, 'NumHeaderLines', 1); % Skip the first row (header)

% Extract the three columns of interest
angle = data(:, 1);
accelAngle = data(:, 2);
gyroAngle = data(:, 3);

% Plot the data on one graph
plot(angle, 'DisplayName', 'Kalman Pitch');
hold on;
% plot(accelAngle, 'DisplayName', 'Accelerometer Pitch');
plot(gyroAngle, 'DisplayName', 'Gyro Pitch');
hold off;

% Add labels and legend
xlabel('Data Points');
ylabel('Pitch (degrees)');
legend('Location', 'best');
title('Comparison of Pitch between different sensors and the Kalman filter');

% Display the grid
grid on;