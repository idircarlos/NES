#include <stdio.h>

#include "cpu.h"
#include "demo.h"
#include "cartridge.h"

static Bus bus = {0};
static Memory memory = {0};

int main(void) {
    bus.memory = &memory;
    bus.ppu = PpuGet();
    CpuInit();
    PpuInit();
    CpuConnectBus(&bus);
    StartDemo();
    return 0;
}
