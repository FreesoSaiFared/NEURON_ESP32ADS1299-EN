#include "ESPADS1299.h"
#include <SPI.h>


float values[6]; // Array para almacenar los 6 valores flotantes

ESPADS1299 ADS; // Crear una instancia de ADS1299

unsigned long thisTime;
unsigned long thatTime;
unsigned long elapsedTime;
int resetPin = 15; // Pin usado para iniciar la conversión en modo de lectura continua
int startPin = 16;  // Pin usado para la señal de inicio
int sampleCounter = 0; // Usado para cronometrar el bucle de prueba
boolean testing = true; // Esta bandera se establece en serialEvent al recibir el prompt

const float cutoff_freq = 150.0; // Frecuencia de corte en Hz
const float sampling_time = 0.004; // Tiempo de muestreo en segundos

// Coeficientes del filtro de paso alto
float alpha = 0.9;
float prev_input[6] = {0, 0, 0, 0, 0, 0}; // Valores anteriores de entrada
float prev_output[6] = {0, 0, 0, 0, 0, 0}; // Valores anteriores de salida

float highPassFilter(float input, int channel) {
    float output = alpha * (prev_output[channel] + input - prev_input[channel]);
    prev_input[channel] = input;
    prev_output[channel] = output;
    return output;
}

void setup() {
    // Inicialización del ADS1299
    ADS.initialize(4, 15, 16, 1, false); // (DRDY pin, RST pin, START pin, frecuencia SCK en MHz, isDaisy)
    Serial.begin(115200); // Inicializar la comunicación serial

    // Configuración del ADS1299
    ADS.verbose = true; // Activar verbose para depuración
    ADS.RESET(); // Enviar comando RESET para predeterminar todos los registros
    ADS.SDATAC(); // Salir del modo de lectura continua para comunicarse con el ADS
    configureGain();
    
    // Configuración de registros del ADS1299
    ADS.WREG(CONFIG1, 0b11010110);
    ADS.WREG(CONFIG2, 0b11010101);
    ADS.WREG(CONFIG3, 0b11001010);
    ADS.WREG(CH1SET, 0b10100000);
    ADS.WREG(CH2SET, 0b00100000);
    ADS.WREG(CH3SET, 0b00100000);
    ADS.WREG(CH4SET, 0b00100000);
    ADS.WREG(CH5SET, 0b00100000);
    ADS.WREG(CH6SET, 0b00100000);
    ADS.WREG(CH7SET, 0b00100000);
    ADS.WREG(CH8SET, 0b00100000);
    ADS.WREG(BIAS_SENSP, 0b11111111);
    ADS.WREG(BIAS_SENSN, 0b11111111);
    ADS.WREG(LOFF_SENSP, 0x00);
    ADS.WREG(LOFF_SENSN, 0x00);
    ADS.WREG(CONFIG4, 0x02);

    // Leer todos los registros desde ID hasta CONFIG4
    ADS.RREGS(0x00, 0x17);
    ADS.RDATAC(); // Entrar en modo de lectura continua
    ADS.START(); // Iniciar el muestreo a la velocidad predeterminada

    delay(2000); // Retraso de 2000 milisegundos (2 segundos)
}

void loop() {
    if (testing) {
        if (digitalRead(4) == LOW) { // Esperar a que el pin DRDY indique que los datos están listos
            ADS.updateChannelData(); // Actualizar el array de datos del canal

            // Leer datos de los canales del ADS1299
            int ads_value_2 = ADS.channelData[1];
            int ads_value_3 = ADS.channelData[2];
            int ads_value_4 = ADS.channelData[3];
            int ads_value_5 = ADS.channelData[4];
            int ads_value_6 = ADS.channelData[5];
            int ads_value_7 = ADS.channelData[6];
            int ads_value_8 = ADS.channelData[7];

            // Filtrar los valores leídos
            values[0] = highPassFilter(ads_value_3, 0) / 1000.0;
            values[1] = highPassFilter(ads_value_4, 1) / 1000.0;
            values[2] = highPassFilter(ads_value_5, 2) / 1000.0;
            values[3] = highPassFilter(ads_value_6, 3) / 1000.0;
            values[4] = highPassFilter(ads_value_7, 4) / 1000.0;
            values[5] = highPassFilter(ads_value_8, 5) / 1000.0;

            // Imprimir los valores filtrados en el serial para visualización en el Plotter
            Serial.print(values[0]); Serial.print("\t");
            Serial.print(values[1]); Serial.print("\t");
            Serial.print(values[2]); Serial.print("\t");
            Serial.print(values[3]); Serial.print("\t");
            Serial.print(values[4]); Serial.print("\t");
            Serial.println(values[5]);
        }
    }
}

void configureGain() {
    // Define las ganancias deseadas para cada canal
    byte gain = 0x06; // Ganancia de 24 (binario 110)
    ADS.WREG(CH1SET, gain);
    ADS.WREG(CH2SET, gain);
    ADS.WREG(CH3SET, gain);
    ADS.WREG(CH4SET, gain);
    ADS.WREG(CH5SET, gain);
    ADS.WREG(CH6SET, gain);
    ADS.WREG(CH7SET, gain);
    ADS.WREG(CH8SET, gain);
}

void serialEvent() {
    while (Serial.available()) {      
        char inChar = (char)Serial.read();
        if (inChar == 'x') {
            testing = false;
        }
        if (inChar == 'v') {
            testing = true;
        }
    }
}
