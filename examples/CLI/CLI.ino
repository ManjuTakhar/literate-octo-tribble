/**
 * This example turns the ESP32 into a Bluetooth LE mouse with a Command Line Interface (CLI).
 * 
 * Connect to the ESP32 via Serial Monitor (115200 baud) and send commands to control the mouse.
 * 
 * Example commands:
 *   help              - Show available commands
 *   status            - Show connection status
 *   click left        - Left click
 *   move 10 20        - Move mouse pointer
 *   scroll -5         - Scroll down
 *   battery 75        - Set battery level to 75%
 */
#include <BleMouse.h>
#include <BleMouseCLI.h>

BleMouse bleMouse;
BleMouseCLI cli(&bleMouse);

void setup() {
  // Initialize BLE Mouse
  bleMouse.begin();
  
  // Initialize CLI (starts Serial at 115200 baud)
  cli.begin();
  
  Serial.println("BLE Mouse initialized. Waiting for connection...");
  Serial.println("Once connected, you can use the CLI to control the mouse.\n");
}

void loop() {
  // Update CLI to process incoming commands
  cli.update();
  
  // Optional: Print connection status when it changes
  static bool lastConnected = false;
  bool currentConnected = bleMouse.isConnected();
  
  if (currentConnected != lastConnected) {
    if (currentConnected) {
      Serial.println("\n*** Connected to host! ***");
      Serial.println("You can now send commands via Serial Monitor.\n");
    } else {
      Serial.println("\n*** Disconnected from host ***\n");
    }
    lastConnected = currentConnected;
  }
  
  delay(10); // Small delay to prevent overwhelming the CPU
}

