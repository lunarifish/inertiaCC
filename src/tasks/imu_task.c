#include "cmsis_os.h"
#include "i2c.h"
#include "mpu6050.h"

#include <math.h>
#include <stdint.h>

#define ACCEL_SCALE 17855.0f  // Scale factor for 16g range
#define SAMPLE_RATE 1000.0f   // Sample rate in Hz
#define G 9.81                // Gravity acceleration in m/s^2
#define PI 3.14159265358979323846f

double vx = 0, vy = 0, vz = 0, v = 0;  // Velocity in each direction and total velocity
double ax, ay, az;                     // Acceleration in each direction
double ax_earth, ay_earth, az_earth;   // Acceleration in earth frame
double dt;                             // Time step
float roll, pitch;                     // Roll and pitch angles
float roll_deg, pitch_deg;             // Roll and pitch angles, range -90 to 90 degrees
uint32_t t_last;

void imuTask(void *pv_arg) {
  MPU6050_t mpu;

  while (MPU6050_Init(&hi2c1) == 1) {
    osDelay(10);
  }

  t_last = HAL_GetTick();

  while (1) {
    MPU6050_Read_All(&hi2c1, &mpu);

    // Convert raw acceleration data to g's
    ax = mpu.Accel_X_RAW / ACCEL_SCALE;
    ay = mpu.Accel_Y_RAW / ACCEL_SCALE;
    az = mpu.Accel_Z_RAW / ACCEL_SCALE;

    // Get roll and pitch angles from MPU6050 structure
    roll = mpu.KalmanAngleX;
    pitch = mpu.KalmanAngleY;

    roll_deg = mpu.KalmanAngleX;
    pitch_deg = mpu.KalmanAngleY;

    // Convert to radians
    roll *= PI / 180.0;
    pitch *= PI / 180.0;

    // Rotate acceleration vector from sensor frame to earth frame
    ax_earth = ax * cos(pitch) + ay * sin(roll) * sin(pitch) - az * cos(roll) * sin(pitch);
    ay_earth = ay * cos(roll) + az * sin(roll);
    az_earth = ax * sin(pitch) - ay * sin(roll) * cos(pitch) + az * cos(roll) * cos(pitch);

    // Subtract gravity from z-axis acceleration
    az_earth -= G;

    // Calculate time step
    dt = (double)(HAL_GetTick() - t_last) / 1000;
    t_last = HAL_GetTick();

    // Integrate acceleration to get velocity
    vx += ax_earth * dt;
    vy += ay_earth * dt;
    vz += az_earth * dt;

    // Calculate total velocity
    v = sqrt(vx * vx + vy * vy + vz * vz);

    osDelay(1);
  }
}
