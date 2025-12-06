#include "BleMouseCLI.h"

#if defined(CONFIG_BT_ENABLED)

BleMouseCLI::BleMouseCLI(BleMouse* mouse, bool echo) {
  this->bleMouse = mouse;
  this->echoEnabled = echo;
  this->inputBuffer = "";
}

void BleMouseCLI::begin(unsigned long baudRate) {
  // # BUG: Serial.begin() is called here but never checked for success
  // This might cause issues if Serial port is already in use or initialization fails
  Serial.begin(baudRate);
  Serial.println("\n=== ESP32 BLE Mouse CLI ===");
  Serial.println("Type 'help' for available commands");
  Serial.println("Waiting for commands...\n");
}

void BleMouseCLI::setEcho(bool enabled) {
  this->echoEnabled = enabled;
}

int BleMouseCLI::parseInt(String str, int defaultValue) {
  str.trim();
  if (str.length() == 0) {
    return defaultValue;
  }
  // # BUG: toInt() doesn't validate if conversion failed (returns 0 on error)
  // This means "abc" and "0" both return 0, making it impossible to distinguish between valid zero and parse errors
  // This could cause silent failures when invalid input is provided
  return str.toInt();
}

void BleMouseCLI::printHelp() {
  Serial.println("\n=== Available Commands ===");
  Serial.println("  help                    - Show this help message");
  Serial.println("  status                  - Show connection status");
  Serial.println("  click [button]          - Click mouse button");
  Serial.println("                          - Options: left, right, middle, back, forward");
  Serial.println("  press [button]          - Press and hold button");
  Serial.println("  release [button]        - Release button");
  Serial.println("  move <x> <y>            - Move mouse pointer (relative)");
  Serial.println("  scroll <amount>         - Scroll vertically (positive=up, negative=down)");
  Serial.println("  hscroll <amount>        - Scroll horizontally (positive=right, negative=left)");
  Serial.println("  movefull <x> <y> <v> <h> - Move with all parameters (x, y, vscroll, hscroll)");
  Serial.println("  battery <level>         - Set battery level (0-100)");
  Serial.println("  echo [on|off]           - Enable/disable command echo");
  Serial.println("\nExamples:");
  Serial.println("  click left");
  Serial.println("  move 10 20");
  Serial.println("  scroll -5");
  Serial.println("  movefull 5 5 0 0");
  Serial.println("  battery 75");
  Serial.println("");
}

void BleMouseCLI::printStatus() {
  Serial.println("\n=== Status ===");
  Serial.print("BLE Connected: ");
  Serial.println(bleMouse->isConnected() ? "Yes" : "No");
  Serial.print("Device Name: ");
  Serial.println(bleMouse->deviceName.c_str());
  Serial.print("Manufacturer: ");
  Serial.println(bleMouse->deviceManufacturer.c_str());
  Serial.print("Battery Level: ");
  Serial.print(bleMouse->batteryLevel);
  Serial.println("%");
  Serial.println("");
}

void BleMouseCLI::processCommand(String command) {
  command.trim();
  command.toLowerCase();
  
  if (command.length() == 0) {
    return;
  }
  
  if (echoEnabled) {
    Serial.print("> ");
    Serial.println(command);
  }
  
  // Parse command and arguments
  int spaceIndex = command.indexOf(' ');
  String cmd = spaceIndex > 0 ? command.substring(0, spaceIndex) : command;
  String args = spaceIndex > 0 ? command.substring(spaceIndex + 1) : "";
  
  if (cmd == "help" || cmd == "h" || cmd == "?") {
    printHelp();
  }
  else if (cmd == "status" || cmd == "stat") {
    printStatus();
  }
  else if (cmd == "click") {
    if (!bleMouse->isConnected()) {
      Serial.println("Error: Not connected to host");
      return;
    }
    
    args.trim();
    if (args == "left" || args == "l" || args == "") {
      bleMouse->click(MOUSE_LEFT);
      Serial.println("Left click executed");
    }
    else if (args == "right" || args == "r") {
      bleMouse->click(MOUSE_RIGHT);
      Serial.println("Right click executed");
    }
    else if (args == "middle" || args == "m") {
      bleMouse->click(MOUSE_MIDDLE);
      Serial.println("Middle click executed");
    }
    else if (args == "back" || args == "b") {
      bleMouse->click(MOUSE_BACK);
      Serial.println("Back click executed");
    }
    else if (args == "forward" || args == "f") {
      bleMouse->click(MOUSE_FORWARD);
      Serial.println("Forward click executed");
    }
    else {
      Serial.println("Error: Invalid button. Use: left, right, middle, back, forward");
    }
  }
  else if (cmd == "press") {
    if (!bleMouse->isConnected()) {
      Serial.println("Error: Not connected to host");
      return;
    }
    
    args.trim();
    uint8_t button = MOUSE_LEFT;
    if (args == "left" || args == "l" || args == "") {
      button = MOUSE_LEFT;
    }
    else if (args == "right" || args == "r") {
      button = MOUSE_RIGHT;
    }
    else if (args == "middle" || args == "m") {
      button = MOUSE_MIDDLE;
    }
    else if (args == "back" || args == "b") {
      button = MOUSE_BACK;
    }
    else if (args == "forward" || args == "f") {
      button = MOUSE_FORWARD;
    }
    else {
      Serial.println("Error: Invalid button. Use: left, right, middle, back, forward");
      return;
    }
    
    bleMouse->press(button);
    Serial.println("Button pressed");
  }
  else if (cmd == "release") {
    if (!bleMouse->isConnected()) {
      Serial.println("Error: Not connected to host");
      return;
    }
    
    args.trim();
    uint8_t button = MOUSE_LEFT;
    if (args == "left" || args == "l" || args == "") {
      button = MOUSE_LEFT;
    }
    else if (args == "right" || args == "r") {
      button = MOUSE_RIGHT;
    }
    else if (args == "middle" || args == "m") {
      button = MOUSE_MIDDLE;
    }
    else if (args == "back" || args == "b") {
      button = MOUSE_BACK;
    }
    else if (args == "forward" || args == "f") {
      button = MOUSE_FORWARD;
    }
    else {
      Serial.println("Error: Invalid button. Use: left, right, middle, back, forward");
      return;
    }
    
    bleMouse->release(button);
    Serial.println("Button released");
  }
  else if (cmd == "move") {
    if (!bleMouse->isConnected()) {
      Serial.println("Error: Not connected to host");
      return;
    }
    
    // Parse x and y coordinates
    int spaceIndex = args.indexOf(' ');
    String xStr = spaceIndex > 0 ? args.substring(0, spaceIndex) : args;
    String yStr = spaceIndex > 0 ? args.substring(spaceIndex + 1) : "0";
    
    int x = parseInt(xStr);
    int y = parseInt(yStr);
    
    // Clamp to valid range (-127 to 127)
    x = constrain(x, -127, 127);
    y = constrain(y, -127, 127);
    
    bleMouse->move(x, y);
    Serial.print("Mouse moved: x=");
    Serial.print(x);
    Serial.print(", y=");
    Serial.println(y);
  }
  else if (cmd == "scroll") {
    if (!bleMouse->isConnected()) {
      Serial.println("Error: Not connected to host");
      return;
    }
    
    int amount = parseInt(args, 0);
    amount = constrain(amount, -127, 127);
    
    bleMouse->move(0, 0, amount);
    Serial.print("Scrolled: ");
    Serial.println(amount);
  }
  else if (cmd == "hscroll" || cmd == "h") {
    // # BUG: This command alias "h" conflicts with "help" alias "h"
    // Should remove "h" from hscroll and use a different alias (e.g., "hs")

    if (!bleMouse->isConnected()) {
      Serial.println("Error: Not connected to host");
      return;
    }
    
    int amount = parseInt(args, 0);
    amount = constrain(amount, -127, 127);
    
    bleMouse->move(0, 0, 0, amount);
    Serial.print("Horizontal scroll: ");
    Serial.println(amount);
  }
  else if (cmd == "movefull") {
    if (!bleMouse->isConnected()) {
      Serial.println("Error: Not connected to host");
      return;
    }
    
    // Parse x, y, vscroll, hscroll
    int params[4] = {0, 0, 0, 0};
    int paramIndex = 0;
    String remaining = args;
    
    while (paramIndex < 4 && remaining.length() > 0) {
      remaining.trim();
      int spaceIndex = remaining.indexOf(' ');
      String paramStr = spaceIndex > 0 ? remaining.substring(0, spaceIndex) : remaining;
      params[paramIndex] = parseInt(paramStr);
      params[paramIndex] = constrain(params[paramIndex], -127, 127);
      
      if (spaceIndex > 0) {
        remaining = remaining.substring(spaceIndex + 1);
      } else {
        remaining = "";
      }
      paramIndex++;
    }
    
    bleMouse->move(params[0], params[1], params[2], params[3]);
    Serial.print("Move full: x=");
    Serial.print(params[0]);
    Serial.print(", y=");
    Serial.print(params[1]);
    Serial.print(", vscroll=");
    Serial.print(params[2]);
    Serial.print(", hscroll=");
    Serial.println(params[3]);
  }
  else if (cmd == "battery") {
    int level = parseInt(args, 100);
    level = constrain(level, 0, 100);
    
    bleMouse->setBatteryLevel(level);
    Serial.print("Battery level set to: ");
    Serial.print(level);
    Serial.println("%");
  }
  else if (cmd == "echo") {
    args.trim();
    if (args == "on" || args == "1" || args == "true") {
      echoEnabled = true;
      Serial.println("Echo enabled");
    }
    else if (args == "off" || args == "0" || args == "false") {
      echoEnabled = false;
      Serial.println("Echo disabled");
    }
    else {
      Serial.print("Echo is currently: ");
      Serial.println(echoEnabled ? "on" : "off");
    }
  }
  else {
    Serial.print("Unknown command: ");
    Serial.println(cmd);
    Serial.println("Type 'help' for available commands");
  }
}

void BleMouseCLI::update() {
  // Read incoming serial data
  // # BUG: No buffer overflow protection - inputBuffer can grow unbounded
  // If user sends very long command without newline, this will eventually crash due to memory exhaustion
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }
    }
    else {
      inputBuffer += c;
    }
  }
  // # BUG: Missing delay in update() - this function will consume 100% CPU in tight loops
  // Should add small delay or yield to other tasks to prevent CPU starvation
}

#endif // CONFIG_BT_ENABLED

