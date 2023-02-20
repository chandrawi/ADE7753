#include "ADE7753.h"

ADE7753 meter;                        // Default constructor
// SPIClass Hspi(HSPI);
// ADE7753 meter(Hspi, 15, 4000000);  // Constructor for non-default SPI bus and frequency

void setup() {

  Serial.begin(500000);
  meter.begin();

  // Set waveform new data interrupt
  meter.setInterrupt(ADE7753_WSMP);

  /*
    Set wave form mode
    ADE7753_WAV_OFF   => disable wave form
    ADE7753_WAV_PWR   => instantaneous power
    ADE7753_WAV_CH1   => current
    ADE7753_WAV_CH2   => voltage
  */
  meter.setWaveSelect(ADE7753_WAV_CH2);

  /*
    Set wave form data rate
    ADE7753_DR_1_128  => 1 / 128 clock (27.9 kSPS)
    ADE7753_DR_1_256  => 1 / 256 clock (14 kSPS)
    ADE7753_DR_1_512  => 1 / 512 clock (7 kSPS)
    ADE7753_DR_1_1024 => 1 / 1024 clock (3.5 kSPS)
  */
  meter.setDataRate(ADE7753_DR_1_1024);

  // Set channel 1 and channel 2 gain
  meter.setCh1Gain(ADE7753_GAIN_8);
  meter.setCh2Gain(ADE7753_GAIN_2);
}

void loop() {

  // Check new waveform data interrupt status
  if (meter.status() & ADE7753_WSMP) {
    // Show waveform value in serial plotter
    int32_t value = meter.readWaveForm();
    Serial.println(value);
  }
}
