# CMX918 in SSB mode

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

The very first use of the CMX918 chip to receive SSB. Demodulator is based on
the RP2350 microprocessor with an output to MAX98357A D-class audio amplifier.
Antenna is 1m wire.

## Demo

[CMX918 receiving FT8 in SSB mode on the 20m band.](https://www.youtube.com/watch?v=lPEvQfSI_JY)

<img width="1271" height="706" alt="CMX918 SSB receiver demonstration" src="https://github.com/user-attachments/assets/1bc2acfc-d336-40eb-beca-08fa46fc4771" />

## Signal path

`1m wire antenna -> CMX918 -> RP2350 -> MAX98357A -> speaker`

The RP2350 receives I/Q samples from test points 9, 10, and 11 of the DRM1000
module over SPI, demodulates SSB, and sends the audio samples to the MAX98357A
over I2S.

The sketch is based on the
[`SPItoMyselfAsync`](https://github.com/earlephilhower/arduino-pico) example from
the SPISlave Arduino library by Earle F. Philhower, III.

## Related project

See the [CMX918 DRM1000 project](https://github.com/ur8us/cmx918).

## License

This project is licensed under the [MIT License](LICENSE).
