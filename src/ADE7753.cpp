#include <ADE7753.h>

ADE7753::ADE7753(SPIClass &Spi, uint8_t nss, uint32_t spiFreq)
{
    _spi = &Spi;
    _nss = nss;
    _spiFreq = spiFreq;
}

ADE7753::ADE7753(uint8_t nss, uint32_t spiFreq)
{
    _spi = &SPI;
    _nss = nss;
    _spiFreq = spiFreq;
}

ADE7753::ADE7753()
{
    _spi = &SPI;
    _nss = SS;
    _spiFreq = ADE7753_SPI_FREQ;
}

void ADE7753::begin()
{
    pinMode(_nss, OUTPUT);
    _spi->begin();
}

void ADE7753::end()
{
    _spi->end();
}

uint32_t ADE7753::_readRegister(uint8_t address, uint8_t length)
{
    // Hold max 24-bit data (3 bytes)
    uint8_t data[3];
    _transfer(address, data, length);
    // Combine register value from SPI transfer data
    uint32_t value = 0;
    for (int8_t i=0; i<length; i++) {
        value = value << 8 | data[i];
    }
    // Convert 8, 16, 24 bit negative number to 32 bit negative number
    uint8_t nbits = length * 8;
    if (value & ((uint32_t) 1 << (nbits - 1))) {
        return value | (0xFFFFFFFF << nbits);
    }
    return value;
}

void ADE7753::_writeRegister(uint8_t address, uint16_t value, uint8_t length)
{
    // Hold max 16-bit data (2 bytes)
    uint8_t data[2];
    // Break value for SPI transfer
    for (int8_t i=length-1; i>=0; i--) {
        data[i] = value;
        value = value >> 8;
    }
    _transfer(address | 0x80, data, length);
}

void ADE7753::_writeMaskRegister(uint8_t address, uint16_t value, uint8_t length, uint16_t mask)
{
    uint16_t reg = _readRegister(address, length);
    // Replace some bits in register with input value
    reg = (reg & ~mask) | (value & mask);
    _writeRegister(address, reg, length);
}

void ADE7753::_transfer(uint8_t address, uint8_t* data, uint8_t length)
{
    digitalWrite(_nss, LOW);
    _spi->beginTransaction(SPISettings(_spiFreq, MSBFIRST, SPI_MODE2));
    _spi->transfer(address);
    delayMicroseconds(ADE7753_TRA_DEL);
    for (int8_t i=0; i<length; i++) {
        data[i] = _spi->transfer(data[i]);
    }
    _spi->endTransaction();
    digitalWrite(_nss, HIGH);
}
