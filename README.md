# cmx918_ssb
A proof-of-concept program to receive SSB via CMX918

Simplified schematic: Antenna - CMX918 (TPs 9,10,11 of the DRM1000 module) -> (SPI slave) RP2350 (I2S master) -> MAX98357 - Speaker

Based on the SPItoMyselfAsync example from the SPISlave Arduino library by Earle F. Philhower, III. ( https://github.com/earlephilhower/arduino-pico )

Demo:
https://www.youtube.com/watch?v=lPEvQfSI_JY&feature=youtu.be

<img width="1271" height="706" alt="image" src="https://github.com/user-attachments/assets/1bc2acfc-d336-40eb-beca-08fa46fc4771" />

See also:

https://github.com/ur8us/cmx918
