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
