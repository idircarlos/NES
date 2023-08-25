#include "bus.h"

void BusInit(Bus *bus) {
    
}

u8 BusRead(Bus *bus, u16 addr) {
    return MemoryRead(bus->memory, addr);
}

void BusWrite(Bus *bus, u16 addr, u8 data) {
    MemoryWrite(bus->memory, addr, data);
}