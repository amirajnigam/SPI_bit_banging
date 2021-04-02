# SPI_bit_banging

There is an external target device operating as a slave on a Serial Peripheral Interface ( SPI ) bus. This target device is one of several devices acting as slaves on the SPI bus. The 32-bit host microcontroller runs at 100 MHz but has no internal SPI peripheral to communicate with this device. The host microcontroller has IO ports configured as general purpose IO pins which are accessible as 8 bit values. The maximum SPI clock for the target device is 400 kHz. The SPI clock idles in the High state and all SPI bus slave selections are active in the Low state. Data is sampled on the rising edge of the clock without delay. SPI data communication is most significant bit first.

The target device uses 8 bit register numbers to identify internal 16 bit registers. Each communication with the target device begins by presenting a register number on the SPI bus. The target device then requires a one millisecond delay before the register data contents can be transferred. The SPI bus is not available during this delay period.

Write the host microcontroller ‘C’ language code to monitor the buttons of the target device and update the Digital Outputs as quickly as possible using the information and GPIO/timing “black box” API functions provided.
