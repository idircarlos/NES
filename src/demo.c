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
    DrawText("STATUS:", x , y , 4, BLACK);
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


    DrawText(pc, x , y + 10, 4, BLACK);
    DrawText(a, x , y + 20, 4, BLACK);
    DrawText(xr, x , y + 30, 4, BLACK);
    DrawText(yr, x , y + 40, 4, BLACK);
    DrawText(sp, x , y + 50, 4, BLACK);
}

void DrawCode(int x, int y, int nLines) {
    int nLineY = (nLines >> 1) * 10 + y;
    u16 pc = cpu->PC;
    char *pcLine = mapAsm[pc++];
    DrawText(pcLine, x, nLineY, 4, BLUE);
    while (nLineY < (nLines * 10) + y) {
        pcLine = mapAsm[pc++];
        if (pcLine != NULL) {
            nLineY += 10;
            DrawText(pcLine, x, nLineY, 4, BLACK); 
        }  
    }
    pc = cpu->PC;
    nLineY = (nLines >> 1) * 10 + y;
    while (nLineY > y) {
        pcLine = mapAsm[--pc];
        if (pcLine != NULL) {
            nLineY -= 10;
            DrawText(pcLine, x, nLineY, 4, BLACK); 
        }
    }
}

void SetupDemo() {
    cpu = CpuGet();
    Cartridge *cartridge = CartridgeCreate("./nestest.nes");
    NesInsertCartridge(cpu->bus, cartridge);
    // Extract dissassembly
    CpuDisassemble(0x0000, 0xFFFF, mapAsm);
    // Reset
    NesReset(cpu->bus);
}

void GetInput() {
    if (IsKeyPressed(KEY_SPACE)) {
        do {
            CpuClock();
        } 
        while (!CpuComplete());
    }

    if (IsKeyPressed(KEY_R))
        CpuReset();

    if (IsKeyPressed(KEY_I))
        CpuIrq();

    if (IsKeyPressed(KEY_N))
        CpuNmi();
}

void StartDemo() {
    printf("Hello, Demo!\n");
    InitWindow(680, 480, "NES Instructions Demo");
    
    SetupDemo();
    while (!WindowShouldClose()) {
        GetInput();
        BeginDrawing();
            ClearBackground(WHITE);
            DrawCpu(450, 2);
            DrawCode(450, 72, 26);
            DrawText("SPACE = Step Instruction    R = RESET    I = IRQ    N = NMI", 10, 370, 4, BLACK);
        EndDrawing();
    }
}