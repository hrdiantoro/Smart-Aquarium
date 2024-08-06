void usbCommunicationTask(const String& dataRecv) {
  String data = dataRecv;
  String dataHeader = usbSerial.getStrData(data, 0, "#");
  String dataValue = usbSerial.getStrData(data, 1, "#");
  Serial.print(F("| data: "));
  Serial.print(data);
  Serial.print(F("| dataHeader: "));
  Serial.print(dataHeader);
  Serial.print(F("| dataValue: "));
  Serial.print(dataValue);
  if (isDigit(data[0]) || isDigit(data[1])) {
    // nums
  } else {
    dataHeader.toUpperCase();
    if (dataHeader == "R") ESP.restart();
    if (dataHeader == "D") {  // D#0
      var.debugLevel = dataValue.toInt();
      Serial.print(F("| var.debugLevel: "));
      Serial.print(var.debugLevel);
    }
    if (dataHeader == "FIREBASE") {
      var.firebaseEnable = !var.firebaseEnable;
      Serial.print("| var.firebaseEnable: ");
      Serial.print(var.firebaseEnable);
      Serial.println();
    }
    if (dataHeader == "SYSENA") {
      var.systemEnable = !var.systemEnable;
      Serial.print("| var.systemEnable: ");
      Serial.print(var.systemEnable);
      Serial.println();
    }

    if (dataHeader == "TEMP") var.temperature = dataValue.toFloat();
    if (dataHeader == "PEHA") var.ph = dataValue.toFloat();         // PEHA#6.83  |  8.86, 6.83, 3.45
    if (dataHeader == "TURB") var.turbidity = dataValue.toFloat();  // TURB#1.50  |  < 0.75 == Tinggi
    if (dataHeader == "AMON") var.amonia = dataValue.toFloat();     // AMON#0.54  |  > 1.0 == Tinggi
    if (dataHeader == "HEIG") var.height = dataValue.toFloat();

    if (dataHeader == "RELA") relayPompaKuras.getState() ? relayPompaKuras.off() : relayPompaKuras.on();
    if (dataHeader == "RELB") relayPompaIsi.getState() ? relayPompaIsi.off() : relayPompaIsi.on();
    if (dataHeader == "RELC") relayHeater.getState() ? relayHeater.off() : relayHeater.on();
    if (dataHeader == "RELD") relayD.getState() ? relayD.off() : relayD.on();
  }
  Serial.print(F("| ESP.getFreeHeap(): "));
  Serial.print(ESP.getFreeHeap());
  Serial.println();
}

void comCommunicationTask(const String& dataRecv) {
  if (var.debugLevel == 3) Serial.println(dataRecv);
  float valueAmonia = comSerial.getStrData(dataRecv, 0, ";").toFloat();
  float valuePh = comSerial.getStrData(dataRecv, 1, ";").toFloat();
  float valueTurbidity = comSerial.getStrData(dataRecv, 2, ";").toFloat();

  var.amonia = valueAmonia;
  var.ph = valuePh;
  var.turbidity = valueTurbidity;
}