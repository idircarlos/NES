#ifndef CPU_H
#define CPU_H

#include "bus.h"
#include "types.h"

typedef struct {
    Bus *bus;
    u8 A;
    u8 X;
    u8 Y;
    u16 PC;
    u8 SP;
    u8 status;
    u8 fetched;
    u16 addr_abs;
    u16 addr_rel;
    u8 opcode;
    u8 cycles;
} Cpu6502;

typedef enum {
    C = (1 << 0),   // Carry
    Z = (1 << 1),   // Zero
    I = (1 << 2),   // Interrupt Disable
    D = (1 << 3),   // Decimal (will not be used)
    B = (1 << 4),   // Break
    U = (1 << 5),   // Unused
    V = (1 << 6),   // Overflow
    N = (1 << 7),   // Negative
} CpuStatusFlag;

void CpuInit();

void CpuConnectBus(Bus *bus);
u8 CpuRead(u16 addr);
void CpuWrite(u16 addr, u8 data);
void CpuClock();
u8 CpuFetch();
void CpuReset();
void CpuIrq();  // Interrupt request signal
void CpuNmi();  // Non maskable interrupt signal

//----------------------------------------------------------------------------------
// Addressing modes
//----------------------------------------------------------------------------------

// Indexed addressing
u8 ZPX();
u8 ZPY();
u8 ABX();
u8 ABY();
u8 IZX();
u8 IZY();

// Other addressing modes
u8 IMP();
u8 ACC();
u8 IMM();
u8 ZP0();
u8 ABS();
u8 REL();
u8 IND();

//----------------------------------------------------------------------------------
// Op codes
//----------------------------------------------------------------------------------

u8 ADC();
u8 AND();
u8 ASL();
u8 BCC();
u8 BCS();
u8 BEQ();
u8 BIT();
u8 BMI();
u8 BNE();
u8 BPL();
u8 BRK();
u8 BVC();
u8 BVS();
u8 CLC();
u8 CLD();
u8 CLI();
u8 CLV();
u8 CMP();
u8 CPX();
u8 CPY();

u8 DEC();
u8 DEX();
u8 DEY();
u8 EOR();
u8 INC();
u8 INX();
u8 INY();
u8 JMP();
u8 JSR();
u8 LDA();
u8 LDX();
u8 LDY();
u8 LSR();
u8 NOP();
u8 ORA();
u8 PHA();
u8 PHP();
u8 PLA();
u8 PLP();

u8 ROL();
u8 ROR();
u8 RTI();
u8 RTS();
u8 SBC();
u8 SEC();
u8 SED();
u8 SEI();
u8 STA();
u8 STX();
u8 STY();
u8 TAX();
u8 TAY();
u8 TSX();
u8 TXA();
u8 TXS();
u8 TYA();

u8 XXX();

#endif  // CPU_H
