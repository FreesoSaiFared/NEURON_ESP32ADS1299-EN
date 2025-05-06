# NEURON_ESP32ADS1299

This project, developed by the GPS UDI research group, is a prototype called NEURON that uses an ESP32 microcontroller and the ADS1299 chip to capture EEG signals. The project includes adaptive filters implemented during the system's development.

## Contents

- [Description](#description)
- [Features](#features)
- [Technical Specifications](#technical-specifications)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Contributions](#contributions)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Description

The goal of this project is to develop a portable, low-cost system for acquiring EEG signals, using the ESP32 microcontroller and Texas Instruments' ADS1299 analog-to-digital converter. This system enables real-time brain signal capture and processing, facilitating analysis for a range of applications in neuroscience and brain-computer interfaces.

## Features

- **EEG Signal Acquisition**: Captures multiple high-precision EEG signal channels.
- **Real-Time Processing**: Utilizes adaptive filters for signal processing.
- **Wireless Connectivity**: Transmits data via Wi-Fi using the ESP32.
- **Portability**: Compact design with efficient power consumption.

## Technical Specifications

- **Microcontroller**: ESP32
- **ADC Converter**: 24-bit ADS1299
- **EEG Channels**: Up to 8 channels
- **Sampling Frequency**: Configurable up to 16 kHz
- **Communication**: SPI between ESP32 and ADS1299; Wi-Fi for data transmission

## Requirements

### Hardware:

- ESP32 development board
- ADS1299 module
- EEG signal capture electrodes
- Suitable power supply

### Software:

- [PlatformIO](https://platformio.org/) or [Arduino IDE](https://www.arduino.cc/en/software)
- Necessary libraries (listed in the `platformio.ini` file or installation instructions)

## Installation

1. **Clone the repository**:

   ```bash
   git clone https://github.com/jcristancho2/NEURON_ESP32ADS1299.git
   ```
2. Make your changes and commit them with descriptive messages.

3. Submit a pull request detailing the changes you’ve made.

Submit a pull request detailing your modifications.

### License

This project is licensed under the MIT License. See the LICENSE file for more information.

### Acknowledgements

We thank the GPS UDI research group, UDITECH, and ZION for their support in developing this project.
