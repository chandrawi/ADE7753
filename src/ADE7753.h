#ifndef ADE7753_H
#define ADE7753_H

#include <Arduino.h>
#include <SPI.h>

//Register addresses
//------Name----------------Address----No.Bits(Bytes)
#define ADE7753_WAVEFORM    0x01    // 24 (3)
#define ADE7753_AENERGY     0x02    // 24 (3)
#define ADE7753_RAENERGY    0x03    // 24 (3)
#define ADE7753_LAENERGY    0x04    // 24 (3)
#define ADE7753_VAENERGY    0x05    // 24 (3)
#define ADE7753_RVAENERGY   0x06    // 24 (3)
#define ADE7753_LVAENERGY   0x07    // 24 (3)
#define ADE7753_LVARENERGY  0x08    // 24 (3)
#define ADE7753_MODE        0x09    // 16 (2)
#define ADE7753_IRQEN       0x0A    // 16 (2)
#define ADE7753_STATUS      0x0B    // 16 (2)
#define ADE7753_RSTSTATUS   0x0C    // 16 (2)
#define ADE7753_CH1OS       0x0D    // 8  (1)
#define ADE7753_CH2OS       0x0E    // 8  (1)
#define ADE7753_GAIN        0x0F    // 8  (1)
#define ADE7753_PHCAL       0x10    // 6  (1)
#define ADE7753_APOS        0x11    // 16 (2)
#define ADE7753_WGAIN       0x12    // 12 (2)
#define ADE7753_WDIV        0x13    // 8  (1)
#define ADE7753_CFNUM       0x14    // 12 (2)
#define ADE7753_CFDEN       0x15    // 12 (2)
#define ADE7753_IRMS        0x16    // 24 (3)
#define ADE7753_VRMS        0x17    // 24 (3)
#define ADE7753_IRMSOS      0x18    // 12 (2)
#define ADE7753_VRMSOS      0x19    // 12 (2)
#define ADE7753_VAGAIN      0x1A    // 12 (2)
#define ADE7753_VADIV       0x1B    // 8  (1)
#define ADE7753_LINECYC     0x1C    // 16 (2)
#define ADE7753_ZXTOUT      0x1D    // 12 (2)
#define ADE7753_SAGCYC      0x1E    // 8  (1)
#define ADE7753_SAGLVL      0x1F    // 8  (1)
#define ADE7753_IPKLVL      0x20    // 8  (1)
#define ADE7753_VPKLVL      0x21    // 8  (1)
#define ADE7753_IPEAK       0x22    // 24 (3)
#define ADE7753_RSTIPEAK    0x23    // 24 (3)
#define ADE7753_VPEAK       0x24    // 24 (3)
#define ADE7753_RSTVPEAK    0x25    // 24 (3)
#define ADE7753_TEMP        0x26    // 8  (1)
#define ADE7753_PERIOD      0x27    // 16 (2)
#define ADE7753_TMODE       0x3D    // 8  (1)
#define ADE7753_CHKSUM      0x3E    // 6  (1)
#define ADE7753_DIEREV      0X3F    // 8  (1)

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

    uint32_t _readRegister(uint8_t address, uint8_t length);
    void _writeRegister(uint8_t address, uint16_t value, uint8_t length);
    void _writeMaskRegister(uint8_t address, uint16_t value, uint8_t length, uint16_t mask);
    void _transfer(uint8_t address, uint8_t* data, uint8_t length);
};

#endif