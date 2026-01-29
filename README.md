# ESP32 Bluetooth Keyboard Clicker

This project turns an ESP32 into a Bluetooth HID keyboard that automatically sends spacebar presses every 500ms. Perfect for keeping applications active, preventing screen timeouts, or automating repetitive spacebar tasks.

## Hardware Requirements

- **ESP32 Development Board** - DOIT ESP32-32DN or compatible
  - Board used in development: [DOIT ESP32 DevKit V1 on Amazon](https://www.amazon.com/dp/B0DSZFD1SD?th=1) (not sponsored)
  - Any ESP32 board with Bluetooth LE support should work
- **Built-in components used:**
  - GPIO 0 (BOOT button) - Toggle auto-clicking on/off
  - GPIO 2 (Built-in LED) - Green status indicator (on when running)
  - GPIO 4 - Red status indicator (on when stopped)

## Software Requirements

- Arduino IDE (1.8.x or 2.x)
- ESP32 Board Support Package (tested with v3.3.5)
- No external libraries required - uses built-in ESP32 BLE libraries

## Installation

### 1. Install Arduino IDE
Download and install from [arduino.cc](https://www.arduino.cc/en/software)

### 2. Add ESP32 Board Support
1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add this URL to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Go to **Tools → Board → Boards Manager**
5. Search for "esp32" and install "esp32 by Espressif Systems"

### 3. Configure Board Settings
1. Go to **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
2. Configure the following settings:
   - **Upload Speed:** 921600 (or 115200 if issues occur)
   - **Flash Frequency:** 80MHz
   - **Flash Mode:** QIO
   - **Flash Size:** 4MB (32Mb)
   - **Partition Scheme:** Default 4MB with spiffs
   - **PSRAM:** Disabled
   - **Port:** Select your COM port

## Usage

### Upload the Code
1. Open `ESP32Clicker.ino` in Arduino IDE
2. Select **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
3. Select the correct COM port: **Tools → Port**
4. Click **Upload**

### Connect to Your Device
1. After uploading, the ESP32 will start advertising as "ESP32 Clicker"
2. On your computer/phone/tablet:
   - Go to Bluetooth settings
   - Look for "ESP32 Clicker"
   - Pair with it as a **keyboard device**
   - May require bonding/pairing confirmation
3. The red LED will be on, indicating it's ready but not auto-clicking

### Start Auto-Clicking
1. Press the **BOOT button** (GPIO 0) on the ESP32
2. The **green LED** will turn on, red LED will turn off
3. Spacebar presses will be sent every 500ms
4. Press the button again to stop (red LED on, green LED off)

### Monitor Serial Output
- Open Serial Monitor (**Tools → Serial Monitor**)
- Set baud rate to **115200**
- You'll see:
  - Connection status
  - Button press events
  - "Space key sent" messages when auto-clicking is active

## How It Works

The ESP32 acts as a Bluetooth Low Energy (BLE) HID keyboard using the built-in BLE stack. It implements:
- Standard HID keyboard report descriptor
- Proper BLE security with bonding
- Connection state management
- Debounced button input for reliable toggling
- Visual feedback via LEDs

## Customization

### Change the Press Interval
Modify the `pressInterval` constant (in milliseconds):
```cpp
const unsigned long pressInterval = 500;  // Change to desired interval (e.g., 1000 for 1 second)
```

### Change the Key Being Pressed
Modify the `sendSpaceKey()` function to send different keys. The key code 0x2C is spacebar. Other common HID key codes:
```cpp
// In sendSpaceKey() function, change 0x2C to:
0x28  // Enter/Return
0x29  // Escape
0x2A  // Backspace
0x50  // Left Arrow
0x4F  // Right Arrow
0x52  // Up Arrow
0x51  // Down Arrow
```

### Change Device Name
Modify the device name in `setup()`:
```cpp
BLEDevice::init("ESP32 Clicker");  // Change to your desired name
```

### Change Button Pin
If you want to use a different GPIO pin for the button:
```cpp
const int BUTTON_PIN = 0;  // Change to your desired GPIO pin
```

### Change LED Pins
Modify the LED pin assignments:
```cpp
const int LED_GREEN_PIN = 2;  // GPIO for green/running indicator
const int LED_RED_PIN = 4;    // GPIO for red/stopped indicator
```

## Troubleshooting

**ESP32 not showing in Bluetooth devices:**
- Make sure the code uploaded successfully
- Check Serial Monitor for "BLE Keyboard ready!" message
- Try restarting the ESP32
- Ensure Bluetooth is enabled on your device
- Some devices may take a few seconds to discover new Bluetooth devices

**Compilation errors:**
- Ensure ESP32 board support is properly installed (v3.3.5 tested)
- Check that you've selected "ESP32 Dev Module" (not Arduino Uno, etc.)
- Verify all required BLE libraries are included with the ESP32 package

**Device connects but keys not being sent:**
- Verify the device is paired as a **keyboard**, not just a generic Bluetooth device
- Check Serial Monitor for "BLE Device Connected" message
- Press the button to start auto-clicking (green LED should turn on)
- Open a text editor to test if spaces appear
- Try unpairing and re-pairing the device
- Some operating systems may require additional permissions for BLE HID devices

**Button not responding:**
- Check Serial Monitor for button state change messages
- Ensure GPIO 0 is not damaged or shorted
- Try holding the button for 1-2 seconds
- The BOOT button may be sensitive - try pressing firmly

**Connection drops frequently:**
- Keep the ESP32 within range of the connected device
- Ensure stable power supply to the ESP32
- Check for interference from other 2.4GHz devices

## Compatible Devices

Tested and working with:
- Windows 10/11
- macOS
- Linux
- Android
- iOS/iPadOS

## Technical Details

- **Protocol:** Bluetooth Low Energy (BLE) 4.0+
- **Profile:** HID (Human Interface Device)
- **Device Type:** Keyboard
- **Security:** ESP_LE_AUTH_BOND (bonding required)
- **Report ID:** 1
- **Key Code:** 0x2C (Spacebar in HID usage table)

## License

This project is open source and available for any use.

## Disclaimer

This device is intended for legitimate automation purposes. Users are responsible for ensuring their use complies with applicable terms of service and regulations.
