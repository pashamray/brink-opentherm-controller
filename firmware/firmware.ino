void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    Serial.println(command);
  }
}