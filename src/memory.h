#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#define RAM_CAPAPCITY 64*1024   // [0x0000, 0xFFFF]

typedef struct {
    u8 RAM [RAM_CAPAPCITY];
} Memory;

void MemoryInit(Memory *memory);

u8 MemoryRead(Memory *memory, u16 addr);
void MemoryWrite(Memory *memory, u16 addr, u8 data);

#endif  // MEMORY_H