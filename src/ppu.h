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

typedef union {
    struct {
        u8 nametableX : 1;
        u8 nametableY : 1;
        u8 incrementMode : 1;
        u8 patternSprite : 1;
        u8 patternBackground : 1;
        u8 spriteSize : 1;
        u8 slaveMode : 1;   // unused
        u8 enableNmi : 1;
    } bits;
    u8 reg;
} PpuCtrl;

typedef union {
    struct {
        u8 unused : 5;
        u8 spriteOverflow : 1;
        u8 spriteZeroHit : 1;
        u8 verticalBlank : 1;
    } bits;
    u8 reg;
} PpuStatus;

typedef union {
    struct {
        u8 grayscale : 1;
        u8 renderBackgroundLeft : 1;
        u8 renderSpritesLeft : 1;
        u8 renderBackground : 1;
        u8 renderSprites : 1;
        u8 enhanceRed : 1;
        u8 enhanceGreen : 1;
        u8 enhanceBlue : 1;
    } bits;
    u8 reg;
} PpuMask;

typedef struct {
    PpuCtrl ctrl;
    PpuStatus status;
    PpuMask mask;
} PpuRegisters;

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
    PpuRegisters registers;
    // Internal communications
	u8 addressLatch;
	u8 ppuDataBuffer;
    u16 ppuAddr;
} Ppu2C02;

void PpuInit();

u8 CpuReadFromPpu(u16 addr, bool readOnly);
void CpuWriteToPpu(u16 addr, u8 data);

u8 PpuRead(u16 addr);
void PpuWrite(u16 addr, u8 data);

Ppu2C02 *PpuGet();

void PpuConnectCartridge(Cartridge *cartridge);
void PpuClock();

Color GetColourFromPaletteRam(u8 palette, u8 pixel);
Sprite *GetPatternTable(u8 i, u8 palette);

#endif  // PPU_H