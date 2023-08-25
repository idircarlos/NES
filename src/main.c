#include <stdio.h>

#include "cpu.h"

static Bus bus = {0};
static Memory memory = {0};

int main(void) {
    bus.memory = &memory;
    CpuInit();
    CpuConnectBus(&bus);
    u8 data = CpuRead(0x0123);
    printf("data = %x\n", data);
    CpuWrite(0x0123, 0x1234);
    data = CpuRead(0x0123);
    printf("data = %x\n", data);
    return 0;
}
