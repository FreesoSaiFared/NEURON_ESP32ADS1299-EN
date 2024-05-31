#include <Arduino.h>

#include <NimBLEDevice.h>


static const BLEUUID primaryServiceUUID("ffff");

static const BLEUUID configCharacteristicUUID("ff12");
static const BLEUUID startStopCharacteristicUUID("ff13");

static const BLEUUID sensor1CharacteristicUUID("fff1");
static const BLEUUID sensor2CharacteristicUUID("fff2");
static const BLEUUID sensor3CharacteristicUUID("fff3");
static const BLEUUID sensor4CharacteristicUUID("fff4");
static const BLEUUID sensor5CharacteristicUUID("fff5");
static const BLEUUID sensor6CharacteristicUUID("fff6");
static const BLEUUID sensor7CharacteristicUUID("fff7");
static const BLEUUID sensor8CharacteristicUUID("fff8");

// Declara características adicionales según sea necesario
BLEServer *pServer = NULL;
BLEService *pService = NULL;

BLECharacteristic *pConfigCharacteristic = NULL;
BLECharacteristic *pStartStopCharacteristic = NULL;

BLECharacteristic *pSensor1Characteristic = NULL;
BLECharacteristic *pSensor2Characteristic = NULL;
BLECharacteristic *pSensor3Characteristic = NULL;
BLECharacteristic *pSensor4Characteristic = NULL;
BLECharacteristic *pSensor5Characteristic = NULL;
BLECharacteristic *pSensor6Characteristic = NULL;
BLECharacteristic *pSensor7Characteristic = NULL;
BLECharacteristic *pSensor8Characteristic = NULL;

int numSamples = 500;
bool flag;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;
const int ledPin = 2;

// Servidor BLE y devoluciones de llamada
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
      deviceConnected = true;
    };
    void onDisconnect(BLEServer *pServer) {
      deviceConnected = false;
    }
};


// Function to generate an array of 8 bytes with random values
void generateRandomBytes( byte* byteArray, int numElements) {
  // Seed the random number generator with the input x
  randomSeed(millis());

  // Generate 8 random integers within the range -2000 to 2000
  for (int i = 0; i < numElements; i++) {
    // Generate a random number between -2000 and 2000
    int randomValue = random(0, 255);

    // Convert the random number to bytes and store it in the byteArray
    byteArray[i] = (byte)(randomValue & 0xFF); // Take the least significant byte
  }
}

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      Serial.print("UUID:");
      Serial.println( String(pCharacteristic->getUUID().toString().c_str()));
      std::string value = pCharacteristic->getValue();

      if (pCharacteristic->getUUID().equals(configCharacteristicUUID)) {
        uint8_t byte1 = pCharacteristic->getValue()[0];
        uint8_t byte2 = pCharacteristic->getValue()[1];
        numSamples = byte1 << 8 | byte2;
        Serial.println("Num samples:" + String(numSamples));
      }

      if (pCharacteristic->getUUID().equals(startStopCharacteristicUUID)) {
        flag = pCharacteristic->getValue()[0] == 1 ? true : false;
        Serial.println("Start/Stop:" + String(flag));
      }

    }
};

void setup() {
  Serial.begin(115200);
  // Inicializa el dispositivo BLE
  BLEDevice::init("NEURON-BLE");

  // Crea el servidor BLE
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  pService = pServer->createService(primaryServiceUUID);

  // Crea características adicionales para los 8 vectores
  pSensor1Characteristic = pService->createCharacteristic(
                             sensor1CharacteristicUUID,
                             NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
                             NIMBLE_PROPERTY::NOTIFY);
  pSensor2Characteristic = pService->createCharacteristic(
                             sensor2CharacteristicUUID,
                             NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
                             NIMBLE_PROPERTY::NOTIFY);
  pSensor3Characteristic = pService->createCharacteristic(
                             sensor3CharacteristicUUID,
                             NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
                             NIMBLE_PROPERTY::NOTIFY);
  pSensor4Characteristic = pService->createCharacteristic(
                             sensor4CharacteristicUUID,
                             NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
                             NIMBLE_PROPERTY::NOTIFY);
  pSensor5Characteristic = pService->createCharacteristic(
                             sensor5CharacteristicUUID,
                             NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
                             NIMBLE_PROPERTY::NOTIFY);
  pSensor6Characteristic = pService->createCharacteristic(
                             sensor6CharacteristicUUID,
                             NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE |
                             NIMBLE_PROPERTY::NOTIFY);
  pSensor7Characteristic = pService->createCharacteristic(
                             sensor7CharacteristicUUID,
                             NIMBLE_PROPERTY::READ |
                             NIMBLE_PROPERTY::WRITE |
                             NIMBLE_PROPERTY::NOTIFY);
  pSensor8Characteristic = pService->createCharacteristic(
                             sensor8CharacteristicUUID,
                             NIMBLE_PROPERTY::READ |
                             NIMBLE_PROPERTY::WRITE |
                             NIMBLE_PROPERTY::NOTIFY);

  // Crea la característica para enviar el tamaño de los vectores
  pConfigCharacteristic = pService->createCharacteristic(
                            configCharacteristicUUID, NIMBLE_PROPERTY::READ |
                            NIMBLE_PROPERTY::WRITE);

  // Crea la característica para el botón de inicio
  pStartStopCharacteristic = pService->createCharacteristic(
                               startStopCharacteristicUUID,  NIMBLE_PROPERTY::READ |
                               NIMBLE_PROPERTY::WRITE);

  // Configura las devoluciones de llamada para la característica de inicio
  pConfigCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
  pStartStopCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  pSensor1Characteristic->setCallbacks(new MyCharacteristicCallbacks());
  pSensor2Characteristic->setCallbacks(new MyCharacteristicCallbacks());
  pSensor3Characteristic->setCallbacks(new MyCharacteristicCallbacks());
  pSensor4Characteristic->setCallbacks(new MyCharacteristicCallbacks());
  pSensor5Characteristic->setCallbacks(new MyCharacteristicCallbacks());
  pSensor6Characteristic->setCallbacks(new MyCharacteristicCallbacks());
  pSensor7Characteristic->setCallbacks(new MyCharacteristicCallbacks());
  pSensor8Characteristic->setCallbacks(new MyCharacteristicCallbacks());


  // Inicia el servicio BLE
  pService->start();

  // Inicia el anuncio BLE
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(primaryServiceUUID);
  pAdvertising->setScanResponse(false);

  pAdvertising->start();

  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  //  // Notifica los cambios en las características cuando un dispositivo está conectado
  if (flag) {
    int dataByteSize = numSamples * 2;

    byte* data1 = (byte*)malloc(dataByteSize * sizeof(byte));
    generateRandomBytes(data1, dataByteSize);
    pSensor1Characteristic->setValue(data1, dataByteSize);
    pSensor1Characteristic->notify();
    free(data1);
    
    byte* data2 = (byte*)malloc(dataByteSize * sizeof(byte));
    generateRandomBytes(data1, dataByteSize);
    pSensor2Characteristic->setValue(data2, dataByteSize);
    pSensor2Characteristic->notify();
    free(data2);

    byte* data3 = (byte*)malloc(dataByteSize * sizeof(byte));
    generateRandomBytes(data1, dataByteSize);
    pSensor3Characteristic->setValue(data3, dataByteSize);
    pSensor3Characteristic->notify();
    free(data3);

    byte* data4 = (byte*)malloc(dataByteSize * sizeof(byte));
    generateRandomBytes(data1, dataByteSize);
    pSensor4Characteristic->setValue(data4, dataByteSize);
    pSensor4Characteristic->notify();
    free(data4);

    byte* data5 = (byte*)malloc(dataByteSize * sizeof(byte));
    generateRandomBytes(data1, dataByteSize);
    pSensor5Characteristic->setValue(data5, dataByteSize);
    pSensor5Characteristic->notify();
    free(data5);

    byte* data6 = (byte*)malloc(dataByteSize * sizeof(byte));
    generateRandomBytes(data1, dataByteSize);
    pSensor6Characteristic->setValue(data6, dataByteSize);
    pSensor6Characteristic->notify();
    free(data6);

    byte* data7 = (byte*)malloc(dataByteSize * sizeof(byte));
    generateRandomBytes(data1, dataByteSize);
    pSensor7Characteristic->setValue(data7, dataByteSize);
    pSensor7Characteristic->notify();
    free(data7);

    byte* data8 = (byte*)malloc(dataByteSize * sizeof(byte));
    generateRandomBytes(data1, dataByteSize);
    pSensor8Characteristic->setValue(data8, dataByteSize);
    pSensor8Characteristic->notify();
    free(data8);

    flag = false;
  }
  //  // Maneja los eventos de conexión y desconexión
  if (!deviceConnected && oldDeviceConnected) {
    Serial.println("Device disconnected.");
    delay(500);
    pServer->startAdvertising();
    oldDeviceConnected = deviceConnected;
  }

  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
    Serial.println("Device Connected");
  }
}
