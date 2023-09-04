#include "cartridge.h"
#include "mapper.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char asciiNes[4];
    u8 PRG_chunks;
    u8 CHR_chunks;
    u8 mapper1;
    u8 mapper2;
    u8 PRG_ramSize;
    u8 tvSystem1;
    u8 tvSystem2;
    char unusedPadding[5];
} iNesHeader;

Vector *VectorCreate(usize initialCapacity) {
    Vector *vector = (Vector*)malloc(sizeof(Vector));
    if (!vector) { 
        fprintf(stderr, "Could't allocate space for the vector\n");
        exit(1);
    }
    vector->items = (u8*)malloc(initialCapacity*sizeof(u8));
    if (!vector->items) {
        fprintf(stderr, "Could't allocate space for the items of the vector\n");
        exit(1);
    }
    vector->size = 0;
    vector->capacity = initialCapacity;
    return vector;
}

Cartridge *CartridgeCreate(const char *romPath) {
    FILE *fd = fopen(romPath, "rb");
    if (fd == NULL) {
        fprintf(stderr, "Error opening '%s' file\n", romPath);
        exit(1);
    }
    iNesHeader header = {0};
    usize bytes = fread(&header, sizeof(iNesHeader), 1, fd);
    if (header.mapper1 & 0x04)
        fseek(fd, 512, SEEK_CUR);   // Ignore the 512 bytes of Training information in case that exist

    Cartridge *cartridge = (Cartridge*)malloc(sizeof(Cartridge));
    if (!cartridge) { 
        fprintf(stderr, "Could't allocate space for the cartridge\n");
        exit(1);
    }
    cartridge->PRGMemory = VectorCreate(16384*header.PRG_chunks);
    cartridge->CHRMemory = VectorCreate(8192*header.CHR_chunks);
    cartridge->nPRGBanks = header.PRG_chunks;
    cartridge->nCHRBanks = header.CHR_chunks;
    cartridge->mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
    cartridge->mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

    u8 fileType = 1;
    // By the moment we only consider file type 1
    if (fileType == 1) {
        printf("PRG_chunks = %d\n", header.PRG_chunks);
        printf("CHR_chunks = %d\n", header.CHR_chunks);
        bytes = fread(cartridge->PRGMemory->items, sizeof(u8), 16384*header.PRG_chunks, fd);
        printf("PRGMemory readed = %d\n", bytes);
        bytes = fread(cartridge->CHRMemory->items, sizeof(u8), 8192*header.CHR_chunks, fd);
        
        printf("CHRMemory readed = %d\n", bytes);
    }
    if (fclose(fd)) { 
        fprintf(stdout, "Error closing the file\n");
        exit(1);
    }
    cartridge->mapper = MapperCreate(cartridge->nPRGBanks, cartridge->nCHRBanks);
    switch (cartridge->mapperID) {
        case 0: MapperLoadNROM(cartridge->mapper); break;
        default: assert(cartridge->mapperID && "Mapper ID not implemented"); break;
    }
    return cartridge;
}

bool CartridgeCpuRead(Cartridge *cartridge, u16 addr, u8* data) {
    u32 mappedAddr = 0;
    Mapper *mapper = cartridge->mapper;
    if (mapper->MapperCpuRead(mapper, addr, &mappedAddr)) {
        *data = cartridge->PRGMemory->items[mappedAddr];
        return true;
    }
    return false;
}

bool CartridgeCpuWrite(Cartridge *cartridge, u16 addr, u8 data) {
    u32 mappedAddr = 0;
    Mapper *mapper = cartridge->mapper;
    if (mapper->MapperCpuWrite(mapper, addr, &mappedAddr)) {
        cartridge->PRGMemory->items[mappedAddr] = data;
        return true;
    }
    return false;
}

bool CartridgePpuRead(Cartridge *cartridge, u16 addr, u8* data) {
    u32 mappedAddr = 0;
    Mapper *mapper = cartridge->mapper;
    if (mapper->MapperPpuRead(mapper, addr, &mappedAddr)) {
        *data = cartridge->CHRMemory->items[mappedAddr];
        return true;
    }
    return false;
}

bool CartridgePpuWrite(Cartridge *cartridge, u16 addr, u8 data) {
    u32 mappedAddr = 0;
    Mapper *mapper = cartridge->mapper;
    if (mapper->MapperPpuWrite(mapper, addr, &mappedAddr)) {
        cartridge->CHRMemory->items[mappedAddr] = data;
        return true;
    }
    return false;
}