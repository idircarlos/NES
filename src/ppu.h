#ifndef PPU_H
#define PPU_H

#include "types.h"
#include "cartridge.h"

typedef struct {
    Cartridge *cartridge;
    u8 nameTable[2][1024];
    u8 paletteTable[32];
    u8 patternTable[2][4096];   // This table wont be used in the real emulation. Just keep it here for the moment for the design.
} Ppu2C02;

u8 CpuReadFromPpu(u16 addr);
void CpuWriteToPpu(u16 addr, u8 data);

u8 PpuRead(u16 addr);
void PpuWrite(u16 addr, u8 data);

Ppu2C02 *PpuGet();

void PpuConnectCartridge(Cartridge *cartridge);
void PpuClock();

#endif  // PPU_H