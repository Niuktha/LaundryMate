#include <LiquidCrystal.h>

// Initialize the LCD object
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define the pin numbers for each sensor
#define VIBRATION_SENSOR_PIN 7
#define WATER_FLOW_SENSOR_PIN 6
#define TDS_SENSOR_PIN A0

// Define the states of the washing machine
enum MachineState {
  OFF,
  WASHING,
  RINSING,
  SPINNING,
  DRAINING,
  DONE
};

MachineState machineState = OFF;

// Define the number of TDS readings to average 
#define NUM_TDS_READINGS 20

void setup() {
  // Initialize the serial communication for debugging
  Serial.begin(9600);

  // Set up the LCD
  lcd.begin(16, 2);

  // Set the sensor pins as input
  pinMode(VIBRATION_SENSOR_PIN, INPUT);
  pinMode(WATER_FLOW_SENSOR_PIN, INPUT);
  pinMode(TDS_SENSOR_PIN, INPUT);
}

void loop() {
  // Read the sensor values 
  int vibration = digitalRead(VIBRATION_SENSOR_PIN);
  int waterFlow = digitalRead(WATER_FLOW_SENSOR_PIN);

  // Take multiple TDS readings and calculate the average
  int totalTdsValue = 0;
  for (int i = 0; i < NUM_TDS_READINGS; i++) {
    totalTdsValue += analogRead(TDS_SENSOR_PIN);
    delay(5); // Wait a bit between readings
  }
  int tdsValue = totalTdsValue / NUM_TDS_READINGS;

  // Determine the machine state based on the sensor values
  if (vibration == HIGH && waterFlow == LOW) {
    machineState = WASHING;
  } else if (waterFlow == HIGH && vibration == LOW) {
    machineState = RINSING;
  } else if (vibration == HIGH && waterFlow == HIGH) {
    machineState = SPINNING;
  } else if (tdsValue > 260) {
    machineState = DRAINING;
  } else {
    machineState = OFF;
  }

  // Display the machine state on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Machine State:");
  lcd.setCursor(0, 1);
  lcd.print(getMachineStateString(machineState));

  // Print the machine state and TDS value to the serial monitor for debugging
  Serial.print("Machine state: ");
  Serial.println(getMachineStateString(machineState));
  Serial.print("TDS value: ");
  Serial.println(tdsValue);

  delay(1000);
}

// Helper function to get the machine state as a string
String getMachineStateString(MachineState state) {
  switch (state) {
    case OFF: return "OFF";
    case WASHING: return "WASHING";
    case RINSING: return "RINSING";
    case SPINNING: return "SPINNING";
    case DRAINING: return "DRAINING";
    case DONE: return "DONE";
    default: return "UNKNOWN";
  }
}
