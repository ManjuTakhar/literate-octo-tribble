#ifndef ESP32_BLE_MOUSE_CLI_H
#define ESP32_BLE_MOUSE_CLI_H
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)

#include "BleMouse.h"
#include <Arduino.h>

class BleMouseCLI {
private:
  BleMouse* bleMouse;
  String inputBuffer;
  bool echoEnabled;
  
  void processCommand(String command);
  void printHelp();
  void printStatus();
  int parseInt(String str, int defaultValue = 0);
  
public:
  BleMouseCLI(BleMouse* mouse, bool echo = true);
  void begin(unsigned long baudRate = 115200);
  void update(); // Call this in loop() to process incoming commands
  void setEcho(bool enabled);
};

#endif // CONFIG_BT_ENABLED
#endif // ESP32_BLE_MOUSE_CLI_H

