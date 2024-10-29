#define RELAY_POMPA_KURAS 27
#define RELAY_POMPA_ISI 26
#define RELAY_HEATER 25
#define RELAY_D 33

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_POMPA_KURAS, OUTPUT);
  pinMode(RELAY_POMPA_ISI, OUTPUT);
  pinMode(RELAY_HEATER, OUTPUT);
  pinMode(RELAY_D, OUTPUT);

  digitalWrite(RELAY_POMPA_KURAS, HIGH);
  digitalWrite(RELAY_POMPA_ISI, HIGH);
  digitalWrite(RELAY_HEATER, HIGH);
  digitalWrite(RELAY_D, HIGH);
}

void loop() {
  static uint32_t relayTimer;
  if (millis() - relayTimer >= 4000) {
    digitalWrite(RELAY_POMPA_KURAS, !digitalRead(RELAY_POMPA_KURAS));
    digitalWrite(RELAY_POMPA_ISI, !digitalRead(RELAY_POMPA_ISI));
    digitalWrite(RELAY_HEATER, !digitalRead(RELAY_HEATER));
    digitalWrite(RELAY_D, !digitalRead(RELAY_D));
    relayTimer = millis();
  }
}
