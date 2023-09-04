#include "demo.h"
#include "types.h"
#include "cpu.h"
#include <raylib.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEX_CHARS "0123456789ABCDEF"

static char* mapAsm[0xFFFF] = {0};
static Cpu6502 *cpu = {0};
static Ppu2C02 *ppu = {0};
static bool emulationRun = false;
static float residualTime = 0.0f;
static u8 selectedPalette = 0;

static char* hex(u32 n, u8 d, char *dst) {
    strset(dst, 0);
    int i;
    dst[d] = 0;
    for (i = d - 1; i >= 0; i--, n >>= 4) {
        dst[i] = HEX_CHARS[n & 0xF];
    }
    return dst;
}

void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
    int nRamX = x, nRamY = y;
    char hex_aux[16];
    char sOffSet[1024];
    for (int row = 0; row < nRows; row++) {
        strset(sOffSet, 0);
        strcat(sOffSet, "$");
        strcat(sOffSet, hex(nAddr, 4, hex_aux));
        strcat(sOffSet, ":");
        for (int col = 0; col < nColumns; col++) {
            strcat(sOffSet, " ");
            strcat(sOffSet, hex(CpuRead(nAddr), 2, hex_aux));
            nAddr += 1;
        }
        DrawText(sOffSet, nRamX, nRamY, 4, BLACK);
        nRamY += 10;
    }
}

void DrawCpu(int x, int y) {
    char hex_aux[16];
    DrawText("STATUS:", x , y , 4, WHITE);
    DrawText("N", x  + 64, y,  4, cpu->status & N ? GREEN : RED);
    DrawText("V",x  + 80, y ,  4, cpu->status & V ? GREEN : RED);
    DrawText("-",x  + 96, y ,  4, cpu->status & U ? GREEN : RED);
    DrawText("B",x  + 112, y ,  4, cpu->status & B ? GREEN : RED);
    DrawText("D",x  + 128, y ,  4, cpu->status & D ? GREEN : RED);
    DrawText("I",x  + 144, y ,  4, cpu->status & I ? GREEN : RED);
    DrawText("Z",x  + 160, y ,  4, cpu->status & Z ? GREEN : RED);
    DrawText("C",x  + 178, y ,  4, cpu->status & C ? GREEN : RED);
    char pc[1024];
    char a[1024];
    char xr[1024];
    char yr[1024];
    char sp[1024];

    strcpy(pc, "");
    strcpy(a, "");
    strcpy(xr, "");
    strcpy(yr, "");
    strcpy(sp, "");

    strcat(pc, "PC: $");
    strcat(pc, hex(cpu->PC, 4, hex_aux));

    strcat(a, "A: $");
    strcat(a, hex(cpu->A, 2, hex_aux));
    strcat(a, "  [");
    char *a_str [256];
    sprintf(a_str,"%d", cpu->A);
    strcat(a, a_str);
    strcat(a, "]");

    strcat(xr, "X: $");
    strcat(xr, hex(cpu->X, 2, hex_aux));
    strcat(xr, "  [");
    char *x_str [256];
    sprintf(x_str,"%d", cpu->X);
    strcat(xr, x_str);
    strcat(xr, "]");

    strcat(yr, "Y: $");
    strcat(yr, hex(cpu->Y, 2, hex_aux));
    strcat(yr, "  [");
    char *y_str [256];
    sprintf(y_str,"%d", cpu->Y);
    strcat(yr, y_str);
    strcat(yr, "]");

    strcat(sp, "SP: $");
    strcat(sp, hex(cpu->SP, 4, hex_aux));


    DrawText(pc, x , y + 10, 4, WHITE);
    DrawText(a, x , y + 20, 4, WHITE);
    DrawText(xr, x , y + 30, 4, WHITE);
    DrawText(yr, x , y + 40, 4, WHITE);
    DrawText(sp, x , y + 50, 4, WHITE);
}

void DrawCode(int x, int y, int nLines) {
    int nLineY = (nLines >> 1) * 10 + y;
    u16 pc = cpu->PC;
    char *pcLine = mapAsm[pc++];
    DrawText(pcLine, x, nLineY, 4, GREEN);
    while (nLineY < (nLines * 10) + y) {
        pcLine = mapAsm[pc++];
        if (pcLine != NULL) {
            nLineY += 10;
            DrawText(pcLine, x, nLineY, 4, WHITE); 
        }  
    }
    pc = cpu->PC;
    nLineY = (nLines >> 1) * 10 + y;
    while (nLineY > y) {
        pcLine = mapAsm[--pc];
        if (pcLine != NULL) {
            nLineY -= 10;
            DrawText(pcLine, x, nLineY, 4, WHITE); 
        }
    }
}

void DrawSprite(Sprite *sprite, u16 x, u16 y, i32 scale) {
    if (sprite == NULL) return;

    i32 fxs = 0, fxm = 1, fx = 0;
    i32 fys = 0, fym = 1, fy = 0;

    if (scale > 1) {
        fx = fxs;
        for (i32 i = 0; i < sprite->width; i++, fx += fxm) {
            fy = fys;
            for (i32 j = 0; j < sprite->height; j++, fy += fym)
                for (i32 is = 0; is < scale; is++)
                    for (i32 js = 0; js < scale; js++)
                        DrawPixel(x + (i * scale) + is, y + (j * scale) + js, SpriteGetPixel(sprite, fx, fy));
        }
    }
    else {
        fx = fxs;
        for (i32 i = 0; i < sprite->width; i++, fx += fxm) {
            fy = fys;
            for (i32 j = 0; j < sprite->height; j++, fy += fym)
                DrawPixel(x + i, y + j, SpriteGetPixel(sprite, fx, fy));
        }
    }
}

void SetupDemo() {
    cpu = CpuGet();
    ppu = PpuGet();
    Cartridge *cartridge = CartridgeCreate("./nestest.nes");
    NesInsertCartridge(cpu->bus, cartridge);
    // Extract dissassembly
    CpuDisassemble(0x0000, 0xFFFF, mapAsm);
    // Reset
    NesReset(cpu->bus);
}

void UpdateDemo() {
    cpu->bus->controller[0] = 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_X) ? 0x80 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_Z) ? 0x40 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_A) ? 0x20 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_S) ? 0x10 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_UP) ? 0x08 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_DOWN) ? 0x04 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_LEFT) ? 0x02 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_RIGHT) ? 0x00 : 0x00;
    float elapsedTime = GetFrameTime();
    if (emulationRun) {
        if (residualTime > 0.0f)
            residualTime -= elapsedTime;
        else {
            residualTime += (1.0f / 60.0f) - elapsedTime;
            do { NesClock(cpu->bus); } while (!ppu->frameCompleted);
            ppu->frameCompleted = false;
        }
    }
    else {
        // Emulate code step-by-step
        if (IsKeyDown(KEY_C)) {
            // Clock enough times to execute a whole CPU instruction
            do { NesClock(cpu->bus); } while (!CpuComplete());
            // CPU clock runs slower than system clock, so it may be
            // complete for additional system clock cycles. Drain
            // those out
            do { NesClock(cpu->bus); } while (!CpuComplete());
        }

        // Emulate one whole frame
        if (IsKeyPressed(KEY_F)) {
            // Clock enough times to draw a single frame
            do { NesClock(cpu->bus); } while (!ppu->frameCompleted);
            // Use residual clock cycles to complete current instruction
            do { NesClock(cpu->bus); } while (!CpuComplete());
            // Reset frame completion flag
            ppu->frameCompleted = false;
        }
    }

    if (IsKeyPressed(KEY_SPACE)) emulationRun = !emulationRun;
    if (IsKeyPressed(KEY_R)) NesReset(cpu->bus);

    if (IsKeyPressed(KEY_P)) selectedPalette = (selectedPalette + 1) % 8;	
}

void StartDemo() {
    printf("Hello, Demo!\n");
    InitWindow(780, 480, "NES Instructions Demo");
    
    SetupDemo();
    const int nSwatchSize = 6;
    while (!WindowShouldClose()) {
        UpdateDemo();
        BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawCpu(516, 2);
            DrawCode(516, 72, 26);

            DrawSprite(ppu->spriteScreen, 0, 0, 2);

            for (int p = 0; p < 8; p++) // For each palette
                for(int s = 0; s < 4; s++) // For each index
                    DrawRectangle(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340, 
                        nSwatchSize, nSwatchSize, GetColourFromPaletteRam(p, s));

            // Draw selection reticule around selected palette
            DrawRectangleLines(516 + selectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4) + 2, nSwatchSize + 2, WHITE);
            
            DrawSprite(GetPatternTable(0, selectedPalette), 516, 348, 1);
            DrawSprite(GetPatternTable(1, selectedPalette), 648, 348, 1);


        EndDrawing();
    }
}