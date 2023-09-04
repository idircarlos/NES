#ifndef BUS_H
#define BUS_H

#include "memory.h"
#include "cartridge.h"
#include "ppu.h"
#include "types.h"

typedef struct {
    Ppu2C02 *ppu;
    Memory *memory;
    Cartridge *cartridge;
    u8 controllerState[2];
    u8 controller[2];
} Bus;

void BusInit(Bus *bus);

u8 BusRead(Bus *bus, u16 addr);
void BusWrite(Bus *bus, u16 addr, u8 data);

void NesInsertCartridge(Bus *bus, Cartridge *cartridge);
void NesReset(Bus *bus);
void NesClock(Bus *bus);

#endif  // BUS_H
