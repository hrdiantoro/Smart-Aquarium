#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_ANALOG

#include "Kinematrix.h"

SensorModule sensor;

void setup() {
  Serial.begin(9600);
  sensor.addModule("turbid", []() -> BaseSens* {
    auto turbid = new AnalogSens(A2, 5.0, 1023.0, [](JsonVariant value, int analogValue, float voltage) {
      value["val"] = voltage < 1.0 ? "Keruh" : "Bersih";
    });
    return turbid;
  });
  sensor.init();
}

void loop() {
  sensor.update([]() {
    sensor.debug(500, true);
  });
}
