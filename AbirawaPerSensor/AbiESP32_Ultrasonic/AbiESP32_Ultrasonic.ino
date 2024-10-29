#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_ULTRASONIC

#include "Kinematrix.h"

SensorModule sensor;
MovingAverageFilter heightFilter(30);

float height;

void setup() {
  Serial.begin(115200);
  sensor.addModule("sonar", new UltrasonicSens(12, 13));
  sensor.init();
}

void loop() {
  sensor.update([]() {
    static uint32_t heightAverageTimer;
    if (millis() - heightAverageTimer >= 100) {
      height = sensor["sonar"];
      height = 27 - height;
      heightFilter.addMeasurement(height);
      height = round(heightFilter.getFilteredValue());
      height = height < 0 ? 0 : height;
      heightAverageTimer = millis();
    }
  });
}
