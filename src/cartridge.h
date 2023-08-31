#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <assert.h>
#include "types.h"
#include "mapper.h"

#define DA_INIT_CAP 2048
#define da_append(da, item)                                                          \
    do {                                                                             \
        if ((da)->count >= (da)->capacity) {                                         \
            (da)->capacity = (da)->capacity == 0 ? DA_INIT_CAP : (da)->capacity*2;   \
            (da)->items = realloc((da)->items, (da)->capacity*sizeof(*(da)->items)); \
            assert((da)->items != NULL && "Not enough memory to allocate");          \
        }                                                                            \
        (da)->items[(da)->count++] = (item);                                         \
    } while (0)



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