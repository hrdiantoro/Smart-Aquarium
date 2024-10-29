#define BUZZER_PIN 5

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN);
}

void loop() {
  static uint32_t buzzerTimer;
  if (millis() - buzzerTimer >= 1000) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(75);
    digitalWrite(BUZZER_PIN, LOW);
    buzzerTimer = millis();
  }
}
