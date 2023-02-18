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
    reset();
}

void ADE7753::end()
{
    _spi->end();
}

void ADE7753::reset()
{
    _writeMaskRegister(ADE7753_MODE, ADE7753_SWRST, 2, ADE7753_SWRST);
    delayMicroseconds(18);
    _writeMaskRegister(ADE7753_MODE, 0x0000, 2, ADE7753_SWRST);
}

uint8_t ADE7753::getVersion()
{
    return _readRegister(ADE7753_DIEREV, 1);
}

void ADE7753::setHpfEnable(bool en)
{
    uint16_t value = en ? 0x0000 : ADE7753_DISHPF;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_DISHPF);
}

void ADE7753::setLpfEnable(bool en)
{
    uint16_t value = en ? 0x0000 : ADE7753_DISLPF2;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_DISLPF2);
}

void ADE7753::setCfEnable(bool en)
{
    uint16_t value = en ? 0x0000 : ADE7753_DISCF;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_DISCF);
}

void ADE7753::setSagEnable(bool en)
{
    uint16_t value = en ? 0x0000 : ADE7753_DISSAG;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_DISSAG);
}

void ADE7753::suspend(bool suspend)
{
    uint16_t value = suspend ? ADE7753_DISSAG : 0x0000;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_DISSAG);
}

void ADE7753::setCycleMode(bool cyc)
{
    uint16_t value = cyc ? ADE7753_CYCMODE : 0x0000;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_CYCMODE);
}

void ADE7753::setCh1Enable(bool en)
{
    uint16_t value = en ? 0x0000 : ADE7753_DISCH1;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_DISCH1);
}

void ADE7753::setCh2Enable(bool en)
{
    uint16_t value = en ? 0x0000 : ADE7753_DISCH2;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_DISCH2);
}

void ADE7753::setSwap(bool swap)
{
    uint16_t value = swap ? ADE7753_SWAP : 0x0000;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_SWAP);
}

void ADE7753::setDataRate(uint16_t dtrt)
{
    _writeMaskRegister(ADE7753_MODE, dtrt, 2, ADE7753_DTRT);
}

void ADE7753::setWaveSelect(uint16_t wavsel)
{
    _writeMaskRegister(ADE7753_MODE, wavsel, 2, ADE7753_WAVSEL);
}

void ADE7753::setPoamEnable(bool en)
{
    uint16_t value = en ? ADE7753_POAM : 0x0000;
    _writeMaskRegister(ADE7753_MODE, value, 2, ADE7753_POAM);
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
