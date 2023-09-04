#include "ppu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Color ColorBuild(u8 r, u8 g, u8 b) {
    return (Color){r,g,b,255};
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
    ppu.vramAddr = (LoopyRegister){0};
    ppu.vramAddr.reg = 0x0000;
    ppu.tramAddr = (LoopyRegister){0};
    ppu.tramAddr.reg = 0x0000;
    ppu.fineX = 0x00;

    ppu.bgNextTileId = 0x00;
    ppu.bgNextTileAttr = 0x00;
    ppu.bgNextTileLsb = 0x00;
    ppu.bgNextTileMsb = 0x00;

    ppu.bgShifterPatternLo = 0x0000;
    ppu.bgShifterPatternHi = 0x0000;
    ppu.bgShifterAttribLo = 0x0000;
    ppu.bgShifterAttribHi = 0x0000;

    ppu.nmi = false;
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
                ppu.ppuDataBuffer = PpuRead(ppu.vramAddr.reg);
                if (ppu.vramAddr.reg >= 0x3F00) data = ppu.ppuDataBuffer;
                ppu.vramAddr.reg += ppu.registers.ctrl.bits.incrementMode ? 32 : 1;
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
            ppu.tramAddr.bits.nametableX = ppu.registers.ctrl.bits.nametableX;
            ppu.tramAddr.bits.nametableY = ppu.registers.ctrl.bits.nametableY;
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
            if (ppu.addressLatch == 0) {
                ppu.fineX = data & 0x07;
                ppu.tramAddr.bits.coarseX = data >> 3;
                ppu.addressLatch = 1;
            }
            else {
                ppu.tramAddr.bits.fineY = data & 0x07;
                ppu.tramAddr.bits.coarseY = data >> 3;
                ppu.addressLatch = 0;
            }
            break;
        case 0x0006: // PPU Address
            if (ppu.addressLatch == 0) {
                ppu.tramAddr.reg = (u16)((data & 0x3F) << 8) | (ppu.tramAddr.reg & 0x00FF);
                ppu.addressLatch = 1;
            }
            else {
                ppu.tramAddr.reg = (ppu.tramAddr.reg & 0xFF00) | data;
                ppu.vramAddr = ppu.tramAddr; 
                ppu.addressLatch = 0;
            }
            break;
        case 0x0007: // PPU Data
            PpuWrite(ppu.vramAddr.reg, data);
            ppu.vramAddr.reg += ppu.registers.ctrl.bits.incrementMode ? 32 : 1;
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
        addr &= 0x0FFF;
        if (ppu.cartridge->mirror == VERTICAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) data = ppu.nameTable[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF) data = ppu.nameTable[1][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF) data = ppu.nameTable[0][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF) data = ppu.nameTable[1][addr & 0x03FF];
        }
        else if (ppu.cartridge->mirror == HORIZONTAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) data = ppu.nameTable[0][addr & 0x03FF];
            if (addr >= 0x0400 && addr <= 0x07FF) data = ppu.nameTable[0][addr & 0x03FF];
            if (addr >= 0x0800 && addr <= 0x0BFF) data = ppu.nameTable[1][addr & 0x03FF];
            if (addr >= 0x0C00 && addr <= 0x0FFF) data = ppu.nameTable[1][addr & 0x03FF];
        }
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
        ppu.patternTable[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    }
    // Name table
    else if (addr >= 0x2000 && addr <= 0x3EFF) {
        addr &= 0x0FFF;
        if (ppu.cartridge->mirror == VERTICAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) ppu.nameTable[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF) ppu.nameTable[1][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF) ppu.nameTable[0][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF) ppu.nameTable[1][addr & 0x03FF] = data;
        }
        else if (ppu.cartridge->mirror == HORIZONTAL) {
            if (addr >= 0x0000 && addr <= 0x03FF) ppu.nameTable[0][addr & 0x03FF] = data;
            if (addr >= 0x0400 && addr <= 0x07FF) ppu.nameTable[0][addr & 0x03FF] = data;
            if (addr >= 0x0800 && addr <= 0x0BFF) ppu.nameTable[1][addr & 0x03FF] = data;
            if (addr >= 0x0C00 && addr <= 0x0FFF) ppu.nameTable[1][addr & 0x03FF] = data;
        }
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
                u8 tile_lsb = PpuRead(i * 0x1000 + nOffset + row + 0x0000);
                u8 tile_msb = PpuRead(i * 0x1000 + nOffset + row + 0x0008);
                for (u16 col = 0; col < 8; col++) {
                    u8 pixel = ((tile_lsb & 0x01) << 0) | ((tile_msb & 0x01) << 1);
                    tile_lsb >>= 1; tile_msb >>= 1;
                    Color c = GetColourFromPaletteRam(palette, pixel);
                    SpriteSetPixel(ppu.spritePatternTable[i], nTileX * 8 + (7 - col), nTileY * 8 + row, c);
                }
            }
        }
    }
    return ppu.spritePatternTable[i];
}

void PpuIncrementScrollX() {
    if (ppu.registers.mask.bits.renderBackground || ppu.registers.mask.bits.renderSprites) {
        if (ppu.vramAddr.bits.coarseX == 31) {
            ppu.vramAddr.bits.coarseX = 0;
            ppu.vramAddr.bits.nametableX = ~ppu.vramAddr.bits.nametableX;
        }
        else {
            ppu.vramAddr.bits.coarseX++;
        }
    }
}

void PpuIncrementScrollY() {
    if (ppu.registers.mask.bits.renderBackground || ppu.registers.mask.bits.renderSprites) {
        if (ppu.vramAddr.bits.fineY < 7) {
            ppu.vramAddr.bits.fineY++;
        }
        else {
            ppu.vramAddr.bits.fineY = 0;
            if (ppu.vramAddr.bits.coarseY == 29) {
                ppu.vramAddr.bits.coarseY = 0;
                ppu.vramAddr.bits.nametableY = ~ppu.vramAddr.bits.nametableY;
            }
            else if (ppu.vramAddr.bits.coarseY == 31) {
                ppu.vramAddr.bits.coarseY = 0;
            }
            else {
                ppu.vramAddr.bits.coarseY++;
            }
        }
    }
}

void PpuTransferAddressX() {
    if (ppu.registers.mask.bits.renderBackground || ppu.registers.mask.bits.renderSprites) {
        ppu.vramAddr.bits.nametableX = ppu.tramAddr.bits.nametableX;
        ppu.vramAddr.bits.coarseX = ppu.tramAddr.bits.coarseX;
    }
}

void PpuTransferAddressY() {
    if (ppu.registers.mask.bits.renderBackground || ppu.registers.mask.bits.renderSprites) {
        ppu.vramAddr.bits.fineY = ppu.tramAddr.bits.fineY;
        ppu.vramAddr.bits.nametableY = ppu.tramAddr.bits.nametableY;
        ppu.vramAddr.bits.coarseY = ppu.tramAddr.bits.coarseY;
    }
}

void PpuLoadBackgroundShifters() {
    ppu.bgShifterPatternLo = (ppu.bgShifterPatternLo & 0xFF00) | ppu.bgNextTileLsb;
    ppu.bgShifterPatternHi = (ppu.bgShifterPatternHi & 0xFF00) | ppu.bgNextTileMsb;
    ppu.bgShifterAttribLo = (ppu.bgShifterAttribLo & 0xFF00) | ((ppu.bgNextTileAttr & 0b01) ? 0xFF : 0x00);
    ppu.bgShifterAttribHi = (ppu.bgShifterAttribHi & 0xFF00) | ((ppu.bgNextTileAttr & 0b10) ? 0xFF : 0x00);
}

void PpuUpdateShifters() {
    if (ppu.registers.mask.bits.renderBackground) {
        ppu.bgShifterPatternLo <<= 1;
        ppu.bgShifterPatternHi <<= 1;
        ppu.bgShifterAttribLo <<= 1;
        ppu.bgShifterAttribHi <<= 1;
    }
}

void PpuClock() {
    if (ppu.scanline >= -1 && ppu.scanline < 240) {
        if (ppu.scanline == 0 && ppu.cycle == 0) {
            ppu.cycle = 1;
        }
        if (ppu.scanline == -1 && ppu.cycle == 1) {
            ppu.registers.status.bits.verticalBlank = 0;
        }
        if ((ppu.cycle >= 2 && ppu.cycle < 258) || (ppu.cycle >= 321 && ppu.cycle < 338)) {
            PpuUpdateShifters();
            switch ((ppu.cycle - 1) % 8) {
                case 0:
                    PpuLoadBackgroundShifters();
                    ppu.bgNextTileId = PpuRead(0x2000 | (ppu.vramAddr.reg & 0x0FFF));
                    break;
                case 2:
                    ppu.bgNextTileAttr = PpuRead(0x23C0 | (ppu.vramAddr.bits.nametableY << 11)
                        | (ppu.vramAddr.bits.nametableX << 10)
                        | ((ppu.vramAddr.bits.coarseY >> 2) << 3)
                        | (ppu.vramAddr.bits.coarseX >> 2)); 
                    if (ppu.vramAddr.bits.coarseY & 0x02) ppu.bgNextTileAttr >>= 4;
                    if (ppu.vramAddr.bits.coarseX & 0x02) ppu.bgNextTileAttr >>= 2;
                    ppu.bgNextTileAttr &= 0x03;
                    break;
                case 4:
                    ppu.bgNextTileLsb = PpuRead((ppu.registers.ctrl.bits.patternBackground << 12)
                        + ((u16)ppu.bgNextTileId << 4)
                        + (ppu.vramAddr.bits.fineY) + 0);
                    break;
                case 6:
                    ppu.bgNextTileMsb = PpuRead((ppu.registers.ctrl.bits.patternBackground << 12)
                        + ((u16)ppu.bgNextTileId << 4)
                        + (ppu.vramAddr.bits.fineY) + 8);
                    break;
                case 7:
                    PpuIncrementScrollX();
                    break;
                default:
                    break;
            }
        }
        if (ppu.cycle == 256) {
            PpuIncrementScrollY();
        }
        if (ppu.cycle == 257) {
            PpuLoadBackgroundShifters();
            PpuTransferAddressX();
        }
        if (ppu.cycle == 338 || ppu.cycle == 340) {
            ppu.bgNextTileId = PpuRead(0x2000 | (ppu.vramAddr.reg & 0x0FFF));
        }
        if (ppu.scanline == -1 && ppu.cycle >= 280 && ppu.cycle < 305) {
            PpuTransferAddressY();
        }
    }

    if (ppu.scanline == 240) {
        // Nothing happen here
    }

    if (ppu.scanline >= 241 && ppu.scanline < 261) {
        if (ppu.scanline == 241 && ppu.cycle == 1) {
            ppu.registers.status.bits.verticalBlank = 1;
            if (ppu.registers.ctrl.bits.enableNmi) {
                ppu.nmi = true;
            }
        }
    }

    u8 bgPixel = 0x00;
    u8 bgPalette = 0x00;

    if (ppu.registers.mask.bits.renderBackground) {
        u16 bitMux = 0x8000 >> ppu.fineX;
        u8 p0Pixel = (ppu.bgShifterPatternLo & bitMux) > 0;
        u8 p1Pixel = (ppu.bgShifterPatternHi & bitMux) > 0;
        bgPixel = (p1Pixel << 1) | p0Pixel;

        u8 bgPalette0 = (ppu.bgShifterAttribLo & bitMux) > 0;
        u8 bgPalette1 = (ppu.bgShifterAttribHi & bitMux) > 0;
        bgPalette = (bgPalette1 << 1) | bgPalette0;
    }

    // Update the Sprite screen with the appropiate pixels and palettes
    SpriteSetPixel(ppu.spriteScreen, ppu.cycle - 1, ppu.scanline, GetColourFromPaletteRam(bgPalette, bgPixel));
    
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