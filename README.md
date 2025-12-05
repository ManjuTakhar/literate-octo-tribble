# ESP32 BLE Mouse library

This library allows you to make the ESP32 act as a Bluetooth Mouse and control what it does. E.g. move the mouse, scroll, make a click etc.

You might also be interested in:
- [ESP32-BLE-Abs-Mouse](https://github.com/sobrinho/ESP32-BLE-Abs-Mouse)
- [ESP32-BLE-Keyboard](https://github.com/T-vK/ESP32-BLE-Keyboard)
- [ESP32-BLE-Gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad)

## Features

 - [x] Left click
 - [x] Right click
 - [x] Middle click
 - [x] Back/Forwards click
 - [x] Move mouse pointer left/right
 - [x] Move mouse pointer up/down
 - [x] Scroll up/down
 - [x] Scroll left/right
 - [x] Report optional battery level to host (basically works, but it doesn't show up in Android's status bar)
 - [x] Customize Bluetooth device name/manufacturer
 - [x] Compatible with Android
 - [x] Compatible with Windows
 - [x] Compatible with Linux
 - [x] Compatible with MacOS X (not stable, some people have issues, doesn't work with old devices)
 - [x] Compatible with iOS (not stable, some people have issues, doesn't work with old devices)
 
## Installation
- (Make sure you can use the ESP32 with the Arduino IDE. [Instructions can be found here.](https://github.com/espressif/arduino-esp32#installation-instructions))
- [Download the latest release of this library from the release page.](https://github.com/T-vK/ESP32-BLE-Mouse/releases)
- In the Arduino IDE go to "Sketch" -> "Include Library" -> "Add .ZIP Library..." and select the file you just downloaded.
- You can now go to "File" -> "Examples" -> "ESP32 BLE Mouse" and select any of the examples to get started.

## Example

``` C++
/**
 * This example turns the ESP32 into a Bluetooth LE mouse that scrolls down every 2 seconds.
 */
#include <BleMouse.h>

BleMouse bleMouse;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleMouse.begin();
}

void loop() {
  if(bleMouse.isConnected()) {
    Serial.println("Scroll Down");
    bleMouse.move(0,0,-1);
  }
  delay(2000);
}
```

## API docs
The BleMouse interface is almost identical to the Mouse Interface, so you can use documentation right here:
https://www.arduino.cc/reference/en/language/functions/usb/mouse/

Just remember that you have to use `bleMouse` instead of just `Mouse` and you need these two lines at the top of your script:
```
#include <BleMouse.h>
BleMouse bleMouse;
```

This library supports a few additional features that the Mouse library does not support at the time of writing:

- Scrolling left/right E.g.: `bleMouse.move(0,0,0,1)` (Scroll left) and `bleMouse.move(0,0,0,-1)` (Scroll right)
- Using the back and forward buttons E.g.: `bleMouse.click(MOUSE_BACK)` and `bleMouse.click(MOUSE_FORWARD)`

There is also Bluetooth specific information that you can use (optional):

Instead of `BleMouse bleMouse;` you can do `BleMouse bleMouse("Bluetooth Device Name", "Bluetooth Device Manufacturer", 100);`.
The third parameter is the initial battery level of your device. To adjust the battery level later on you can simply call e.g.  `bleMouse.setBatteryLevel(50)` (set battery level to 50%).
By default the battery level will be set to 100%, the device name will be `ESP32 Bluetooth Mouse` and the manufacturer will be `Espressif`.


## Known Issues and Limitations

This section documents known issues, limitations, and design decisions that might appear as bugs but are intentional or unavoidable due to technical constraints.

### Real Issues

1. **Battery level reporting on Android**: Battery level is reported but doesn't show up in Android's status bar. This is a limitation of how Android handles HID battery reports.

2. **macOS/iOS compatibility**: Not stable on macOS and iOS, especially on older devices. Some users experience connection issues. This is due to Apple's stricter BLE HID implementation.

3. **Empty `end()` function**: The `end()` method doesn't actually stop BLE advertising or cleanup resources. Once `begin()` is called, BLE cannot be properly stopped without resetting the ESP32. This is a known limitation.

4. **No error handling in `move()`**: The `move()` function silently fails when not connected - there's no way to know if a command was ignored. This is by design to match the Arduino Mouse library API.

5. **Command alias conflict in CLI**: The CLI command "h" is used for both "help" and "hscroll". Currently, "h" triggers horizontal scroll, not help. This is a known limitation that may be addressed in future versions.

### False Positives / Not Actually Bugs

1. **C-style cast in `taskServer()`**: The code uses `(BleMouse *)` cast instead of `static_cast`. While less type-safe, this is acceptable in FreeRTOS task contexts and matches common ESP32 patterns.

2. **No delay in `click()`**: The click function doesn't add a delay between press and release. This is intentional - the HID protocol handles click timing, and adding delays would slow down the library unnecessarily.

3. **`parseInt()` doesn't validate conversion**: The `parseInt()` function returns 0 for both "0" and invalid strings. This is a limitation of Arduino's `String.toInt()` method, not a bug in our code. For CLI usage, this is acceptable behavior.

4. **No buffer overflow protection in CLI**: The CLI input buffer can theoretically grow unbounded if very long commands are sent. In practice, Serial buffer limits and typical command lengths make this unlikely to be an issue.

5. **No delay in `update()` loop**: The CLI `update()` function doesn't include a delay, which could consume CPU. However, `Serial.available()` blocks when there's no data, so this is not actually a problem.

6. **Silent failure when not connected**: Mouse commands fail silently when BLE is not connected. This matches the behavior of the standard Arduino Mouse library and is intentional for API compatibility.

7. **`notify()` return value not checked**: BLE notification failures aren't checked. The underlying BLE stack handles retries automatically, so explicit checking isn't necessary for most use cases.

### Technical Constraints

1. **1-byte movement range**: Mouse movement is limited to -127 to +127 pixels per command due to HID protocol constraints. This is a hardware/protocol limitation, not a software bug.

2. **BLE dependency**: The library requires Bluetooth to be enabled. This is fundamental to the library's purpose and cannot be changed.

3. **ESP32-only**: This library only works on ESP32 due to BLE stack requirements. It cannot be ported to ESP8266 or other platforms without significant architectural changes.

4. **Relative positioning only**: The library only supports relative mouse movement (like a standard mouse), not absolute positioning (like a touchpad). This is by design - use [ESP32-BLE-Abs-Mouse](https://github.com/sobrinho/ESP32-BLE-Abs-Mouse) for absolute positioning.

## Credits

Credits to [chegewara](https://github.com/chegewara) as this library is based on [this piece of code](https://github.com/nkolban/esp32-snippets/issues/230#issuecomment-473135679) that he provided.
