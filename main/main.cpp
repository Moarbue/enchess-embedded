#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

using std::string;

#include "enchess_pinout.h"
#include "util.h"
#include "config.h"

bool deviceConnected = false;

class EnchessServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      digitalWrite(ENCHESS_PIN_LED3, HIGH);
      
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      digitalWrite(ENCHESS_PIN_LED3, LOW);
    }
};

class EnchessCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      string rxValue = pCharacteristic->getValue();


      if (rxValue.length() > 0) {
        for (int i = 0; i < rxValue.length(); i++) {
          LOG_MSG("%c", rxValue[i]);
        }

        LOG_MSG("\r\n");

        // Do stuff based on the command received from the app
        if (rxValue.find("A") != -1) { 
          LOG_MSG("Turning ON!");
          digitalWrite(ENCHESS_PIN_LED1, HIGH);
        }
        else if (rxValue.find("B") != -1) {
          LOG_MSG("Turning OFF!");
          digitalWrite(ENCHESS_PIN_LED1, LOW);
        }
      }
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
  home_motors();
}

void loop() {
}

void setupBLE(void)
{
  BLEDevice::init("Enchess");
  LOG_MSG("INFO: BLE Device Name is Enchess");

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
  enchess_server->startAdvertising();
}