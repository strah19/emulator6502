#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "../include/bus.h"

typedef struct {
    uint8_t a, x, y;
    uint8_t status;
    uint16_t pc;
    uint8_t opcode;
    uint8_t sp;
    uint32_t clock_count;
    uint8_t cycles;

    uint16_t fetched_address;
    uint8_t fetched_data;

    Bus* bus;
} Cpu;

typedef enum {
    C = 0x01,	// Carry Bit
    Z = 0x02,	// Zero
    I = 0x04,	// Disable Interrupts
    D = 0x08,	// Decimal Mode (unused in this implementation)
    B = 0x10,	// Break
    U = 0x20,	// Unused
    V = 0x40,	// Overflow
    N = 0x80,	// Negative
} CpuFlags;

typedef uint8_t(*AddressMode)(void);
typedef uint8_t(*Opcode)(void);

typedef struct {
    const char* name;
    AddressMode address_mode;
    Opcode opcode;
    uint8_t cycles;
} Instruction;

extern void cpu_init();

extern void cpu_free();

extern void cpu_connect_bus(Bus* bus);

extern void cpu_reset();

extern uint8_t cpu_read(uint16_t address);

extern void cpu_write(uint16_t address, uint8_t data);

extern bool cpu_clock();

extern void set_flag(CpuFlags flag, bool set);

extern uint8_t get_flag(CpuFlags flag);

extern Cpu* get_cpu();

extern uint8_t fetch();
 
extern uint8_t ILL();
extern uint8_t NOP();
extern uint8_t LDA(); extern uint8_t LDX(); extern uint8_t LDY();
extern uint8_t STA(); extern uint8_t STX(); extern uint8_t STY();
extern uint8_t TAX(); extern uint8_t TAY(); extern uint8_t TSX(); extern uint8_t TXA();
extern uint8_t TXS(); extern uint8_t TYA();
extern uint8_t ORA(); extern uint8_t AND(); extern uint8_t EOR();
extern uint8_t PHA(); extern uint8_t PHP(); extern uint8_t PLA(); extern uint8_t PLP();
extern uint8_t ROL(); extern uint8_t ROR(); extern uint8_t ASL(); extern uint8_t LSR();
extern uint8_t RTI(); extern uint8_t RTS(); extern uint8_t JSR();
extern uint8_t CLC(); extern uint8_t CLD(); extern uint8_t CLI(); extern uint8_t CLV();
extern uint8_t SEC(); extern uint8_t SED(); extern uint8_t SEI();
extern uint8_t DEC(); extern uint8_t DEX(); extern uint8_t DEY();
extern uint8_t INC(); extern uint8_t INX(); extern uint8_t INY();
extern uint8_t ADC(); extern uint8_t SBC(); 
extern uint8_t CMP(); extern uint8_t CPX(); extern uint8_t CPY();
extern uint8_t BIT();
extern uint8_t JMP();
extern uint8_t BCC(); extern uint8_t BCS(); extern uint8_t BEQ(); extern uint8_t BMI();
extern uint8_t BNE(); extern uint8_t BPL(); extern uint8_t BVS(); extern uint8_t BVC();
extern uint8_t BRK();

extern void irq();
extern void nmi();

extern uint8_t MODE_ACC();
extern uint8_t MODE_IMP();
extern uint8_t MODE_IMM();

extern uint8_t MODE_ABS();
extern uint8_t MODE_ZP();
extern uint8_t MODE_REL();

extern uint8_t MODE_IND();
extern uint8_t MODE_ABX();
extern uint8_t MODE_ABY();

extern uint8_t MODE_ZPX();
extern uint8_t MODE_ZPY();

extern uint8_t MODE_INX();
extern uint8_t MODE_INY();

static Instruction instructions[] = {
    //               0                               1                              2                                 3                             4                             5                             6                               7                             8                               9                               A                              B                              C                               D                             E                             F                                                       
/* 0 */ { "BRK", &MODE_IMP, &BRK, 7 }, { "ORA", &MODE_INX, &ORA, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ORA", &MODE_ZP,  &ORA, 3 }, { "ASL", &MODE_ZP,  &ASL, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "PHP", &MODE_IMP, &PHP, 3 }, { "ORA", &MODE_IMM, &ORA, 2 }, { "ASL", &MODE_ACC, &ASL, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ORA", &MODE_ABS, &ORA, 4 }, { "ASL", &MODE_ABS, &ASL, 6 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 1 */ { "BPL", &MODE_REL, &BPL, 2 }, { "ORA", &MODE_INY, &ORA, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ORA", &MODE_ZPX, &ORA, 4 }, { "ASL", &MODE_ZPX, &ASL, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CLC", &MODE_IMP, &CLC, 2 }, { "ORA", &MODE_ABY, &ORA, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ORA", &MODE_ABX, &ORA, 4 }, { "ASL", &MODE_ABX, &ASL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 2 */ { "JSR", &MODE_ABS, &JSR, 6 }, { "AND", &MODE_INX, &AND, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "BIT", &MODE_ZP,  &BIT, 3 }, { "AND", &MODE_ZP,  &AND, 3 }, { "ROL", &MODE_ZP,  &ROL, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "PLP", &MODE_IMP, &PLP, 4 }, { "AND", &MODE_IMM, &AND, 2 }, { "ROL", &MODE_ACC, &ROL, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "BIT", &MODE_ABS, &BIT, 4 }, { "AND", &MODE_ABS, &AND, 4 }, { "ROL", &MODE_ABS, &ROL, 6 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 3 */ { "BMI", &MODE_REL, &BMI, 2 }, { "AND", &MODE_INY, &AND, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "AND", &MODE_ZPX, &AND, 4 }, { "ROL", &MODE_ZPX, &ROL, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "SEC", &MODE_IMP, &SEC, 2 }, { "AND", &MODE_ABY, &AND, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "AND", &MODE_ABX, &AND, 4 }, { "ROL", &MODE_ABX, &ROL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 4 */ { "RTI", &MODE_IMP, &RTI, 6 }, { "EOR", &MODE_INX, &EOR, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "EOR", &MODE_ZP,  &EOR, 3 }, { "LSR", &MODE_ZP,  &LSR, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "PHA", &MODE_IMP, &PHA, 3 }, { "EOR", &MODE_IMM, &EOR, 2 }, { "LSR", &MODE_ACC, &LSR, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "JMP", &MODE_ABS, &JMP, 3 }, { "EOR", &MODE_ABS, &EOR, 4 }, { "LSR", &MODE_ABS, &LSR, 6 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 5 */ { "BVC", &MODE_REL, &BVC, 2 }, { "EOR", &MODE_INY, &EOR, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "EOR", &MODE_ZPX, &EOR, 4 }, { "LSR", &MODE_ZPX, &LSR, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CLI", &MODE_IMP, &CLI, 2 }, { "EOR", &MODE_ABY, &EOR, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "EOR", &MODE_ABX, &EOR, 4 }, { "LSR", &MODE_ABX, &LSR, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 6 */ { "RTS", &MODE_IMP, &RTS, 6 }, { "ADC", &MODE_INX, &ADC, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ADC", &MODE_ZP,  &ADC, 3 }, { "ROR", &MODE_ZP,  &ROR, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "PLA", &MODE_IMP, &PLA, 4 }, { "ADC", &MODE_IMM, &ADC, 2 }, { "ROR", &MODE_ACC, &ROR, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "JMP", &MODE_IND, &JMP, 5 }, { "ADC", &MODE_ABS, &ADC, 4 }, { "ROR", &MODE_ABS, &ROR, 6 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 7 */ { "BVS", &MODE_REL, &BVS, 2 }, { "ADC", &MODE_INY, &ADC, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ADC", &MODE_ZPX, &ADC, 4 }, { "ROR", &MODE_ZPX, &ROR, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "SEI", &MODE_IMP, &SEI, 2 }, { "ADC", &MODE_ABY, &ADC, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ADC", &MODE_ABX, &ADC, 4 }, { "ROR", &MODE_ABX, &ROR, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 8 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "STA", &MODE_INX, &STA, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "STY", &MODE_ZP,  &STY, 3 }, { "STA", &MODE_ZP,  &STA, 3 }, { "STX", &MODE_ZP,  &STX, 3 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "DEY", &MODE_IMP, &DEY, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "TXA", &MODE_IMP, &TXA, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "STY", &MODE_ABS, &STY, 4 }, { "STA", &MODE_ABS, &STA, 4 }, { "STX", &MODE_ABS, &STX, 4 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 9 */ { "BCC", &MODE_REL, &BCC, 2 }, { "STA", &MODE_INY, &STA, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "STY", &MODE_ZPX, &STY, 4 }, { "STA", &MODE_ZPX, &STA, 4 }, { "STX", &MODE_ZPX, &STX, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "TYA", &MODE_IMP, &TYA, 2 }, { "STA", &MODE_ABY, &STA, 5 }, { "TXS", &MODE_IMP, &TXS, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "STA", &MODE_ABX, &STA, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* A */ { "LDY", &MODE_IMM, &LDY, 2 }, { "LDA", &MODE_INX, &LDA, 6 }, { "LDX", &MODE_IMM, &LDX, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "LDY", &MODE_ZP,  &LDY, 3 }, { "LDA", &MODE_ZP,  &LDA, 3 }, { "LDX", &MODE_ZP,  &LDX, 3 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "TAY", &MODE_IMP, &TAY, 2 }, { "LDA", &MODE_IMM, &LDA, 2 }, { "TAX", &MODE_IMP, &TAX, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "LDY", &MODE_ABS, &LDY, 4 }, { "LDA", &MODE_ABS, &LDA, 4 }, { "LDX", &MODE_ABS, &LDX, 4 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* B */ { "BCS", &MODE_REL, &ILL, 2 }, { "LDA", &MODE_INY, &LDA, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "LDY", &MODE_ZPX, &LDY, 4 }, { "LDA", &MODE_ZPX, &LDA, 4 }, { "LDX", &MODE_ZPY, &LDX, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CLV", &MODE_IMP, &CLV, 2 }, { "LDA", &MODE_ABY, &LDA, 4 }, { "TSX", &MODE_IMP, &TSX, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "LDY", &MODE_ABX, &LDY, 4 }, { "LDA", &MODE_ABX, &LDA, 4 }, { "LDX", &MODE_ABY, &LDX, 4 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* C */ { "CPY", &MODE_IMM, &CPY, 2 }, { "CMP", &MODE_INX, &CMP, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CPY", &MODE_ZP,  &CPY, 3 }, { "CMP", &MODE_ZP,  &CMP, 3 }, { "DEC", &MODE_ZP,  &DEC, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "INY", &MODE_IMP, &INY, 2 }, { "CMP", &MODE_IMM, &CMP, 2 }, { "DEX", &MODE_IMP, &DEX, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CPY", &MODE_ABS, &CPY, 4 }, { "CMP", &MODE_ABS, &CMP, 4 }, { "DEC", &MODE_ABS, &DEC, 6 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* D */ { "BNE", &MODE_REL, &BNE, 2 }, { "CMP", &MODE_INY, &CMP, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CMP", &MODE_ZPX, &CMP, 4 }, { "DEC", &MODE_ZPX, &DEC, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CLD", &MODE_IMP, &CLD, 2 }, { "CMP", &MODE_ABY, &CMP, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CMP", &MODE_ABX, &CMP, 4 }, { "DEC", &MODE_ABX, &DEC, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* E */ { "CPX", &MODE_IMM, &CPX, 2 }, { "SBC", &MODE_INX, &SBC, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CPX", &MODE_ZP,  &CPX, 3 }, { "SBC", &MODE_ZP,  &SBC, 3 }, { "INC", &MODE_ZP,  &INC, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "INX", &MODE_IMP, &INX, 2 }, { "SBC", &MODE_IMM, &SBC, 2 }, { "NOP", &MODE_IMP, &NOP, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "CPX", &MODE_ABS, &CPX, 4 }, { "SBC", &MODE_ABS, &SBC, 4 }, { "INC", &MODE_ABS, &INC, 6 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* F */ { "BEQ", &MODE_REL, &BEQ, 2 }, { "SBC", &MODE_INY, &SBC, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "SBC", &MODE_ZPX, &SBC, 4 }, { "INC", &MODE_ZPX, &INC, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "SED", &MODE_IMP, &SED, 2 }, { "SBC", &MODE_ABY, &SBC, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "SBC", &MODE_ABX, &SBC, 4 }, { "INC", &MODE_ABX, &INC, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },

};

#endif // !CPU_H