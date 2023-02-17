#ifndef ADE7753_H
#define ADE7753_H

#include <Arduino.h>
#include <SPI.h>

#define ADE7753_SPI_FREQ    F_CPU / 4
#define ADE7753_TRA_DEL     4

class ADE7753
{
public:

    ADE7753(SPIClass &Spi, uint8_t nss, uint32_t spiFreq = ADE7753_SPI_FREQ);
    ADE7753(uint8_t nss, uint32_t spiFreq = ADE7753_SPI_FREQ);
    ADE7753();

    void begin();
    void end();

private:

    SPIClass* _spi;
    uint8_t _nss;
    uint32_t _spiFreq;

    void _transfer(uint8_t address, uint8_t* data, uint8_t length);
};

#endif