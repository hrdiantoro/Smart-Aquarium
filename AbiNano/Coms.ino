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
    if (dataHeader == "R") {}
    if (dataHeader == "D") {  // D#0
      var.debugLevel = dataValue.toInt();
      Serial.print(F("| var.debugLevel: "));
      Serial.print(var.debugLevel);
    }
    if (dataHeader == "SERVO") {  // SERVO#0
      int degree = constrain(dataValue.toInt(), 0, 180);
      servo.write(degree);
      Serial.print(F("| degree: "));
      Serial.print(degree);
      Serial.print(F("| servo.read(): "));
      Serial.print(servo.read());
    }
  }
  Serial.print(F("| freeMemory(): "));
  Serial.print(freeMemory());
  Serial.println();
}

void comCommunicationTask(const String& dataRecv) {
  if (var.debugLevel == 3) {
    Serial.println(dataRecv);
  }
  var.servoDegree = comSerial.getStrData(dataRecv, 0, ";").toInt();
  servo.write(var.servoDegree);
}