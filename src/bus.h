#ifndef BUS_H
#define BUS_H

#include "memory.h"
#include "types.h"

typedef struct {
    Memory *memory;
} Bus;

void BusInit(Bus *bus);

u8 BusRead(Bus *bus, u16 addr);
void BusWrite(Bus *bus, u16 addr, u8 data);


#endif  // BUS_H
