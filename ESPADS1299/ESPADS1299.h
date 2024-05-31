#ifndef ESPADS1299_h
#define ESPADS1299_h

#include <Arduino.h>
#include <SPI.h>
#include "Definitions.h"

class ESPADS1299 {
public:
    void initialize(int _DRDY, int _RST, int _startPin, int _FREQ, boolean _isDaisy);
    void WAKEUP();
    void STANDBY();
    void RESET();
    void START();
    void STOP();
    void RDATAC();
    void SDATAC();
    byte getDeviceID();
    byte RREG(byte _address);
    void RREGS(byte _address, byte _numRegistersMinusOne);
    void WREG(byte _address, byte _value);
    void WREGS(byte _address, byte _numRegistersMinusOne);
    void updateChannelData();
    void RDATA();
    void printRegisterName(byte _address);
    void printHex(byte _data);

    boolean verbose;
    long channelData[16];

private:
    boolean isDaisy;
    int DRDY, RST, startPin;
    int FREQ;
    byte regData[24];
    unsigned long stat_1, stat_2;

    SPISettings _spisettings;
    SPIClass spi;
};
#endif
