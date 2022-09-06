#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "../include/bus.h"

typedef struct {
    uint8_t a, x, y;
    uint8_t status;
    uint16_t pc;
    uint8_t opcode;
    uint32_t clock_count;
    uint8_t cycles;
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

extern void cpu_init(Cpu* cpu);

extern void cpu_free(Cpu* cpu);

extern void cpu_connect_bus(Cpu* cpu, Bus* bus);

extern void cpu_reset(Cpu* cpu);

extern uint8_t cpu_read(Cpu* cpu, uint16_t address);

extern void cpu_write(Cpu* cpu, uint16_t address, uint8_t data);

extern bool cpu_clock(Cpu* cpu);

extern uint8_t ILL();

extern uint8_t MODE_IMP();

static Instruction instructions[] = {
/* 0 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 1 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 2 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 3 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 4 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 5 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 6 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 7 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 8 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* 9 */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* A */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* B */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* C */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* D */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* E */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },
/* F */ { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 }, { "ILL", &MODE_IMP, &ILL, 7 },

};

#endif // !CPU_H