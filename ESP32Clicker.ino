#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"

const int BUTTON_PIN = 0;
const int LED_GREEN_PIN = 2;
const int LED_RED_PIN = 4;

unsigned long lastPressTime = 0;
const unsigned long pressInterval = 1000;

bool isRunning = false;
bool lastButtonState = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
bool isBleConnected = false;

BLEHIDDevice* hid;
BLECharacteristic* input;

struct InputReport {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t pressedKeys[6];
};

const InputReport NO_KEY_PRESSED = { };

static const uint8_t REPORT_MAP[] = {
  USAGE_PAGE(1), 0x01,
  USAGE(1), 0x06,
  COLLECTION(1), 0x01,
  REPORT_ID(1), 0x01,
  USAGE_PAGE(1), 0x07,
  USAGE_MINIMUM(1), 0xE0,
  USAGE_MAXIMUM(1), 0xE7,
  LOGICAL_MINIMUM(1), 0x00,
  LOGICAL_MAXIMUM(1), 0x01,
  REPORT_COUNT(1), 0x08,
  REPORT_SIZE(1), 0x01,
  HIDINPUT(1), 0x02,
  REPORT_COUNT(1), 0x01,
  REPORT_SIZE(1), 0x08,
  HIDINPUT(1), 0x01,
  REPORT_COUNT(1), 0x06,
  REPORT_SIZE(1), 0x08,
  LOGICAL_MINIMUM(1), 0x00,
  LOGICAL_MAXIMUM(1), 0x65,
  USAGE_MINIMUM(1), 0x00,
  USAGE_MAXIMUM(1), 0x65,
  HIDINPUT(1), 0x00,
  REPORT_COUNT(1), 0x05,
  REPORT_SIZE(1), 0x01,
  USAGE_PAGE(1), 0x08,
  USAGE_MINIMUM(1), 0x01,
  USAGE_MAXIMUM(1), 0x05,
  LOGICAL_MINIMUM(1), 0x00,
  LOGICAL_MAXIMUM(1), 0x01,
  HIDOUTPUT(1), 0x02,
  REPORT_COUNT(1), 0x01,
  REPORT_SIZE(1), 0x03,
  HIDOUTPUT(1), 0x01,
  END_COLLECTION(0)
};

class BleKeyboardCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* server) {
    isBleConnected = true;
    BLE2902* cccDesc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    cccDesc->setNotifications(true);
    Serial.println("BLE Device Connected");
  }

  void onDisconnect(BLEServer* server) {
    isBleConnected = false;
    BLE2902* cccDesc = (BLE2902*)input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    cccDesc->setNotifications(false);
    Serial.println("BLE Device Disconnected");
    delay(500);
    BLEDevice::startAdvertising();
    Serial.println("Advertising restarted");
  }
};

void sendSpaceKey() {
  if (!isBleConnected) return;

  InputReport report = {
    .modifiers = 0,
    .reserved = 0,
    .pressedKeys = { 0x2C, 0, 0, 0, 0, 0 }
  };

  input->setValue((uint8_t*)&report, sizeof(report));
  input->notify();
  delay(250);

  input->setValue((uint8_t*)&NO_KEY_PRESSED, sizeof(NO_KEY_PRESSED));
  input->notify();
  delay(10);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting ESP32 BLE Keyboard...");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);

  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);

  BLEDevice::init("ESP32 Clicker");
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BleKeyboardCallbacks());

  hid = new BLEHIDDevice(pServer);
  input = hid->inputReport(1);

  hid->manufacturer()->setValue("ESP32");
  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00, 0x02);

  BLESecurity* security = new BLESecurity();
  security->setAuthenticationMode(ESP_LE_AUTH_BOND);

  hid->reportMap((uint8_t*)REPORT_MAP, sizeof(REPORT_MAP));
  hid->startServices();

  hid->setBatteryLevel(100);

  BLEAdvertising* pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_KEYBOARD);
  pAdvertising->addServiceUUID(hid->hidService()->getUUID());
  pAdvertising->addServiceUUID(hid->deviceInfo()->getUUID());
  pAdvertising->addServiceUUID(hid->batteryService()->getUUID());
  pAdvertising->start();

  Serial.println("BLE Keyboard ready!");
  Serial.println("Waiting for connection...");
  Serial.println("Press button to toggle auto-clicking");
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        isRunning = !isRunning;
        Serial.println(isRunning ? "Auto-clicking STARTED" : "Auto-clicking STOPPED");

        if (isRunning) {
          digitalWrite(LED_GREEN_PIN, HIGH);
          digitalWrite(LED_RED_PIN, LOW);
        } else {
          digitalWrite(LED_GREEN_PIN, LOW);
          digitalWrite(LED_RED_PIN, HIGH);
        }
      }
    }
  }

  lastButtonState = reading;

  if (isBleConnected && isRunning) {
    unsigned long currentTime = millis();
    if (currentTime - lastPressTime >= pressInterval) {
      sendSpaceKey();
      lastPressTime = currentTime;
      Serial.println("Space key sent");
    }
  }

  delay(10);
}