/*
 * E-Ink Display Control using Relay Polarity Switching
 * Controlled by Serial Commands with Automatic Timing
 * 
 * This code controls an E-Ink display by switching polarity via relays:
 * 'w' : Apply negative voltage (white particles move to top) for a timed pulse
 * 'b' : Apply positive voltage (black particles move to top) for a timed pulse
 * 'o' : No voltage (maintain current state)
 * 
 * Circuit connections:
 * - Arduino pin 11 -> Relay 1 input
 * - Arduino pin 12 -> Relay 2 input
 * - GND -> Relay module GND
 * 
 * Relay states and E-Ink behavior:
 * Relay 1 | Relay 2 | E-Ink Display
 * --------+---------+-------------
 *    0    |    0    | No voltage (maintain state)
 *    1    |    0    | Positive voltage (black particles to top)
 *    0    |    1    | Negative voltage (white particles to top)
 *    1    |    1    | No voltage (maintain state)
 */

// Define the pins connected to the relay module
const int RELAY_1 = 11;  // Controls polarity direction
const int RELAY_2 = 12;  // Controls polarity direction

// Define pulse duration in milliseconds (time to apply voltage)
const unsigned long PULSE_DURATION = 300;  // seconds

char command;                // For storing the incoming command
unsigned long pulseStartTime = 0;  // To track when a pulse started
bool pulseActive = false;    // Flag to track if a pulse is currently active

void setup() {
  // Initialize the relay pins as outputs
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  
  // Ensure no voltage is applied initially (relays are OFF)
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, LOW);
  
  // Initialize Serial communication
  Serial.begin(9600);
  Serial.println("E-Ink Display Control Initialized");
  Serial.println("Commands:");
  Serial.println("  'w' : Apply negative voltage (white particles to top) for " + String(PULSE_DURATION/1000) + " seconds");
  Serial.println("  'b' : Apply positive voltage (black particles to top) for " + String(PULSE_DURATION/1000) + " seconds");
  Serial.println("  'o' : No voltage (maintain current state)");
}

void loop() {
  // Check if a pulse is active and should be turned off
  if (pulseActive && (millis() - pulseStartTime >= PULSE_DURATION)) {
    noVoltage();
    Serial.println("Pulse complete - voltage removed");
    pulseActive = false;
  }
  
  // Check if data is available to read
  if (Serial.available() > 0) {
    // Read the incoming command
    command = Serial.read();
    
    // Process the command
    switch (command) {
      case 'w':
        // Apply negative voltage (white particles to top)
        applyPositiveVoltage();
        Serial.println("Applying NEGATIVE voltage (white particles moving to top)");
        pulseStartTime = millis();
        pulseActive = true;
        break;
        
      case 'b':
        // Apply positive voltage (black particles to top)
        applyNegativeVoltage();
        Serial.println("Applying POSITIVE voltage (black particles moving to top)");
        pulseStartTime = millis();
        pulseActive = true;
        break;
        
      case 'o':
        // No voltage (maintain current state)
        noVoltage();
        Serial.println("No voltage applied (maintaining current state)");
        break;
        
      default:
        // Ignore other characters (like newline, carriage return)
        if (command >= 32 && command <= 126) {  // Printable ASCII characters
          Serial.println("Invalid command. Use 'w' (white), 'b' (black), or 'o' (off).");
        }
        break;
    }
  }
}

// Function to apply no voltage (maintain current state)
void noVoltage() {
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, LOW);
}

// Function to apply positive voltage (black particles to top)
void applyPositiveVoltage() {
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, LOW);
}

// Function to apply negative voltage (white particles to top)
void applyNegativeVoltage() {
  digitalWrite(RELAY_1, LOW);
  digitalWrite(RELAY_2, HIGH);
}

/*
 * Usage Instructions:
 * 
 * 1. Upload this code to your Arduino
 * 2. Open the Serial Monitor (set baud rate to 9600)
 * 3. Send commands:
 *    - Send 'w' to apply negative voltage (white particles move to top) for 2 seconds
 *    - Send 'b' to apply positive voltage (black particles move to top) for 2 seconds
 *    - Send 'o' to apply no voltage (maintain current state)
 * 
 * E-Ink Notes:
 * - E-Ink displays only need power to change state, not to maintain it
 * - The code automatically applies voltage for 2 seconds (adjustable via PULSE_DURATION)
 * - For best display longevity, don't apply voltage longer than necessary
 * - For multiple segments, modify code to control additional relay pairs
 */
