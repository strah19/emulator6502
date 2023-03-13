#include "../include/cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RESET_VECTOR 0xFFFC
#define STACK_PTR_ADR 0x0100
#define N_FLAG_MASK 0x80
#define LOW_8_BIT_MASK 0x00FF
#define HIGH_8_BIT_MASK 0xFF00

static Cpu* cpu;

void cpu_init() {
    cpu = malloc(sizeof(Cpu));
    memset(cpu, 0, sizeof(Cpu));

    cpu->a = 0x00;
    cpu->x = 0x00;
    cpu->y = 0x00;
    cpu->status = 0x00;
    cpu->bus = NULL;
    cpu->opcode = 0x00;
    cpu->pc = 0x00;
    cpu->clock_count = 0x00;
    cpu->cycles = 0x00;
    cpu->fetched_address = 0x00;
    cpu->fetched_data = 0x00;
}

void cpu_reset() {
    uint8_t low = cpu_read(RESET_VECTOR);
    uint8_t high = cpu_read(RESET_VECTOR + 1);

    cpu->pc = (high << 8) | low;
    cpu->a = 0x00;
    cpu->x = 0x00;
    cpu->y = 0x00;
    cpu->status = 0x00 | U;
    cpu->sp = 0xFD;
    cpu->clock_count = 0;
    cpu->cycles += 8;
    cpu->fetched_address = 0x00;
    cpu->fetched_data = 0x00;
}

void cpu_free() {
    cpu->bus = NULL;
}

Cpu* get_cpu() {
    return cpu;
}

void cpu_connect_bus(Bus* bus) {
    if (!bus) {
        fprintf(stderr, "Cpu cannot connect to a NULL bus.\n");
        exit(EXIT_FAILURE);
    }
    cpu->bus = bus;
}

bool cpu_clock() {
    bool executed = false;

    if (cpu->cycles == 0) {
        cpu->opcode = cpu_read(cpu->pc++);
        cpu->cycles += instructions[cpu->opcode].cycles;

        uint8_t additional_cycles = instructions[cpu->opcode].address_mode();
        additional_cycles += instructions[cpu->opcode].opcode();

        executed = true;
    }

    cpu->cycles--;
    cpu->clock_count++;
    return executed;
}

uint8_t cpu_read(uint16_t address) {
    return bus_read(cpu->bus, address);
}

void cpu_write(uint16_t address, uint8_t data) {
    bus_write(cpu->bus, address, data);
}

void set_flag(CpuFlags flag, bool set) {
    if (set) cpu->status |= flag;
    else     cpu->status &= ~flag;
}

uint8_t get_flag(CpuFlags flag){
    return (((cpu->status & flag) > 0) ? 1 : 0);
}

uint8_t fetch() {
	if (instructions[cpu->opcode].address_mode != &MODE_IMP && instructions[cpu->opcode].address_mode != &MODE_ACC
    && instructions[cpu->opcode].address_mode != &MODE_IMM)
		cpu->fetched_data = cpu_read(cpu->fetched_address);
    
	return cpu->fetched_data;
}

// No extra bytes are needed to perform the instruction.
uint8_t MODE_IMP() {
    return 0x00;
}

// The instruction is performed on the accumulator.
uint8_t MODE_ACC() {
    cpu->fetched_data = cpu->a;
    return 0x00;
}

// The following byte is the data needed for the instruction.
uint8_t MODE_IMM() {
    cpu->fetched_data = cpu_read(cpu->pc++);
    return 0x00;
}

// The next two bytes form an address where the data for the instruction is located.
uint8_t MODE_ABS() {
    uint8_t low_byte = cpu_read(cpu->pc++);
    uint8_t high_byte = cpu_read(cpu->pc++);

    cpu->fetched_address = (high_byte << 8) | low_byte;
    return 0x00;
}

// This is the same as absoulte but the address gets added with the X register.
uint8_t MODE_ABX() {
    uint8_t low_byte = cpu_read(cpu->pc++);
    uint8_t high_byte = cpu_read(cpu->pc++);

    cpu->fetched_address = ((high_byte << 8) | low_byte) + cpu->x;

    if ((HIGH_8_BIT_MASK & cpu->fetched_address) != (high_byte << 8))
        return 0x01;
    return 0x00;
}

// This is the same as absoulte but the address gets added with the Y register.
uint8_t MODE_ABY() {
    uint8_t low_byte = cpu_read(cpu->pc++);
    uint8_t high_byte = cpu_read(cpu->pc++);

    cpu->fetched_address = ((high_byte << 8) | low_byte) + cpu->y;

    if ((HIGH_8_BIT_MASK & cpu->fetched_address) != (high_byte << 8))
        return 0x01;
    return 0x00; 
}

// This takes the next byte and assumes that address is in the zero page.
uint8_t MODE_ZP() {
    cpu->fetched_address = cpu_read(cpu->pc++);
    cpu->fetched_address &= LOW_8_BIT_MASK;
 
    return 0x00;
}

// Same as zero page but adds the X register.
uint8_t MODE_ZPX() {
    cpu->fetched_address = cpu_read(cpu->pc++) + cpu->x;
    cpu->fetched_address &= LOW_8_BIT_MASK;

    return 0x00;
}

// Same as zero page but adds the Y register.
uint8_t MODE_ZPY() {
    cpu->fetched_address = cpu_read(cpu->pc++) + cpu->y;
    cpu->fetched_address &= LOW_8_BIT_MASK;

    return 0x00;
}

// The next byte is added to the current program counter.
uint8_t MODE_REL() {
    cpu->fetched_address = cpu_read(cpu->pc++);
    cpu->fetched_address &= LOW_8_BIT_MASK;

    cpu->fetched_address = cpu->pc + cpu->fetched_address;
}

// The next two bytes are pointers to the address where the data is.
uint8_t MODE_IND() {
    uint8_t low_byte = cpu_read(cpu->pc++);
    uint8_t high_byte = cpu_read(cpu->pc++);

    uint16_t addr = ((high_byte << 8) | low_byte);
    cpu->fetched_address = (cpu_read(addr + 1) << 8) | cpu_read(addr);

    return 0x00;
}

// The next byte is an address which gets added with the X register and the byte at this new address is the address for the data.
uint8_t MODE_INX() {
    uint8_t ptr = cpu_read(cpu->pc++) + cpu->x;
    cpu->fetched_address = (cpu_read(ptr + 1) << 8 | cpu_read(ptr));

    return 0x00;

}

// The next byte is an address. The LSB is first and the next address is the MSB. This address is added with the Y register and creates a new address
// where the data is.
uint8_t MODE_INY() {
    uint8_t ptr = cpu_read(cpu->pc++);
    cpu->fetched_address = (cpu_read(ptr + 1) << 8 | cpu_read(ptr)) + cpu->y;

    return 0x00;
}

// This opcode is for illegal operations.
uint8_t ILL() {
    return 0x00;
}

// This is a no operation opcode.
uint8_t NOP() {
    return 0x00;
}

// Load memory into a register.
uint8_t LDA() {
    cpu->a = fetch();
    return 0x00;
}

// Load memory into x register.
uint8_t LDX() {
    cpu->x = fetch();
    return 0x00;
}

//Load memory into y register.
uint8_t LDY() {
    cpu->y = fetch();
    return 0x00;
}

//Store a register in memory.
uint8_t STA() {
    cpu_write(cpu->fetched_address, cpu->a);
    return 0x00;
}

//Store x register in memory.
uint8_t STX() {
    cpu_write(cpu->fetched_address, cpu->x);
    return 0x00;
}

//Store y register in memory.
uint8_t STY() {
    cpu_write(cpu->fetched_address, cpu->y);
    return 0x00;
}

//Transer a register into x register.
uint8_t TAX() {
    cpu->x = cpu->a;

    set_flag(Z, cpu->x == 0x00);
    set_flag(N, cpu->x & N_FLAG_MASK);

    return 0x00;
}

//Transer a register into y register.
uint8_t TAY() {
    cpu->y = cpu->a;

    set_flag(Z, cpu->y == 0x00);
    set_flag(N, cpu->y & N_FLAG_MASK);

    return 0x00;
}

//Transer stack pointer into x register.
uint8_t TSX() {
    cpu->x = cpu->sp;

    set_flag(Z, cpu->x == 0x00);
    set_flag(N, cpu->x & 0x80);

    return 0x00;
}

//Transer x register into a register.
uint8_t TXA() {
    cpu->a = cpu->x;

    set_flag(Z, cpu->a == 0x00);
    set_flag(N, cpu->a & N_FLAG_MASK);

    return 0x00;
}

//Transer x register into stack pointer.
uint8_t TXS() {
    cpu->sp = cpu->x;

    set_flag(Z, cpu->sp == 0x00);
    set_flag(N, cpu->sp & N_FLAG_MASK);

    return 0x00;
}

//Transer y register into a register.
uint8_t TYA() {
    cpu->a = cpu->y;

    set_flag(Z, cpu->a == 0x00);
    set_flag(N, cpu->a & N_FLAG_MASK);

    return 0x00;
}

//Complete OR operation on a register and memory.
uint8_t ORA() {
    cpu->a = cpu->a | fetch();

    set_flag(Z, cpu->a == 0x00);
    set_flag(N, cpu->a & N_FLAG_MASK);

    return 0x00; 
}

uint8_t PHA() {
    cpu_write(STACK_PTR_ADR + cpu->sp--, cpu->a);
}

uint8_t PHP() {
    cpu_write(STACK_PTR_ADR + cpu->sp--, cpu->status);

    set_flag(B, true);
    set_flag(C, true);

    return 0x00;
}

uint8_t PLA() {
    cpu->sp++;
    cpu->a = cpu_read(STACK_PTR_ADR + cpu->sp);

    set_flag(Z, cpu->a == 0x00);
    set_flag(N, cpu->a & N_FLAG_MASK);

    return 0x00;
}

uint8_t PLP() {
    cpu->sp++;
    uint8_t stat = cpu_read(STACK_PTR_ADR + cpu->sp);
    cpu->status = (stat & 0x11001111);  //The U and B flags are ignored.

    return 0x00;
}

uint8_t ROL() {
    uint16_t data = fetch();
    data = (uint16_t)(data << 1) | get_flag(C);

    set_flag(Z, (data & 0x00FF) == 0x0000);
    set_flag(N, data & N_FLAG_MASK);
    set_flag(C, (data & 0xFF00));

    if (instructions[cpu->opcode].address_mode == &MODE_ACC)
        cpu->a = data;
    else
        cpu_write(cpu->fetched_address, data);

    return 0x00;
}

uint8_t ROR() {
    uint16_t data = fetch();
    data = (uint16_t)(data >> 1) | get_flag(C);

    set_flag(Z, (data & 0x00FF) == 0x0000);
    set_flag(N, data & N_FLAG_MASK);
    set_flag(C, (data & 0xFF00));

    if (instructions[cpu->opcode].address_mode == &MODE_ACC)
        cpu->a = data;
    else
        cpu_write(cpu->fetched_address, data);

    return 0x00;
}

uint8_t RTI() {
    cpu->sp++;
    uint8_t stat = cpu_read(STACK_PTR_ADR + cpu->sp);
    cpu->status = (stat & 0x11001111);  //The U and B flags are ignored.

    cpu->sp++;
    uint8_t low = cpu_read(STACK_PTR_ADR + cpu->sp);
    cpu->sp++;
    uint8_t high = cpu_read(STACK_PTR_ADR + cpu->sp);

    cpu->pc = ((high << 8) | low);
    return 0x00;
}

//Clear the carry flag.
uint8_t CLC() {
    set_flag(C, false);
    return 0x00;
}

//Clear decimal mode flag.
uint8_t CLD() {
    set_flag(D, false);
    return 0x00;
}

//Clear interrupt flag.
uint8_t CLI() {
    set_flag(I, false);
    return 0x00;
}

//Clear overflow flag.
uint8_t CLV() {
    set_flag(V, false);
    return 0x00;
}

//Set carry flag.
uint8_t SEC() {
    set_flag(C, true);
    return 0x00;
}

//Set decimal mode flag.
uint8_t SED() {
    set_flag(D, true);
    return 0x00;
}

//Set interrupt flag.
uint8_t SEI() {
    set_flag(I, true);
    return 0x00;
}

uint8_t DEC() {
    uint8_t data = fetch();
    data--;

    set_flag(Z, (data == 0x00));
    set_flag(N, (N_FLAG_MASK & data));

    cpu_write(cpu->fetched_address, data);
    return 0x00;
}

uint8_t DEX() {
    cpu->x--;

    set_flag(Z, (cpu->x == 0x00));
    set_flag(N, (N_FLAG_MASK & cpu->x));
    return 0x00;
}

uint8_t DEY() {
    cpu->y--;

    set_flag(Z, (cpu->y == 0x00));
    set_flag(N, (N_FLAG_MASK & cpu->y));
    return 0x00;
}

uint8_t INC() {
    uint8_t data = fetch();
    data++;

    set_flag(Z, (data == 0x00));
    set_flag(N, (N_FLAG_MASK & data));

    cpu_write(cpu->fetched_address, data);
    return 0x00;
} 

uint8_t INX() {
    cpu->x++;

    set_flag(Z, (cpu->x == 0x00));
    set_flag(N, (N_FLAG_MASK & cpu->x));
    return 0x00;
} 

uint8_t INY() {
    cpu->y++;

    set_flag(Z, (cpu->y == 0x00));
    set_flag(N, (N_FLAG_MASK & cpu->y));
    return 0x00;
}

uint8_t AND() {
    cpu->a &= fetch();

    set_flag(Z, cpu->a == 0x00);
    set_flag(N, cpu->a & N_FLAG_MASK);
    return 0x00;
} 

uint8_t EOR() {
    cpu->a ^= fetch();
    
    set_flag(Z, cpu->a == 0x00);
    set_flag(N, cpu->a & N_FLAG_MASK);
    return 0x00;
}

uint8_t ASL() {
    uint16_t data = fetch();
    data = (uint16_t)data << 1;

    set_flag(Z, (data & 0x00FF) == 0x0000);
    set_flag(N, (N_FLAG_MASK & data));
    set_flag(C, (data & 0xFF00));

    if (instructions[cpu->opcode].address_mode == &MODE_ACC)
        cpu->a = data;
    else
        cpu_write(cpu->fetched_address, data);

    return 0x00;
}

uint8_t LSR() {
    uint16_t data = fetch();
    data = (uint16_t)data >> 1;

    set_flag(Z, (data & 0x00FF) == 0x0000);
    set_flag(N, false);
    set_flag(C, (data & 0xFF00));

    if (instructions[cpu->opcode].address_mode == &MODE_ACC)
        cpu->a = data;
    else
        cpu_write(cpu->fetched_address, data);

    return 0x00;
}

uint8_t ADC() {
    uint16_t data = cpu->a + fetch() + (uint16_t) get_flag(C);
    set_flag(C, (data & 0x0100));
    set_flag(N, (data & N_FLAG_MASK));
    set_flag(Z, ((data & 0x00FF) == 0x00));

    set_flag(V, (~((cpu->a ^ fetch()) & (cpu->a & data)) & 0x0080));


    cpu->a = (data & 0x00FF);

    return 0x00;
}

uint8_t SBC() {
    uint16_t fetched = fetch();
    fetched = (fetched ^ 0x00FF) + 1; //This gives me the two's complement 

    uint16_t data = cpu->a + fetched + (uint16_t) get_flag(C);
    set_flag(C, (data & 0x0100));
    set_flag(N, (data & N_FLAG_MASK));
    set_flag(Z, ((data & 0x00FF) == 0x00));

    set_flag(V, (~((cpu->a ^ fetch()) & (cpu->a & data)) & 0x0080));


    cpu->a = (data & 0x00FF);

    return 0x00;
}

uint8_t CMP() {
    set_flag(N, (cpu->a < fetch()));
    set_flag(Z, (cpu->a == fetch()));
    set_flag(C, (cpu->a == fetch() || cpu->a > fetch()));

    return 0x00;
}

uint8_t CPX() {
    set_flag(N, (cpu->x < fetch()));
    set_flag(Z, (cpu->x == fetch()));
    set_flag(C, (cpu->x == fetch() || cpu->x > fetch()));

    return 0x00;
}

uint8_t CPY() {
    set_flag(N, (cpu->y < fetch()));
    set_flag(Z, (cpu->y == fetch()));
    set_flag(C, (cpu->y == fetch() || cpu->y > fetch()));

    return 0x00;
}

uint8_t BIT() {
    set_flag(N, fetch() & N_FLAG_MASK);
    set_flag(V, fetch() & 0x40);
    set_flag(Z, (fetch() & cpu->a) == 0x00);

    return 0x00;
}

uint8_t JMP() {
    cpu->pc = cpu->fetched_address;
    return 0x00;
}

uint8_t BCC() {
    if (get_flag(C) == 0)
        cpu->pc = cpu->fetched_address;

    return 0x00;
}

uint8_t BCS() {
    if (get_flag(C) == 1)
        cpu->pc = cpu->fetched_address;

    return 0x00;
} 

uint8_t BEQ() {
    if (get_flag(Z) == 1)
        cpu->pc = cpu->fetched_address;

    return 0x00;
} 

uint8_t BMI() {
    return 0x00;
}

uint8_t BNE() {
    if (get_flag(N) == 1)
        cpu->pc = cpu->fetched_address;

    return 0x00;
} 

uint8_t BPL() {
    if (get_flag(N) == 0)
        cpu->pc = cpu->fetched_address;

    return 0x00;
} 

uint8_t BVS() {
    if (get_flag(V) == 1)
        cpu->pc = cpu->fetched_address;

    return 0x00;
} 

uint8_t BVC() {
    if (get_flag(V) == 0)
        cpu->pc = cpu->fetched_address;

    return 0x00;
}

uint8_t BRK() {
    set_flag(I, true);

    cpu->pc++;
    cpu_write(STACK_PTR_ADR + cpu->sp, (cpu->pc << 8) & 0x00FF);
    cpu->sp--;
    cpu_write(STACK_PTR_ADR + cpu->sp, (cpu->sp & 0x00FF));
    cpu->sp--;

    set_flag(B, true);
    cpu_write(STACK_PTR_ADR + cpu->sp, cpu->status);
    cpu->sp--;
    set_flag(B, false);
    return 0x00;
}

uint8_t RTS() {
    cpu->sp++;
    uint8_t low = cpu_read(STACK_PTR_ADR + cpu->sp);
    cpu->sp++;
    uint8_t high = cpu_read(STACK_PTR_ADR + cpu->sp);
    cpu->pc = (high << 8) | low;

    low = cpu_read(RESET_VECTOR);
    high = cpu_read(RESET_VECTOR + 1);

    cpu->pc = (high << 8) | low;
    return 0x00;
} 

uint8_t JSR() {
    cpu->pc--;
    cpu_write(STACK_PTR_ADR + cpu->sp, (cpu->pc >> 8) & 0x00FF);
    cpu->sp--;
    cpu_write(STACK_PTR_ADR + cpu->sp, (cpu->pc & 0x00FF));
    cpu->sp--;
    cpu->pc = cpu->fetched_address;
    return 0x00;
}
