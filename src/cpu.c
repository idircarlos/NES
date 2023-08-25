#include "cpu.h"

static Cpu6502 cpu = {0};

typedef struct {
    char name[4];
    u8 (*addrmode)(void);
    u8 (*operate)(void);
    u8 cycles;
} CpuInstruction;

static CpuInstruction LOOKUP [16*16]= {
    { "BRK", BRK, IMM, 7 },{ "ORA", ORA, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 3 },{ "ORA", ORA, ZP0, 3 },{ "ASL", ASL, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PHP", PHP, IMP, 3 },{ "ORA", ORA, IMM, 2 },{ "ASL", ASL, IMP, 2 },{ "???", XXX, IMP, 2 },{ "???", NOP, IMP, 4 },{ "ORA", ORA, ABS, 4 },{ "ASL", ASL, ABS, 6 },{ "???", XXX, IMP, 6 },
    { "BPL", BPL, REL, 2 },{ "ORA", ORA, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "ORA", ORA, ZPX, 4 },{ "ASL", ASL, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "CLC", CLC, IMP, 2 },{ "ORA", ORA, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "ORA", ORA, ABX, 4 },{ "ASL", ASL, ABX, 7 },{ "???", XXX, IMP, 7 },
    { "JSR", JSR, ABS, 6 },{ "AND", AND, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "BIT", BIT, ZP0, 3 },{ "AND", AND, ZP0, 3 },{ "ROL", ROL, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PLP", PLP, IMP, 4 },{ "AND", AND, IMM, 2 },{ "ROL", ROL, IMP, 2 },{ "???", XXX, IMP, 2 },{ "BIT", BIT, ABS, 4 },{ "AND", AND, ABS, 4 },{ "ROL", ROL, ABS, 6 },{ "???", XXX, IMP, 6 },
    { "BMI", BMI, REL, 2 },{ "AND", AND, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "AND", AND, ZPX, 4 },{ "ROL", ROL, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "SEC", SEC, IMP, 2 },{ "AND", AND, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "AND", AND, ABX, 4 },{ "ROL", ROL, ABX, 7 },{ "???", XXX, IMP, 7 },
    { "RTI", RTI, IMP, 6 },{ "EOR", EOR, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 3 },{ "EOR", EOR, ZP0, 3 },{ "LSR", LSR, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PHA", PHA, IMP, 3 },{ "EOR", EOR, IMM, 2 },{ "LSR", LSR, IMP, 2 },{ "???", XXX, IMP, 2 },{ "JMP", JMP, ABS, 3 },{ "EOR", EOR, ABS, 4 },{ "LSR", LSR, ABS, 6 },{ "???", XXX, IMP, 6 },
    { "BVC", BVC, REL, 2 },{ "EOR", EOR, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "EOR", EOR, ZPX, 4 },{ "LSR", LSR, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "CLI", CLI, IMP, 2 },{ "EOR", EOR, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "EOR", EOR, ABX, 4 },{ "LSR", LSR, ABX, 7 },{ "???", XXX, IMP, 7 },
    { "RTS", RTS, IMP, 6 },{ "ADC", ADC, IZX, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 3 },{ "ADC", ADC, ZP0, 3 },{ "ROR", ROR, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "PLA", PLA, IMP, 4 },{ "ADC", ADC, IMM, 2 },{ "ROR", ROR, IMP, 2 },{ "???", XXX, IMP, 2 },{ "JMP", JMP, IND, 5 },{ "ADC", ADC, ABS, 4 },{ "ROR", ROR, ABS, 6 },{ "???", XXX, IMP, 6 },
    { "BVS", BVS, REL, 2 },{ "ADC", ADC, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "ADC", ADC, ZPX, 4 },{ "ROR", ROR, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "SEI", SEI, IMP, 2 },{ "ADC", ADC, ABY, 4 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "ADC", ADC, ABX, 4 },{ "ROR", ROR, ABX, 7 },{ "???", XXX, IMP, 7 },
    { "???", NOP, IMP, 2 },{ "STA", STA, IZX, 6 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 6 },{ "STY", STY, ZP0, 3 },{ "STA", STA, ZP0, 3 },{ "STX", STX, ZP0, 3 },{ "???", XXX, IMP, 3 },{ "DEY", DEY, IMP, 2 },{ "???", NOP, IMP, 2 },{ "TXA", TXA, IMP, 2 },{ "???", XXX, IMP, 2 },{ "STY", STY, ABS, 4 },{ "STA", STA, ABS, 4 },{ "STX", STX, ABS, 4 },{ "???", XXX, IMP, 4 },
    { "BCC", BCC, REL, 2 },{ "STA", STA, IZY, 6 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 6 },{ "STY", STY, ZPX, 4 },{ "STA", STA, ZPX, 4 },{ "STX", STX, ZPY, 4 },{ "???", XXX, IMP, 4 },{ "TYA", TYA, IMP, 2 },{ "STA", STA, ABY, 5 },{ "TXS", TXS, IMP, 2 },{ "???", XXX, IMP, 5 },{ "???", NOP, IMP, 5 },{ "STA", STA, ABX, 5 },{ "???", XXX, IMP, 5 },{ "???", XXX, IMP, 5 },
    { "LDY", LDY, IMM, 2 },{ "LDA", LDA, IZX, 6 },{ "LDX", LDX, IMM, 2 },{ "???", XXX, IMP, 6 },{ "LDY", LDY, ZP0, 3 },{ "LDA", LDA, ZP0, 3 },{ "LDX", LDX, ZP0, 3 },{ "???", XXX, IMP, 3 },{ "TAY", TAY, IMP, 2 },{ "LDA", LDA, IMM, 2 },{ "TAX", TAX, IMP, 2 },{ "???", XXX, IMP, 2 },{ "LDY", LDY, ABS, 4 },{ "LDA", LDA, ABS, 4 },{ "LDX", LDX, ABS, 4 },{ "???", XXX, IMP, 4 },
    { "BCS", BCS, REL, 2 },{ "LDA", LDA, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 5 },{ "LDY", LDY, ZPX, 4 },{ "LDA", LDA, ZPX, 4 },{ "LDX", LDX, ZPY, 4 },{ "???", XXX, IMP, 4 },{ "CLV", CLV, IMP, 2 },{ "LDA", LDA, ABY, 4 },{ "TSX", TSX, IMP, 2 },{ "???", XXX, IMP, 4 },{ "LDY", LDY, ABX, 4 },{ "LDA", LDA, ABX, 4 },{ "LDX", LDX, ABY, 4 },{ "???", XXX, IMP, 4 },
    { "CPY", CPY, IMM, 2 },{ "CMP", CMP, IZX, 6 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 8 },{ "CPY", CPY, ZP0, 3 },{ "CMP", CMP, ZP0, 3 },{ "DEC", DEC, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "INY", INY, IMP, 2 },{ "CMP", CMP, IMM, 2 },{ "DEX", DEX, IMP, 2 },{ "???", XXX, IMP, 2 },{ "CPY", CPY, ABS, 4 },{ "CMP", CMP, ABS, 4 },{ "DEC", DEC, ABS, 6 },{ "???", XXX, IMP, 6 },
    { "BNE", BNE, REL, 2 },{ "CMP", CMP, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "CMP", CMP, ZPX, 4 },{ "DEC", DEC, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "CLD", CLD, IMP, 2 },{ "CMP", CMP, ABY, 4 },{ "NOP", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "CMP", CMP, ABX, 4 },{ "DEC", DEC, ABX, 7 },{ "???", XXX, IMP, 7 },
    { "CPX", CPX, IMM, 2 },{ "SBC", SBC, IZX, 6 },{ "???", NOP, IMP, 2 },{ "???", XXX, IMP, 8 },{ "CPX", CPX, ZP0, 3 },{ "SBC", SBC, ZP0, 3 },{ "INC", INC, ZP0, 5 },{ "???", XXX, IMP, 5 },{ "INX", INX, IMP, 2 },{ "SBC", SBC, IMM, 2 },{ "NOP", NOP, IMP, 2 },{ "???", SBC, IMP, 2 },{ "CPX", CPX, ABS, 4 },{ "SBC", SBC, ABS, 4 },{ "INC", INC, ABS, 6 },{ "???", XXX, IMP, 6 },
    { "BEQ", BEQ, REL, 2 },{ "SBC", SBC, IZY, 5 },{ "???", XXX, IMP, 2 },{ "???", XXX, IMP, 8 },{ "???", NOP, IMP, 4 },{ "SBC", SBC, ZPX, 4 },{ "INC", INC, ZPX, 6 },{ "???", XXX, IMP, 6 },{ "SED", SED, IMP, 2 },{ "SBC", SBC, ABY, 4 },{ "NOP", NOP, IMP, 2 },{ "???", XXX, IMP, 7 },{ "???", NOP, IMP, 4 },{ "SBC", SBC, ABX, 4 },{ "INC", INC, ABX, 7 },{ "???", XXX, IMP, 7 },
};

void CpuInit() {
    cpu.bus = NULL;
    cpu.A = 0x00;
    cpu.X = 0x00;
    cpu.Y = 0x00;
    cpu.PC = 0x0000;
    cpu.SP = 0x00;
    cpu.status = 0b00000000;   // CPU Status set all flags to 0

    cpu.fetched = 0x00;
    cpu.addr_abs = 0x0000;
    cpu.addr_abs = 0x0000;
    cpu.opcode = 0x00;
    cpu.cycles = 0;
}

void CpuConnectBus(Bus *bus) {
    cpu.bus = bus;
}

u8 CpuRead(u16 addr) {
    return BusRead(cpu.bus, addr);
}

void CpuWrite(u16 addr, u8 data) {
    BusWrite(cpu.bus, addr, data);
}

CpuStatusFlag CpuGetFlag(CpuStatusFlag flag) {
    return cpu.status & flag;
}

void CpuSetFlag(CpuStatusFlag flag, bool one) {
    if (one) {
        cpu.status |= flag;
    }
    else {
        cpu.status &= ~flag;
    }
}

void CpuClock() {
    if (cpu.cycles == 0) {
        cpu.opcode = CpuRead(cpu.PC);
        cpu.PC++;

        CpuInstruction instruction = LOOKUP[cpu.opcode];
        cpu.cycles = instruction.cycles;
        u8 addrmode_additional_cycle = instruction.addrmode();
        u8 operate_additional_cycle = instruction.operate();
        cpu.cycles += addrmode_additional_cycle & operate_additional_cycle; 
        
    }
    cpu.cycles--;
}

u8 CpuFetch() {
    if (LOOKUP[cpu.opcode].addrmode != IMP) {
        cpu.fetched = CpuRead(cpu.addr_abs);
    }
    return cpu.fetched;
}

//----------------------------------------------------------------------------------
// Addressing modes
//----------------------------------------------------------------------------------

// Indexed addressing
u8 ZPX() {
    cpu.addr_abs = CpuRead(cpu.PC++) + cpu.X;
	cpu.addr_abs &= 0x00FF;
    return 0;
}

u8 ZPY() {
    cpu.addr_abs = read(cpu.PC++) + cpu.Y;
	cpu.addr_abs &= 0x00FF;
    return 0;
}

u8 ABX() {
    u16 lo = CpuRead(cpu.PC++);
    u16 hi = CpuRead(cpu.PC++);

    cpu.addr_abs = ((hi << 8) | lo) + cpu.X;

    // If the addr is in a new page, then we may need another clock cycle
    if ((cpu.addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }

    return 0;
}

u8 ABY() {
    u16 lo = CpuRead(cpu.PC++);
    u16 hi = CpuRead(cpu.PC++);

    cpu.addr_abs = ((hi << 8) | lo) + cpu.Y;

    // If the addr is in a new page, then we may need another clock cycle
    if ((cpu.addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }

    return 0;
}

u8 IZX() {
    u16 t = CpuRead(cpu.PC++);
    u16 lo = CpuRead((u16)(t + (u16)cpu.X) & 0x00FF);
    u16 hi = CpuRead((u16)(t + (u16)cpu.X + 1) & 0x00FF);
    cpu.addr_abs = (lo << 8) | hi;
    return 0;
}

u8 IZY() {
    u16 t = CpuRead(cpu.PC++);
    u16 lo = CpuRead(t & 0x00FF);
    u16 hi = CpuRead((t + 1) & 0x00FF);
    cpu.addr_abs = ((lo << 8) | hi) + cpu.Y;
    // If the addr is in a new page, then we may need another clock cycle
    if ((cpu.addr_abs & 0xFF00) != (hi << 8)) {
        return 1;
    }
    return 0;
}

// Other addressing modes
u8 IMP() {
    cpu.fetched = cpu.A;
    return 0;
}

// Might not needed
u8 ACC() {
    return 0;
}

u8 IMM() {
    cpu.addr_abs = cpu.PC++;
    return 0;
}

u8 ZP0() {
    cpu.addr_abs = CpuRead(cpu.PC++);
	cpu.addr_abs &= 0x00FF;
	return 0;
}

u8 ABS() {
    u16 lo = CpuRead(cpu.PC++);
    u16 hi = CpuRead(cpu.PC++);

    cpu.addr_abs = (hi << 8) | lo;
    return 0;
}

u8 REL() {
    cpu.addr_rel = CpuRead(cpu.PC++);
    if (cpu.addr_rel & 0x80) {      // 0x80 = 128. Checking 7th bit.
        cpu.addr_rel |= 0xFF00;
    }
    return 0;
}

u8 IND() {
    u16 ptr_lo = CpuRead(cpu.PC++);
    u16 ptr_hi = CpuRead(cpu.PC++);

    u16 ptr_addr = (ptr_hi << 8) | ptr_lo;
    
    // NES bug, we need to replicate this:
    // An indirect JMP (xxFF) will fail because the MSB will be fetched from
    // address xx00 instead of page xx+1. See at: https://www.nesdev.org/6502bugs.txt
    if (ptr_lo == 0x00FF) {
        cpu.addr_abs = (CpuRead(ptr_addr & 0xFF00) << 8) | CpuRead(ptr_addr + 0);
    }
    // Normal behaivour
    else {
        cpu.addr_abs = (CpuRead(ptr_addr + 1) << 8) | CpuRead(ptr_addr + 0);
    }
    return 0;
}


//----------------------------------------------------------------------------------
// Op codes
//----------------------------------------------------------------------------------

u8 ADC();

u8 AND() {
    u8 operand = CpuFetch();    // cpu.fetched is populated also, but for clarity we might use the variable
    cpu.A &= operand;
    CpuSetFlag(Z, operand == 0x00);
    CpuSetFlag(N, operand & 0x80);
    return 1;
}

u8 ASL();

u8 BCC() {
    if (CpuGetFlag(C) == 0) {
        cpu.cycles++;
        cpu.addr_abs = cpu.PC + cpu.addr_rel;
        if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00)) {
            cpu.cycles++;
        }
        cpu.PC = cpu.addr_abs;
    }
    return 0;
}

u8 BCS() {
    if (CpuGetFlag(C) == 1) {
        cpu.cycles++;
        cpu.addr_abs = cpu.PC + cpu.addr_rel;
        if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00)) {
            cpu.cycles++;
        }
        cpu.PC = cpu.addr_abs;
    }
    return 0;
}

u8 BEQ() {
    if (CpuGetFlag(Z) == 1) {
        cpu.cycles++;
        cpu.addr_abs = cpu.PC + cpu.addr_rel;
        if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00)) {
            cpu.cycles++;
        }
        cpu.PC = cpu.addr_abs;
    }
    return 0;
}
u8 BIT();

u8 BMI() {
    if (CpuGetFlag(N) == 1) {
        cpu.cycles++;
        cpu.addr_abs = cpu.PC + cpu.addr_rel;
        if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00)) {
            cpu.cycles++;
        }
        cpu.PC = cpu.addr_abs;
    }
    return 0;
}

u8 BNE() {
    if (CpuGetFlag(Z) == 0) {
        cpu.cycles++;
        cpu.addr_abs = cpu.PC + cpu.addr_rel;
        if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00)) {
            cpu.cycles++;
        }
        cpu.PC = cpu.addr_abs;
    }
    return 0;
}

u8 BPL() {
    if (CpuGetFlag(N) == 0) {
        cpu.cycles++;
        cpu.addr_abs = cpu.PC + cpu.addr_rel;
        if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00)) {
            cpu.cycles++;
        }
        cpu.PC = cpu.addr_abs;
    }
    return 0;
}

u8 BRK();

u8 BVC() {
    if (CpuGetFlag(V) == 0) {
        cpu.cycles++;
        cpu.addr_abs = cpu.PC + cpu.addr_rel;
        if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00)) {
            cpu.cycles++;
        }
        cpu.PC = cpu.addr_abs;
    }
    return 0;
}

u8 BVS() {
    if (CpuGetFlag(V) == 1) {
        cpu.cycles++;
        cpu.addr_abs = cpu.PC + cpu.addr_rel;
        if ((cpu.addr_abs & 0xFF00) != (cpu.PC & 0xFF00)) {
            cpu.cycles++;
        }
        cpu.PC = cpu.addr_abs;
    }
    return 0;
}

u8 CLC() {
    CpuSetFlag(C, false);
    return 0;
}

u8 CLD() {
    CpuSetFlag(D, false);
    return 0;
}

u8 CLI() {
    CpuSetFlag(I, false);
    return 0;
}

u8 CLV() {
    CpuSetFlag(V, false);
    return 0;
}

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