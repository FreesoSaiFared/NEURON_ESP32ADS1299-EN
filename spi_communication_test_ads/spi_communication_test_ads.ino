#include <SPI.h>

// Definición de los pines
#define CS_PIN 5       // Pin de Chip Select para el ADS1299
#define DRDY_PIN 4     // Pin de Data Ready, indica cuando los datos están listos
#define RESET_PIN 15   // Pin de RESET
#define START_PIN 16   // Pin de START para comenzar la adquisición de datos

void setup() {
  Serial.begin(115200);  // Iniciar la comunicación serial
  pinMode(CS_PIN, OUTPUT);
  pinMode(DRDY_PIN, INPUT);
  pinMode(START_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);

  digitalWrite(CS_PIN, HIGH); // Desactivar el ADS1299
  digitalWrite(RESET_PIN, HIGH); // Mantener el pin de RESET en alto
  digitalWrite(START_PIN, LOW); // Asegurarse de que START esté en bajo

  // Iniciar SPI
  SPI.begin(18, 19, 23); // Configurar pines SPI (SCK = 18, MISO = 19, MOSI = 23)
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1)); // 4 MHz, MSB primero, Modo 1

  delay(1000); // Pequeño retardo para estabilización

  // Resetear el ADS1299
  digitalWrite(RESET_PIN, LOW);
  delay(1); // Mantener el RESET bajo por al menos 18 tCLK
  digitalWrite(RESET_PIN, HIGH);
  delay(10); // Esperar después del reset

  // Enviar comandos de configuración del ADS1299
  sendCommand(0x06); // Comando SDATAC para detener la lectura continua de datos
  delay(1000);

  // Configurar registros aquí según necesidades específicas
  writeRegister(0x01, 0x95); // CONFIG1: Habilita el oscilador interno

  sendCommand(0x08); // Comando RDATAC para iniciar la lectura continua de datos
  digitalWrite(START_PIN, HIGH); // Iniciar la adquisición de datos
}

void loop() {
  // Esperar a que el pin DRDY indique que los datos están listos para ser leídos
  if (digitalRead(DRDY_PIN) == LOW) {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(0x00); // Comando de lectura

    // Leer los datos de 24 bits de cada canal y enviar a la trama de Serial Plotter
    for (int i = 0; i < 8; i++) { // Suponiendo 8 canales
      long channelData = 0;
      channelData |= (long)SPI.transfer(0x00) << 16;
      channelData |= (long)SPI.transfer(0x00) << 8;
      channelData |= SPI.transfer(0x00);
      Serial.print(channelData);
      if (i < 7) {
        Serial.print(", ");  // Separar los datos de cada canal con comas
      }
    }

    digitalWrite(CS_PIN, HIGH);
    Serial.println();  // Finalizar la línea para actualizar el plotter
  }
}

void sendCommand(byte cmd) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(cmd);
  digitalWrite(CS_PIN, HIGH);
}

void writeRegister(byte reg, byte value) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x40 | reg); // Comando WREG
  SPI.transfer(0x00);       // Número de registros a escribir - 1
  SPI.transfer(value);      // Valor del registro
  digitalWrite(CS_PIN, HIGH);
}