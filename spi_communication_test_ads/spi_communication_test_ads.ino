#include <SPI.h>

// Pin definitions
#define CS_PIN 5       // Chip Select pin for the ADS1299
#define DRDY_PIN 4     // Data Ready pin, indicates when data is available
#define RESET_PIN 15   // RESET pin
#define START_PIN 16   // START pin to begin data acquisition

void setup() {
  Serial.begin(115200);  // Start serial communication
  pinMode(CS_PIN, OUTPUT);
  pinMode(DRDY_PIN, INPUT);
  pinMode(START_PIN, OUTPUT);
  pinMode(RESET_PIN, OUTPUT);

  digitalWrite(CS_PIN, HIGH); // Deactivate the ADS1299
  digitalWrite(RESET_PIN, HIGH); // Keep the RESET pin high
  digitalWrite(START_PIN, LOW); // Ensure START is low

  // Start SPI communication
  SPI.begin(18, 19, 23); // Set SPI pins (SCK = 18, MISO = 19, MOSI = 23)
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1)); // 4 MHz, MSB first, SPI Mode 1

  delay(1000); // Small delay for stabilization

  // Reset the ADS1299
  digitalWrite(RESET_PIN, LOW);
  delay(1); // Keep RESET low for at least 18 tCLK cycles
  digitalWrite(RESET_PIN, HIGH);
  delay(10); // Wait after reset

  // Send configuration commands to the ADS1299
  sendCommand(0x06); // SDATAC command: Stop continuous data reading
  delay(1000);

  // Configure registers here as needed
  writeRegister(0x01, 0x95); // CONFIG1: Enables internal oscillator

  sendCommand(0x08); // RDATAC command: Start continuous data reading
  digitalWrite(START_PIN, HIGH); // Start data acquisition
}

void loop() {
  // Wait for DRDY pin to indicate data is ready to be read
  if (digitalRead(DRDY_PIN) == LOW) {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(0x00); // Read command

    // Read 24-bit data from each channel and send to Serial Plotter
    for (int i = 0; i < 8; i++) { // Assuming 8 channels
      long channelData = 0;
      channelData |= (long)SPI.transfer(0x00) << 16;
      channelData |= (long)SPI.transfer(0x00) << 8;
      channelData |= SPI.transfer(0x00);
      Serial.print(channelData);
      if (i < 7) {
        Serial.print(", ");  // Separate data from each channel with commas
      }
    }

    digitalWrite(CS_PIN, HIGH);
    Serial.println();  // End the line to update the plotter
  }
}

void sendCommand(byte cmd) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(cmd);
  digitalWrite(CS_PIN, HIGH);
}

void writeRegister(byte reg, byte value) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x40 | reg); // WREG command
  SPI.transfer(0x00);       // Number of registers to write minus 1
  SPI.transfer(value);      // Register value
  digitalWrite(CS_PIN, HIGH);
}
