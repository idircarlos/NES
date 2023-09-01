#include "ppu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Color ColorBuild(u8 r, u8 g, u8 b) {
    return (Color){r,g,b,255};
}

Color ColorBuildA(u8 r, u8 g, u8 b, u8 a) {
    return (Color){r,g,b,a};
}

static Ppu2C02 ppu = {0};

void PpuInit() {
    ppu.paletteScreen[0x00] = ColorBuild(84, 84, 84);
	ppu.paletteScreen[0x01] = ColorBuild(0, 30, 116);
	ppu.paletteScreen[0x02] = ColorBuild(8, 16, 144);
	ppu.paletteScreen[0x03] = ColorBuild(48, 0, 136);
	ppu.paletteScreen[0x04] = ColorBuild(68, 0, 100);
	ppu.paletteScreen[0x05] = ColorBuild(92, 0, 48);
	ppu.paletteScreen[0x06] = ColorBuild(84, 4, 0);
	ppu.paletteScreen[0x07] = ColorBuild(60, 24, 0);
	ppu.paletteScreen[0x08] = ColorBuild(32, 42, 0);
	ppu.paletteScreen[0x09] = ColorBuild(8, 58, 0);
	ppu.paletteScreen[0x0A] = ColorBuild(0, 64, 0);
	ppu.paletteScreen[0x0B] = ColorBuild(0, 60, 0);
	ppu.paletteScreen[0x0C] = ColorBuild(0, 50, 60);
	ppu.paletteScreen[0x0D] = ColorBuild(0, 0, 0);
	ppu.paletteScreen[0x0E] = ColorBuild(0, 0, 0);
	ppu.paletteScreen[0x0F] = ColorBuild(0, 0, 0);

	ppu.paletteScreen[0x10] = ColorBuild(152, 150, 152);
	ppu.paletteScreen[0x11] = ColorBuild(8, 76, 196);
	ppu.paletteScreen[0x12] = ColorBuild(48, 50, 236);
	ppu.paletteScreen[0x13] = ColorBuild(92, 30, 228);
	ppu.paletteScreen[0x14] = ColorBuild(136, 20, 176);
	ppu.paletteScreen[0x15] = ColorBuild(160, 20, 100);
	ppu.paletteScreen[0x16] = ColorBuild(152, 34, 32);
	ppu.paletteScreen[0x17] = ColorBuild(120, 60, 0);
	ppu.paletteScreen[0x18] = ColorBuild(84, 90, 0);
	ppu.paletteScreen[0x19] = ColorBuild(40, 114, 0);
	ppu.paletteScreen[0x1A] = ColorBuild(8, 124, 0);
	ppu.paletteScreen[0x1B] = ColorBuild(0, 118, 40);
	ppu.paletteScreen[0x1C] = ColorBuild(0, 102, 120);
	ppu.paletteScreen[0x1D] = ColorBuild(0, 0, 0);
	ppu.paletteScreen[0x1E] = ColorBuild(0, 0, 0);
	ppu.paletteScreen[0x1F] = ColorBuild(0, 0, 0);

	ppu.paletteScreen[0x20] = ColorBuild(236, 238, 236);
	ppu.paletteScreen[0x21] = ColorBuild(76, 154, 236);
	ppu.paletteScreen[0x22] = ColorBuild(120, 124, 236);
	ppu.paletteScreen[0x23] = ColorBuild(176, 98, 236);
	ppu.paletteScreen[0x24] = ColorBuild(228, 84, 236);
	ppu.paletteScreen[0x25] = ColorBuild(236, 88, 180);
	ppu.paletteScreen[0x26] = ColorBuild(236, 106, 100);
	ppu.paletteScreen[0x27] = ColorBuild(212, 136, 32);
	ppu.paletteScreen[0x28] = ColorBuild(160, 170, 0);
	ppu.paletteScreen[0x29] = ColorBuild(116, 196, 0);
	ppu.paletteScreen[0x2A] = ColorBuild(76, 208, 32);
	ppu.paletteScreen[0x2B] = ColorBuild(56, 204, 108);
	ppu.paletteScreen[0x2C] = ColorBuild(56, 180, 204);
	ppu.paletteScreen[0x2D] = ColorBuild(60, 60, 60);
	ppu.paletteScreen[0x2E] = ColorBuild(0, 0, 0);
	ppu.paletteScreen[0x2F] = ColorBuild(0, 0, 0);

	ppu.paletteScreen[0x30] = ColorBuild(236, 238, 236);
	ppu.paletteScreen[0x31] = ColorBuild(168, 204, 236);
	ppu.paletteScreen[0x32] = ColorBuild(188, 188, 236);
	ppu.paletteScreen[0x33] = ColorBuild(212, 178, 236);
	ppu.paletteScreen[0x34] = ColorBuild(236, 174, 236);
	ppu.paletteScreen[0x35] = ColorBuild(236, 174, 212);
	ppu.paletteScreen[0x36] = ColorBuild(236, 180, 176);
	ppu.paletteScreen[0x37] = ColorBuild(228, 196, 144);
	ppu.paletteScreen[0x38] = ColorBuild(204, 210, 120);
	ppu.paletteScreen[0x39] = ColorBuild(180, 222, 120);
	ppu.paletteScreen[0x3A] = ColorBuild(168, 226, 144);
	ppu.paletteScreen[0x3B] = ColorBuild(152, 226, 180);
	ppu.paletteScreen[0x3C] = ColorBuild(160, 214, 228);
	ppu.paletteScreen[0x3D] = ColorBuild(160, 162, 160);
	ppu.paletteScreen[0x3E] = ColorBuild(0, 0, 0);
	ppu.paletteScreen[0x3F] = ColorBuild(0, 0, 0);

    ppu.spriteScreen = SpriteCreate(256, 240);
    ppu.spriteNameTable[0] = SpriteCreate(256, 240);
    ppu.spriteNameTable[1] = SpriteCreate(256, 240);
    ppu.spritePatternTable[0] = SpriteCreate(128, 128);
    ppu.spritePatternTable[1] = SpriteCreate(128, 128);
    ppu.scanline = 0;
    ppu.cycle = 0;
    ppu.frameCompleted = false;
}

Sprite *SpriteCreate(u16 width, u16 height) {
    Sprite *sprite = (Sprite*)malloc(sizeof(sprite));
    sprite->width = width;
    sprite->height = height;
    sprite->pixels = (Color*)malloc(width*height*sizeof(Color));
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            SpriteSetPixel(sprite, i, j, BLACK);
        }
    }
    return sprite;
}

Color SpriteGetPixel(Sprite *sprite, u16 x, u16 y) {
    return sprite->pixels[x*sprite->height + y];
}

bool SpriteSetPixel(Sprite *sprite, u16 x, u16 y, Color color) {
    if (x >= 0 && x < sprite->width && y >= 0 && y < sprite->height) {
        sprite->pixels[x*sprite->height + y] = color;
        return true;
    }
    return false;
}

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

void PpuClock() {
    // Fake some noise
    SpriteSetPixel(ppu.spriteScreen, ppu.cycle - 1, ppu.scanline, ppu.paletteScreen[(rand() % 2) ? 0x3F : 0x30]);
    
    // Advance renderer - it never stops, it's relentless
	ppu.cycle++;
	if (ppu.cycle >= 341) {
		ppu.cycle = 0;
		ppu.scanline++;
		if (ppu.scanline >= 261) {
			ppu.scanline = -1;
			ppu.frameCompleted = true;
		}
	}
}