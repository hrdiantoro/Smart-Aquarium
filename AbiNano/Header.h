
#define ENABLE_MODULE_SERIAL_HARD
#define ENABLE_MODULE_SERIAL_SOFT

#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_ANALOG

#include "Kinematrix.h"
#include "Servo.h"

////////// Utility //////////

////////// Sensor //////////
SensorModule sensor;
MovingAverageFilter phFilter(20);
MovingAverageFilter phFilterVoltage(20);

////////// Communication //////////
HardSerial usbSerial;
SoftSerial comSerial;

////////// Input Module //////////

////////// Output Module //////////
Servo servo;

////////// Global Variable //////////
struct SystemData {
  int debugLevel = 0;
  int servoDegree = 70;
};

SystemData var;