#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_DS18B20

#include "Kinematrix.h"

SensorModule sensor;
MovingAverageFilter temperatureFilter(30);

float temperature;

void setup() {
  Serial.begin(115200);
  sensor.addModule("temp", new DS18B20Sens(14));
  sensor.init();
}

void loop() {
  sensor.update([]() {
    static uint32_t temperatureAverageTimer;
    if (millis() - temperatureAverageTimer >= 50) {
      temperature = sensor["temp"];
      temperatureFilter.addMeasurement(temperature);
      temperature = temperatureFilter.getFilteredValue();
      temperature = temperature < 0 ? 0 : temperature;
      temperatureAverageTimer = millis();
    }
  });
}
