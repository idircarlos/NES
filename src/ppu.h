#ifndef PPU_H
#define PPU_H

#include "types.h"
#include "cartridge.h"
#include <raylib.h>

typedef struct {
    Color *pixels;
    u16 width;
    u16 height;
} Sprite;

Sprite *SpriteCreate(u16 width, u16 height);
Color SpriteGetPixel(Sprite *sprite, u16 x, u16 y);
bool SpriteSetPixel(Sprite *sprite, u16 x, u16 y, Color color);

typedef struct {
    Cartridge *cartridge;
    u8 nameTable[2][1024];
    u8 paletteTable[32];
    u8 patternTable[2][4096];   // This table wont be used in the real emulation. Just keep it here for the moment for the design.
    Color paletteScreen[64];
    Sprite *spriteScreen;
    Sprite *spriteNameTable[2];
    Sprite *spritePatternTable[2];
    u16 scanline;
    u16 cycle;
    bool frameCompleted;
} Ppu2C02;

void PpuInit();

u8 CpuReadFromPpu(u16 addr);
void CpuWriteToPpu(u16 addr, u8 data);

u8 PpuRead(u16 addr);
void PpuWrite(u16 addr, u8 data);

Ppu2C02 *PpuGet();

void PpuConnectCartridge(Cartridge *cartridge);
void PpuClock();

#endif  // PPU_H