#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_ANALOG

#include "Kinematrix.h"

SensorModule sensor;
MovingAverageFilter phFilter(20);
MovingAverageFilter phFilterVoltage(20);

void setup() {
  Serial.begin(9600);
  sensor.addModule("ph", []() -> BaseSens* {
    auto ph = new AnalogSens(A1, 5.0, 1023.0, [](JsonVariant value, int analogValue, float voltage) {
      phFilterVoltage.addMeasurement(voltage);
      float averageVoltage = phFilterVoltage.getFilteredValue();
      float phValue = constrain(mapFloat(averageVoltage, 4.20, 4.05, 3.0, 8.0), 3.0, 8.0);
      phFilter.addMeasurement(phValue);
      float averagepH = phFilter.getFilteredValue();
      value["val"] = phValue;
      value["avg"] = averagepH;
    });
    return ph;
  });
  sensor.init();
}

void loop() {
  sensor.update([]() {
    sensor.debug(500, true);
  });
}
