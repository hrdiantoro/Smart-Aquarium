#include "Header.h"

void setup() {
  usbSerial.begin(&Serial, 115200);
  comSerial.begin(&Serial2, 9600, SERIAL_8N1, 16, 17);

  menu.initialize(true);
  menu.setLen(20, 4);

  task.initialize(wifiConTask);

  sensor.addModule("temp", new DS18B20Sens(14));
  sensor.addModule("sonar", new UltrasonicSens(12, 13));
  sensor.init();

  waterPumpTimer.setDuration(10000);
  waterPumpTimer.reset();
  waterPumpTimer.start();

  phUpDownTimer.setDuration(10000);
  phUpDownTimer.reset();
  phUpDownTimer.start();

  servoOnTimer.setDuration(3000);
  servoOnTimer.reset();
  servoOnTimer.start();

  buzzer.toggleInit(100, 5);
}

void sensorUpdate(void* pvParameter) {
  disableCore0WDT();
  for (;;) {
    task.delay(20);
  }
}

void loop() {
  sensor.update([]() {
    if (var.debugLevel == 1) sensor.debug(500, false);  // D#1
    debug();

    static uint32_t heightAverageTimer;
    if (millis() - heightAverageTimer >= 100) {
      var.height = sensor["sonar"];
      var.height = 27 - var.height;
      heightFilter.addMeasurement(var.height);
      var.height = round(heightFilter.getFilteredValue());
      var.height = var.height < 0 ? 0 : var.height;
      heightAverageTimer = millis();
    }
    var.temperature = sensor["temp"];
    var.temperature = var.temperature < 0 ? 0 : var.temperature;
    var.statusAmonia = var.amonia > var.voltageAmoniaThreshold ? "Tinggi" : "Normal";
    var.statusTurbidity = var.turbidity < var.voltageTurbidityThreshold ? "Keruh" : "Bersih";
  });

  comSerial.clearData();
  comSerial.addData(var.servoDegree);
  comSerial.sendDataAsyncCb(100, [](const String& sendData) {
    if (var.debugLevel == 2) Serial.println(sendData);
  });
  comSerial.receive(comCommunicationTask);
  usbSerial.receive(usbCommunicationTask);

  // Serial.print("| buttonDown: ");
  // Serial.print(buttonDown.getStateRaw());
  // Serial.print("| buttonOk: ");
  // Serial.print(buttonOk.getStateRaw());
  // Serial.println();

  if (buttonDown.isPressed() || buttonOk.isPressed()) {
    buzzer.on();
    buzzer.offDelay(75);
  }

  static uint32_t lcdResetTimer;

  MenuCursor cursor{
    .up = buttonOk.isPressed(),
    .down = buttonDown.isPressed(),
    .select = false,
    .back = false,
    .show = true
  };
  menu.onListen(&cursor, menuTaskCallback);

  DigitalIn::updateAll(&buttonDown, &buttonOk, DigitalIn::stop());
  DigitalOut::updateAll(&buzzer, &relayPompaKuras, &relayPompaIsi, &relayHeater, &relayD, DigitalOut::stop());

  if (!var.systemEnable) {
    relayPompaIsi.off();
    relayPompaKuras.off();
    relayHeater.off();
    return;
  }

  var.historyTemperature = var.temperature <= var.minTemperature || var.temperature >= var.maxTemperature;
  var.historyHeight = var.height <= var.minHeight || var.height >= var.maxHeight;
  var.historyAmonia = var.amonia > var.voltageAmoniaThreshold;
  var.historyPh = var.ph < var.minPh || var.ph > var.maxPh;
  var.historyTurbidity = var.turbidity < var.voltageTurbidityThreshold;

  var.waterPumpTrigger = var.turbidity < var.voltageTurbidityThreshold || var.amonia > var.voltageAmoniaThreshold;
  // var.waterPumpTrigger = var.amonia > var.voltageAmoniaThreshold;
  var.phOnOffTrigger = var.ph < var.minPh || var.ph > var.maxPh;

  if (var.waterPumpState == 0) {
    if (waterPumpTimer.isExpired()) {
      if (var.waterPumpTrigger) {
        var.waterPumpState = 1;
        buzzer.off();
        buzzer.toggleInit(100, 2);
        menu.init();
        menu.clear();
        return;
      }
    }
    /////////////////// HEIGHT ///////////////////
    // if (var.height < var.minHeight) {
    //   if (waterPumpTimer.isExpired()) {
    //     relayPompaIsi.on();
    //     relayPompaKuras.off();
    //   }
    //   if (millis() - lcdResetTimer >= 2000) {
    //     menu.init();
    //     menu.clear();
    //     lcdResetTimer = millis();
    //   }
    // } else if (var.height > var.maxHeight) {
    //   if (waterPumpTimer.isExpired()) {
    //     relayPompaIsi.off();
    //     relayPompaKuras.on();
    //   }
    //   if (millis() - lcdResetTimer >= 2000) {
    //     menu.init();
    //     menu.clear();
    //     lcdResetTimer = millis();
    //   }
    // } else {
    //   relayPompaIsi.off();
    //   relayPompaKuras.off();
    // }
    /////////////////// TEMPERATURE ///////////////////
    if (var.temperature <= var.minTemperature) {
      relayHeater.on();
    } else {
      relayHeater.off();
    }

    /////////////////// PH ///////////////////
    if (phUpDownTimer.isExpired()) {
      if (var.phOnOffTrigger && var.servoDegree == 70) {
        var.servoDegree = 180;
        servoOnTimer.reset();
        phUpDownTimer.reset();
      }
    }
    if (servoOnTimer.isExpired() && var.servoDegree == 180) {
      var.servoDegree = 70;
      servoOnTimer.reset();
      phUpDownTimer.reset();
    }

    // Serial.print("| var.servoDegree: ");
    // Serial.print(var.servoDegree);
    // Serial.print("| servoOnTimer: ");
    // Serial.print(servoOnTimer.getSeconds());
    // Serial.print("| servoOnTimer: ");
    // Serial.print(servoOnTimer.isExpired());
    // Serial.print("| phUpDownTimer: ");
    // Serial.print(phUpDownTimer.getSeconds());
    // Serial.print("| phUpDownTimer: ");
    // Serial.print(phUpDownTimer.isExpired());
    // Serial.println();

  } else if (var.waterPumpState == 1) {
    if (var.height <= var.minHeight) {  // 8
      var.waterPumpState = 2;
      relayPompaIsi.off();
      relayPompaKuras.off();
      buzzer.off();
      buzzer.toggleInit(100, 2);
      delay(5000);
      menu.init();
      menu.clear();
      return;
    }
    relayPompaIsi.off();
    relayPompaKuras.on();
    relayHeater.off();
  } else if (var.waterPumpState == 2) {
    if (var.height >= var.maxHeight) {
      var.waterPumpState = 0;
      relayPompaIsi.off();
      relayPompaKuras.off();
      buzzer.off();
      buzzer.toggleInit(100, 2);
      delay(5000);
      waterPumpTimer.setDuration(30000);
      waterPumpTimer.reset();
      waterPumpTimer.start();
      menu.init();
      menu.clear();
      return;
    }
    if (millis() - lcdResetTimer >= 4000) {
      menu.init();
      menu.clear();
      lcdResetTimer = millis();
    }
    relayPompaIsi.on();
    relayPompaKuras.off();
    relayHeater.off();
  }
}

void debug() {
  if (var.debugLevel == 4) {
    Serial.print("| ena: ");
    Serial.print(var.systemEnable);
    Serial.print("| temp: ");
    Serial.print(var.temperature);
    Serial.print("| height: ");
    Serial.print(var.height);
    Serial.print("| amon: ");
    Serial.print(var.amonia);
    Serial.print("| amonSt: ");
    Serial.print(var.amonia > var.voltageAmoniaThreshold ? "Tinggi" : "Normal");
    Serial.print("| ph: ");
    Serial.print(var.ph);
    Serial.print("| turb: ");
    Serial.print(var.turbidity);
    Serial.print("| turbSt: ");
    Serial.print(var.turbidity < var.voltageTurbidityThreshold ? "Keruh" : "Bersih");
    Serial.print("| waterTrig: ");
    Serial.print(var.waterPumpTrigger);
    Serial.print("| phTrig: ");
    Serial.print(var.phOnOffTrigger);
    Serial.print("| waterTim: ");
    Serial.print(waterPumpTimer.getSeconds());
    Serial.print("| phTime: ");
    Serial.print(phUpDownTimer.getSeconds());
    Serial.println();
  } else if (var.debugLevel == 5) {
    Serial.print("| mTemp: ");
    Serial.print(var.maxTemperature);
    Serial.print("| nTemp: ");
    Serial.print(var.minTemperature);
    Serial.print("| mHeigt: ");
    Serial.print(var.maxHeight);
    Serial.print("| nHeigt: ");
    Serial.print(var.minHeight);
    Serial.print("| mAmon: ");
    Serial.print(var.maxAmonia);
    Serial.print("| nAmon: ");
    Serial.print(var.minAmonia);
    Serial.print("| mPh: ");
    Serial.print(var.maxPh);
    Serial.print("| nPh: ");
    Serial.print(var.minPh);
    Serial.print("| mTurb: ");
    Serial.print(var.maxTurbidity);
    Serial.print("| nTurb: ");
    Serial.print(var.minTurbidity);
    Serial.println();
  } else if (var.debugLevel == 6) {
  }
}
