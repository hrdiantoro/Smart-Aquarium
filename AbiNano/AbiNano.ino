#include "Header.h"  // new bootloader

void setup() {
  usbSerial.begin(&Serial, 9600);
  comSerial.begin(2, 3, 9600);
  servo.attach(9);

  sensor.addModule("mq", []() -> BaseSens* {
    auto mq = new AnalogSens(A0, 5.0, 1023.0, [](JsonVariant value, int analogValue, float voltage) {
      value["val"] = voltage > 1.0 ? "Tinggi" : "Normal";
    });
    return mq;
  });
  sensor.addModule("ph", []() -> BaseSens* {
    auto ph = new AnalogSens(A1, 5.0, 1023.0, [](JsonVariant value, int analogValue, float voltage) {
      phFilterVoltage.addMeasurement(voltage);
      float averageVoltage = phFilterVoltage.getFilteredValue();
      float phValue = constrain(mapFloat(averageVoltage, 4.20, 4.05, 3.0, 8.0), 3.0 + float(random(-3, 3) * 0.01f), 8.0 + float(random(-3, 3) * 0.01f));
      phFilter.addMeasurement(phValue);
      float averagepH = phFilter.getFilteredValue();
      value["val"] = phValue;
      value["avg"] = averagepH;
    });
    return ph;
  });
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
    if (var.debugLevel == 1) {
      sensor.debug(500, true);
    }
  });

  comSerial.clearData();
  comSerial.addData(sensor["mq"]["volt"].as<float>());
  comSerial.addData(sensor["ph"]["val"].as<float>());
  comSerial.addData(sensor["turbid"]["volt"].as<float>());
  comSerial.sendDataAsyncCb(1000, [](const String& sendData) {
    if (var.debugLevel == 2) {
      Serial.println(sendData);
    }
  });

  comSerial.receive(comCommunicationTask);
  usbSerial.receive(usbCommunicationTask);
}
