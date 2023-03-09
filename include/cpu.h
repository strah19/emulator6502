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
extern uint8_t LDA();
extern uint8_t STA(); extern uint8_t STX(); extern uint8_t STY();
extern uint8_t TAX(); extern uint8_t TAY(); extern uint8_t TSX(); extern uint8_t TXA();
extern uint8_t TXS(); extern uint8_t TYA();

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
/* 0 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 1 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 2 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 3 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 4 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 5 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 6 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 7 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 8 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "STA", &MODE_INX, &STA, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "STY", &MODE_ZP,  &STY, 3 }, { "STA", &MODE_ZP,  &STA, 3 }, { "STX", &MODE_ZP,  &STX, 3 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "TXA", &MODE_IMP, &TXA, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "STY", &MODE_ABS, &STY, 4 }, { "STA", &MODE_ABS, &STA, 4 }, { "STX", &MODE_ABS, &STX, 4 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 9 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "STA", &MODE_INY, &STA, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "STY", &MODE_ZPX, &STY, 4 }, { "STA", &MODE_ZPX, &STA, 4 }, { "STX", &MODE_ZPX, &STX, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "TYA", &MODE_IMP, &TYA, 2 }, { "STA", &MODE_ABY, &STA, 5 }, { "TXS", &MODE_IMP, &TXS, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "STA", &MODE_ABX, &STA, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* A */ { "ILL", &MODE_IMP, &ILL, 7 }, { "LDA", &MODE_INX, &LDA, 6 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "LDA", &MODE_ZP,  &LDA, 3 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "TAY", &MODE_IMP, &TAY, 2 }, { "LDA", &MODE_IMM, &LDA, 2 }, { "TAX", &MODE_IMP, &TAX, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "LDA", &MODE_ABS, &LDA, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* B */ { "ILL", &MODE_IMP, &ILL, 7 }, { "LDA", &MODE_INY, &LDA, 5 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "LDA", &MODE_ZPX, &LDA, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "LDA", &MODE_ABY, &LDA, 4 }, { "TSX", &MODE_IMP, &TSX, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "LDA", &MODE_ABX, &LDA, 4 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* C */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* D */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* E */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "NOP", &MODE_IMP, &NOP, 2 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* F */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },

};

#endif // !CPU_H