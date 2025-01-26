unsigned int speed = 0;
unsigned int timer = 0;

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
    
    if (command.equals("ping")) {
      Serial.println("pong");
    }

    if (command.startsWith("set")) {
      if (command.startsWith("speed", 4)) {
        speed = command.substring(9).toInt();

        Serial.printf("set speed: %d", speed);
        Serial.println();
      }

      if (command.startsWith("timer", 4)) {
        timer = command.substring(9).toInt();

        Serial.printf("set timer: %d", timer);
        Serial.println();
      }
    }

    if (command.startsWith("get")) {
      if (command.startsWith("speed", 4)) {
        Serial.printf("get speed: %d", speed);
        Serial.println();
      }

      if (command.startsWith("timer", 4)) {
        Serial.printf("get timer: %d", timer);
        Serial.println();
      }
    }
  }
}