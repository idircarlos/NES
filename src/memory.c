#include "memory.h"

void MemoryInit(Memory *memory) {
    for (u16 addr = 0x0000; addr < 0xFFFF; ++addr) {
        memory->RAM[addr] = 0x00;
    }
}

u8 MemoryRead(Memory *memory, u16 addr) {
    return memory->RAM[addr & 0x07FF];  // RAM Mirroring. 8Kb addresable range is morrered into the first 2Kb of RAM
}

void MemoryWrite(Memory *memory, u16 addr, u8 data) {
    memory->RAM[addr & 0x07FF] = data;
}