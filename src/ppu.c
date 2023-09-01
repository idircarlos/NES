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

    ppu.registers.ctrl = (PpuCtrl){0};
    ppu.registers.status = (PpuStatus){0};
    ppu.registers.mask = (PpuMask){0};

    ppu.addressLatch = 0x00;
    ppu.ppuDataBuffer = 0x00;
    ppu.ppuAddr = 0x0000;
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

u8 CpuReadFromPpu(u16 addr, bool readOnly) {
    u8 data = 0x00;
    addr &= 0x0007; // PPU is mirrored to the first 8 addresess
    if (readOnly) {
        switch (addr) {
            case 0x0000: // Control
                data = ppu.registers.ctrl.reg;
                break;
            case 0x0001: // Mask
                data = ppu.registers.mask.reg;
                break;
            case 0x0002: // Status
                data = ppu.registers.status.reg;
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
    else {
        switch (addr) {
		
            // Control - Not readable
            case 0x0000: break;
            
            // Mask - Not Readable
            case 0x0001: break;
            
            // Status
            case 0x0002:
                ppu.registers.status.bits.verticalBlank = 1;
                data = (ppu.registers.status.reg & 0xE0) | (ppu.ppuDataBuffer & 0x1F);
                ppu.registers.status.bits.verticalBlank = 0;
                ppu.addressLatch = 0;
                break;

            // OAM Address
            case 0x0003: break;

            // OAM Data
            case 0x0004: break;

            // Scroll - Not Readable
            case 0x0005: break;

            // PPU Address - Not Readable
            case 0x0006: break;

            // PPU Data
            case 0x0007: 
                data = ppu.ppuDataBuffer;
                ppu.ppuDataBuffer = PpuRead(ppu.ppuAddr);
                if (ppu.ppuAddr >= 0x3F00) data = ppu.ppuDataBuffer;
                ppu.ppuAddr += (ppu.registers.ctrl.bits.incrementMode ? 32 : 1);
                break;
		}
	}
    return data;
}

void CpuWriteToPpu(u16 addr, u8 data) {
    addr &= 0x0007; // PPU is mirrored to the first 8 addresess
    switch (addr) {
        case 0x0000: // Control
            ppu.registers.ctrl.reg = data;
            break;
        case 0x0001: // Mask
            ppu.registers.mask.reg = data;
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
            if (ppu.addressLatch == 0) {
                ppu.ppuAddr = (u16)((data & 0x3F) << 8) | (ppu.ppuAddr & 0x00FF);
                ppu.addressLatch = 1;
            }
            else {
                ppu.ppuAddr = (ppu.ppuAddr & 0xFF00) | data;
                ppu.addressLatch = 0;
            }
            break;
        case 0x0007: // PPU Data
            PpuWrite(ppu.ppuAddr, data);
            ppu.ppuAddr += (ppu.registers.ctrl.bits.incrementMode ? 32 : 1);
            break;
    }
}

u8 PpuRead(u16 addr) {
    u8 data = 0x00;
    addr &= 0x3FFF;

    if (CartridgePpuRead(ppu.cartridge, addr, &data)) {

    }
    // Pattern table
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // Get the most significant bit of the address and offsets by the rest of the bits of the address
        data = ppu.patternTable[(addr & 0x1000) >> 12][addr & 0x0FFF];
    }
    // Name table
    else if (addr >= 0x2000 && addr <= 0x3EFF) {

    }
    // Palette
    else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F; // Mask less significant 5 bits
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        data = ppu.paletteTable[addr];
    }

    return data;
}

void PpuWrite(u16 addr, u8 data) {
    addr &= 0x3FFF;

    if (CartridgePpuWrite(ppu.cartridge, addr, data)) {

    }
    // Pattern table
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // This memory acts as a ROM for the PPU,
        // but for som NES ROMs, this memory is a RAM.
        ppu.patternTable[(addr & 0x1000) >> 12][addr & 0x0FFF] =  data;
    }
    // Name table
    else if (addr >= 0x2000 && addr <= 0x3EFF) {

    }
    // Palette
    else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        addr &= 0x001F; // Mask less significant 5 bits
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;
        ppu.paletteTable[addr] = data;
    }
}

Ppu2C02 *PpuGet() {
    return &ppu;
}

void PpuConnectCartridge(Cartridge *cartridge) {
    ppu.cartridge = cartridge;
}

Color GetColourFromPaletteRam(u8 palette, u8 pixel) {
    return ppu.paletteScreen[PpuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

Sprite *GetPatternTable(u8 i, u8 palette) {
    for (u16 nTileY = 0; nTileY < 16; nTileY++) {
		for (u16 nTileX = 0; nTileX < 16; nTileX++) {
			u16 nOffset = nTileY * 256 + nTileX * 16;

			// Now loop through 8 rows of 8 pixels (Tile)
			for (u16 row = 0; row < 8; row++) {
				uint8_t tile_lsb = PpuRead(i * 0x1000 + nOffset + row + 0x0000);
				uint8_t tile_msb = PpuRead(i * 0x1000 + nOffset + row + 0x0008);

				for (uint16_t col = 0; col < 8; col++) {
					uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);
					tile_lsb >>= 1; tile_msb >>= 1;
					SpriteSetPixel(ppu.spritePatternTable[i], nTileX * 8 + (7 - col), nTileY * 8 + row, GetColourFromPaletteRam(palette, pixel));
				}
			}
		}
	}

	return ppu.spritePatternTable[i];
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