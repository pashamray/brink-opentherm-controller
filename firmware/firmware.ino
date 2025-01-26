#include <Arduino.h>
#include <OpenTherm.h>

// State management
int desiredNomVent = 25;

unsigned int speed = 0;
unsigned int timer = 0;

#define OT_RX_PIN (21)
#define OT_TX_PIN (22)

OpenTherm ot(OT_RX_PIN, OT_TX_PIN);

void ICACHE_RAM_ATTR handleInterrupt() {
    ot.handleInterrupt();
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);

  ot.begin(handleInterrupt);

  Serial.begin(9600);
  Serial.println("Start");
  Serial.onReceive(serialReceive);
}

void get_temperatures() {
  Serial.println("Getting temperatures");
  float temperature1 = ot.getVentSupplyInTemperature();
  float temperature2 = ot.getVentSupplyOutTemperature();
  float temperature3 = ot.getVentExhaustInTemperature();
  float temperature4 = ot.getVentExhaustOutTemperature();
  Serial.print("T1:");
  Serial.println(temperature1);
  Serial.print("T2:");
  Serial.println(temperature2);
  Serial.print("T3:");
  Serial.println(temperature3);
  Serial.print("T4:");
  Serial.println(temperature4);
}

 void get_pub_diagnostic() {
   Serial.println("Geting and publishing diagnostic information");
   bool ventMode = ot.getVentilationMode();
   bool bypassStatus = ot.getBypassStatus();
   bool bypassAutomaticStatus = ot.getBypassAutomaticStatus();
   bool diagnostic = ot.getDiagnosticIndication();
   bool fault = ot.getFaultIndication();
   Serial.print("Vent mode: ");
   Serial.println(ventMode);
   Serial.print("Bypassstatus: ");
   Serial.println(bypassStatus);
   Serial.print("BypassAutomaticStatus: ");
   Serial.println(bypassAutomaticStatus);
   Serial.print("Fault: ");
   Serial.println(fault);
   Serial.print("Diagnostic: ");
   Serial.println(diagnostic);
}

/**
 * If the current speed is not the desired speed, write it to
 * the Vents
 */
int set_speed(int cur_speed) {
  // Be some what lenient
  if (cur_speed <= (int) desiredNomVent + 4 && cur_speed >= (int) desiredNomVent - 4) {
    Serial.print("Already have desired speed of ");
    Serial.println(cur_speed);
    return 0;
  } else {
    Serial.print("Setting Nominal Ventilation to: ");
    Serial.println(desiredNomVent);
    unsigned int result = ot.setVentilation(desiredNomVent);
    Serial.print("Result: ");
    Serial.println(result);
    return 1;
  }
}

void loop() {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    get_temperatures();
    get_pub_diagnostic();

    // Now get the ventilation status
    unsigned long response2 = ot.OpenTherm::sendRequest(
            ot.OpenTherm::buildRequest(OpenThermMessageType::READ_DATA,
                      VentNomVent, 0));

    
    int nom_vent = ot.getVentilation();
    Serial.print("Current Nominal Ventilation: ");
    Serial.println(nom_vent);
    int didsetspeed = set_speed(nom_vent);
    int rpm =  nom_vent * 37.2;
    // 3370 -> 98
    // 930 -> 25
    // 1680 -> 45
    // 2800 -> 75
    
    Serial.print("RPM: ");
    Serial.println(rpm);

    Serial.println();
    // skip sleep if speed has been set
    if (didsetspeed != 1){
      delay(5000);
    }
}

void serialReceive() {
  if (Serial.available() == 0) {
    return;
  }

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