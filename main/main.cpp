#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

using std::string;

#include "enchess_pinout.h"
#include "util.h"
#include "config.h"
#include "movement.h"
#include "ArduinoJson.h"

bool deviceConnected = false;

static struct SettingsOptions {
  int moveTime;
  bool color;
} settingsOptions;

class EnchessServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      digitalWrite(ENCHESS_PIN_LED3, HIGH);
      LOG_MSG("INFO: Client connection registered.");
      
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      digitalWrite(ENCHESS_PIN_LED3, LOW);
      LOG_MSG("INFO: Client disconnection registered.");
    }
};

class EnchessCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      string json = pCharacteristic->getValue();
      StaticJsonDocument<200> doc;

      DeserializationError error = deserializeJson(doc, json);
      if (error) {
        LOG_MSG("ERROR: failed to deserialize settings: %s", error.c_str());
        return;
      }

      settingsOptions.moveTime = doc["moveTime"];
      settingsOptions.color    = doc["color"];

      LOG_MSG("INFO: Updated Settings. New Settings are:");
      LOG_MSG("Move Time - %u", settingsOptions.moveTime);
      LOG_MSG("Color - %u", settingsOptions.color);
    }
};

void setupBLE(void);

void setup() {
  pinMode(ENCHESS_PIN_LED1, OUTPUT);
  pinMode(ENCHESS_PIN_LED2, OUTPUT);
  pinMode(ENCHESS_PIN_LED3, OUTPUT);

  LOG_MSG("Welcome to Enchess %d.%d!", ENCHESS_VERSION_MAJOR, ENCHESS_VERSION_MINOR);
  
  LOG_MSG("INFO: Setting up BLE...");
  setupBLE();
  LOG_MSG("INFO: BLE is set up. Waiting for a client connection...");
  LOG_MSG("INFO: Homing motors...");
  home_motors();
  LOG_MSG("INFO: Finished homing!");
}

void loop() {
  delay(1);
}

void setupBLE(void)
{
  BLEDevice::init(ENCHESS_BLE_NAME);
  LOG_MSG("INFO: BLE Device Name is ENCHESS");

  BLEServer  *enchess_server;
  BLEService *enchess_service;
  BLECharacteristic *enchess_tx_characteristic;
  BLECharacteristic *enchess_rx_characteristic;

  LOG_MSG("INFO: Creating BLE Server...");
  enchess_server = BLEDevice::createServer();
  enchess_server->setCallbacks(new EnchessServerCallbacks());

  LOG_MSG("INFO: Creating UART-Service with UUID %s...", ENCHESS_SERVICE_UUID);
  enchess_service = enchess_server->createService(ENCHESS_SERVICE_UUID);

  LOG_MSG("INFO: Creating TX-Service-Characteristic with UUID %s...", ENCHESS_CHARACTERISTIC_UUID_TX);
  enchess_tx_characteristic = enchess_service->createCharacteristic(
                                ENCHESS_CHARACTERISTIC_UUID_TX,
                                BLECharacteristic::PROPERTY_NOTIFY
                              );
  enchess_tx_characteristic->addDescriptor(new BLE2902());

  LOG_MSG("INFO: Creating RX-Service-Characteristic with UUID %s...", ENCHESS_CHARACTERISTIC_UUID_RX);
  enchess_rx_characteristic = enchess_service->createCharacteristic(
                                ENCHESS_CHARACTERISTIC_UUID_RX,
                                BLECharacteristic::PROPERTY_WRITE
                              );
  enchess_rx_characteristic->setCallbacks(new EnchessCallbacks());

  LOG_MSG("INFO: Starting UART-Service...");
  enchess_service->start();

  LOG_MSG("INFO: Starting Device Advertising...");
  enchess_server->getAdvertising()->addServiceUUID(ENCHESS_SERVICE_UUID);
  enchess_server->getAdvertising()->setScanResponse(true);
  enchess_server->startAdvertising();
}