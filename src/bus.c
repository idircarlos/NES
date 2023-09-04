#include "bus.h"
#include "ppu.h"

static u32 systemClocks = 0;

void BusInit(Bus *bus) {
    // TODO: See how this can fit into the code
}

u8 BusRead(Bus *bus, u16 addr) {
    u8 data = 0x00;
    if (CartridgeCpuRead(bus->cartridge, addr, &data)) {
        
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        return MemoryRead(bus->memory, addr);
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        return CpuReadFromPpu(addr, false);
    }
    else if (addr >= 0x4016 && addr <= 0x4017) {
        data = (bus->controllerState[addr & 0x0001] & 0x80) > 0;
        bus->controllerState[addr & 0x0001] <<= 1;
    }
    return data;
}

void BusWrite(Bus *bus, u16 addr, u8 data) {
    if (CartridgeCpuWrite(bus->cartridge, addr, data)) {
        
    }
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        MemoryWrite(bus->memory, addr, data);
    }
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        CpuWriteToPpu(addr, data);
    }
    else if (addr >= 0x4016 && addr <= 0x4017) {
        bus->controllerState[addr & 0x0001] = bus->controller[addr & 0x0001];
    }
}

void NesInsertCartridge(Bus *bus, Cartridge *cartridge) {
    bus->cartridge = cartridge;
    PpuConnectCartridge(cartridge);
}

void NesReset(Bus *bus) {
    CpuReset();
    systemClocks = 0;
}

void NesClock(Bus *bus) {
    PpuClock();
    if (systemClocks % 3 == 0) {
        CpuClock();
    }

    if (bus->ppu->nmi) {
        bus->ppu->nmi = false;  // Reset the nmi flag
        CpuNmi();
    }
    systemClocks++;
}