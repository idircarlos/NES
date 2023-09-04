# NES Emulator in C

This is a project for a NES emulator written in pure C. The main goal of this project is to emulate a Nintendo Entertainment System (NES) to run most of the classic games.

## Project Status

Below is a list of features that have been implemented so far and those that are pending:

### CPU (6502)
- [x] Official opcodes
- [ ] Unofficial opcodes (By the moment some NOP)

### RAM
- [x] Mirroring

### PPU (2C02)
- [x] Background
- [ ] Foreground (Sprites)

### Mappers
- [x] Mapper 000 (NROM)
- [ ] Other mappers...

### APU (Audio Processing Unit)
- [ ] APU stuff :D

### Controller
- [x] Keyboard
- [ ] Gamepad

## Compiling and runing

Raylib is the only dependency for the graphics, sound and input. Also, the status of the project is only tested on a Windows machine.
```shell
$ make && ./nes
```