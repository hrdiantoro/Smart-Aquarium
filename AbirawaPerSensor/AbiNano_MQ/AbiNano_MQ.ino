#define ENABLE_SENSOR_MODULE
#define ENABLE_SENSOR_MODULE_UTILITY
#define ENABLE_SENSOR_ANALOG

#include "Kinematrix.h"

SensorModule sensor;

void setup() {
  Serial.begin(9600);
  sensor.addModule("mq", []() -> BaseSens* {
    auto mq = new AnalogSens(A0, 5.0, 1023.0, [](JsonVariant value, int analogValue, float voltage) {
      value["val"] = voltage > 1.0 ? "Tinggi" : "Normal";
    });
    return mq;
  });
  sensor.init();
}

void loop() {
  sensor.update([]() {
    sensor.debug(500, true);
  });
}
