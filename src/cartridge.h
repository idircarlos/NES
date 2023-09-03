#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <assert.h>
#include "types.h"
#include "mapper.h"

typedef struct {
    u8 *items;
    usize size;
    usize capacity;
} Vector;

typedef struct {
    Vector *PRGMemory;
    Vector *CHRMemory;
    u8 mapperID;
    u8 nPRGBanks;
    u8 nCHRBanks;
    Mapper *mapper;
} Cartridge;

Cartridge *CartridgeCreate(const char *romPath);

bool CartridgeCpuRead(Cartridge *cartridge, u16 addr, u8* data);
bool CartridgeCpuWrite(Cartridge *cartridge, u16 addr, u8 data);

bool CartridgePpuRead(Cartridge *cartridge, u16 addr, u8* data);
bool CartridgePpuWrite(Cartridge *cartridge, u16 addr, u8 data);

#endif  // CARTRIDGE_H