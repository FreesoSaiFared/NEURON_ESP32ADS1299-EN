# NEURON_ESP32ADS1299

Este proyecto, desarrollado por el grupo de investigación GPS UDI, consiste en un prototipo denominado NEURON que utiliza un ESP32 y el ADS1299 para la captura de señales EEG. En este proyecto se implementan filtros adaptativos en el proceso de construcción.

## Contenido

- [Descripción](#descripción)
- [Características](#características)
- [Especificaciones Técnicas](#especificaciones-técnicas)
- [Requisitos](#requisitos)
- [Instalación](#instalación)
- [Uso](#uso)
- [Contribuciones](#contribuciones)
- [Licencia](#licencia)
- [Agradecimientos](#agradecimientos)

## Descripción

El objetivo de este proyecto es desarrollar un sistema portátil y de bajo costo para la adquisición de señales EEG, utilizando el microcontrolador ESP32 y el convertidor analógico-digital ADS1299 de Texas Instruments. Este sistema permite la captura y procesamiento de señales cerebrales en tiempo real, facilitando su análisis para diversas aplicaciones en neurociencia e interfaces cerebro-computadora.

## Características

- **Adquisición de señales EEG**: Captura de múltiples canales de señales electroencefalográficas con alta precisión.
- **Procesamiento en tiempo real**: Implementación de filtros adaptativos para el procesamiento de señales.
- **Conectividad inalámbrica**: Transmisión de datos mediante Wi-Fi utilizando el ESP32.
- **Portabilidad**: Diseño compacto y eficiente en consumo energético.

## Especificaciones Técnicas

- **Microcontrolador**: ESP32
- **Convertidor ADC**: ADS1299 de 24 bits
- **Canales EEG**: Hasta 8 canales
- **Frecuencia de muestreo**: Configurable hasta 16 kHz
- **Comunicación**: SPI entre ESP32 y ADS1299; Wi-Fi para transmisión de datos

## Requisitos

### Hardware:

- Placa de desarrollo ESP32
- Módulo ADS1299
- Electrodos para captura de señales EEG
- Fuente de alimentación adecuada

### Software:

- [PlatformIO](https://platformio.org/) o [Arduino IDE](https://www.arduino.cc/en/software)
- Librerías necesarias (detalladas en el archivo `platformio.ini` o en las instrucciones de instalación)

## Instalación

1. **Clonar el repositorio**:

   ```bash
   git clone https://github.com/jcristancho2/NEURON_ESP32ADS1299.git
   ```

2. **Configurar el entorno de desarrollo**:

   - Si utilizas PlatformIO:
     - Abre el proyecto en PlatformIO.
     - Las dependencias se instalarán automáticamente según el archivo `platformio.ini`.

   - Si utilizas Arduino IDE:
     - Asegúrate de tener instalado el soporte para ESP32. Puedes seguir [esta guía](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md).
     - Instala las librerías necesarias manualmente (consulta el código para identificar las dependencias).

3. **Cargar el código en el ESP32**:

   - Conecta el ESP32 a tu computadora mediante un cable USB.
   - Selecciona el puerto y la placa adecuados en tu entorno de desarrollo.
   - Compila y carga el código en el ESP32.

## Uso

1. **Configuración del hardware**:

   - Conecta los electrodos en las posiciones adecuadas para la captura de señales EEG.
   - Asegúrate de que el ADS1299 esté correctamente conectado al ESP32 mediante la interfaz SPI.

2. **Ejecución**:

   - Una vez cargado el código y con el hardware configurado, enciende el dispositivo.
   - El ESP32 iniciará la captura de señales EEG y las procesará en tiempo real utilizando los filtros adaptativos implementados.
   - Los datos pueden ser transmitidos inalámbricamente para su visualización o almacenamiento.

## Contribuciones

Las contribuciones son bienvenidas. Si deseas colaborar en este proyecto, por favor sigue estos pasos:

1. **Fork** el repositorio.
2. Crea una nueva rama con una descripción clara de tu función:
   ```bash
   git checkout -b feature/nueva-funcionalidad
   ```
3. Realiza tus cambios y haz commits con mensajes descriptivos.
4. Envía un **pull request** detallando las modificaciones realizadas.

## Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo `LICENSE` para más información.

## Agradecimientos

Agradecemos al grupo de investigación GPS UDI , UDITECH, ZION por su apoyo en el desarrollo de este proyecto.
