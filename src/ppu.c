#include "ppu.h"

static Ppu2C02 ppu = {0};

u8 CpuReadFromPpu(u16 addr) {
    u8 data = 0x00;
    addr &= 0x0007; // PPU is mirrored to the first 8 addresess
    switch (addr) {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }
	return data;
}

void CpuWriteToPpu(u16 addr, u8 data) {
    addr &= 0x0007; // PPU is mirrored to the first 8 addresess
    switch (addr) {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }
}

u8 PpuRead(u16 addr) {
    u8 data = 0x00;
    addr &= 0x3FFF;

    if (CartridgePpuRead(ppu.cartridge, addr, &data)) {

    }

    return data;
}

void PpuWrite(u16 addr, u8 data) {
    addr &= 0x3FFF;

    if (CartridgePpuWrite(ppu.cartridge, addr, data)) {

    }
}

Ppu2C02 *PpuGet() {
    return &ppu;
}

void PpuConnectCartridge(Cartridge *cartridge) {
    ppu.cartridge = cartridge;
}

void PpuClock();