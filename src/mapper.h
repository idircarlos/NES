#ifndef MAPPER_H
#define MAPPER_H

#include "types.h"
#include <stdlib.h>

typedef struct Mapper{
    u8 PRGbanks;
    u8 CHRbanks;
    bool (*MapperCpuRead)(struct Mapper *this, u16 addr, u32* mappedAddr);
    bool (*MapperCpuWrite)(struct Mapper *this, u16 addr, u32* mappedAddr);

    bool (*MapperPpuRead)(struct Mapper *this, u16 addr, u32* mappedAddr);
    bool (*MapperPpuWrite)(struct Mapper *this, u16 addr, u32* mappedAddr);
} Mapper;

Mapper *MapperCreate(u8 PRGbanks, u8 CHRbanks);

void MapperLoadNROM(Mapper *mapper);

#endif // MAPPER_H