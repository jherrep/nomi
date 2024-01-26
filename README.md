# Nomi

USB-C QMK based keyboard using the RP2040

* Keyboard Maintainer: [jherrep](https://github.com/jherrep)

## Description

Keyboard is based in the Xiao RP2040, mainly because it uses USB-C and has a
small footprint. To work around the small number of GPIOs, SPI IO expanders are
being used.

### Hardware

The schematics and gerber files can be found in the `pcb` directory.

### Firmware

Firmware is powered by QMK, can be found in the `fw` directory. You'll need to
copy the `fw` directory to `qmk_firmware/keyboards/jherrep/nomi`

![Nomi](https://i.imgur.com/hfH5xRT.jpeg)
