# ESP32 Bluetooth Keyboard Clicker

This project turns an ESP32 into a Bluetooth keyboard that automatically sends the space key every 500ms.

## Hardware Requirements
- ESP32 development board (ESP32-WROOM, ESP32-DevKitC, or similar)

## Software Requirements
- Arduino IDE (1.8.x or 2.x)
- ESP32 Board Support Package
- BLE Keyboard Library

## Installation

### 1. Install Arduino IDE
Download and install from [arduino.cc](https://www.arduino.cc/en/software)

### 2. Add ESP32 Board Support
1. Open Arduino IDE
2. Go to File → Preferences
3. Add this URL to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to Tools → Board → Boards Manager
5. Search for "esp32" and install "esp32 by Espressif Systems"

### 3. Install BLE Keyboard Library
1. Go to Sketch → Include Library → Manage Libraries
2. Search for "ESP32 BLE Keyboard"
3. Install "ESP32-BLE-Keyboard" by T-vK

## Usage

### Upload the Code
1. Open `ESP32Clicker.ino` in Arduino IDE
2. Select your ESP32 board: Tools → Board → ESP32 Arduino → (your board model)
3. Select the correct COM port: Tools → Port
4. Click Upload

### Connect to Your Device
1. After uploading, the ESP32 will start advertising as "ESP32 Clicker"
2. On your computer/phone/tablet:
   - Go to Bluetooth settings
   - Look for "ESP32 Clicker"
   - Pair with it (no PIN required)
3. Once connected, the ESP32 will start sending space key presses every 500ms

### Monitor Serial Output
- Open Serial Monitor (Tools → Serial Monitor)
- Set baud rate to 115200
- You'll see connection status and when space keys are sent

## Customization

### Change the Press Interval
Modify the `pressInterval` constant (in milliseconds):
```cpp
const unsigned long pressInterval = 500;  // Change to desired interval
```

### Change the Key Being Pressed
Replace the space character with another key:
```cpp
bleKeyboard.write(' ');  // Space key
bleKeyboard.write('a');  // Letter 'a'
bleKeyboard.press(KEY_RETURN);  // Enter key
bleKeyboard.releaseAll();
```

### Change Device Name
Modify the BleKeyboard initialization:
```cpp
BleKeyboard bleKeyboard("Your Device Name", "Manufacturer", 100);
```

## Troubleshooting

**ESP32 not showing in Bluetooth devices:**
- Make sure the code uploaded successfully
- Check Serial Monitor for "Starting BLE Keyboard..." message
- Try restarting the ESP32
- Some devices may take a few seconds to discover new Bluetooth devices

**Compilation errors:**
- Ensure ESP32 board support is properly installed
- Verify BLE Keyboard library is installed
- Check that you've selected an ESP32 board (not Arduino Uno, etc.)

**Keys not being sent:**
- Verify the device is connected (check Serial Monitor)
- Some applications may not respond to rapid key presses
- Try increasing the press interval

## License
This project is open source and available for any use.
