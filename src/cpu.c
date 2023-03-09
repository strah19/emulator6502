#include "../include/cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    uint8_t high = cpu_read(0xFFFC);
    uint8_t low = cpu_read(0xFFFD);
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

    if ((0xFF00 & cpu->fetched_address) != (high_byte << 8))
        return 0x01;
    return 0x00;
}

// This is the same as absoulte but the address gets added with the Y register.
uint8_t MODE_ABY() {
    uint8_t low_byte = cpu_read(cpu->pc++);
    uint8_t high_byte = cpu_read(cpu->pc++);

    cpu->fetched_address = ((high_byte << 8) | low_byte) + cpu->y;

    if ((0xFF00 & cpu->fetched_address) != (high_byte << 8))
        return 0x01;
    return 0x00; 
}

// This takes the next byte and assumes that address is in the zero page.
uint8_t MODE_ZP() {
    cpu->fetched_address = cpu_read(cpu->pc++);
    cpu->fetched_address &= 0x00FF;
 
    return 0x00;
}

// Same as zero page but adds the X register.
uint8_t MODE_ZPX() {
    cpu->fetched_address = cpu_read(cpu->pc++) + cpu->x;
    cpu->fetched_address &= 0x00FF;

    return 0x00;
}

// Same as zero page but adds the Y register.
uint8_t MODE_ZPY() {
    cpu->fetched_address = cpu_read(cpu->pc++) + cpu->y;
    cpu->fetched_address &= 0x00FF;

    return 0x00;
}

// The next byte is added to the current program counter.
uint8_t MODE_REL() {
    cpu->fetched_address = cpu_read(cpu->pc++);
    cpu->fetched_address &= 0x00FF;

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

uint8_t LDA() {
    uint8_t data = fetch();
    cpu->a = data;
    return 0x00;
}

uint8_t STA() {
    cpu_write(cpu->fetched_address, cpu->a);
    return 0x00;
}

uint8_t STX() {
    cpu_write(cpu->fetched_address, cpu->x);
    return 0x00;
}

uint8_t STY() {
    cpu_write(cpu->fetched_address, cpu->y);
    return 0x00;
}

uint8_t TAX() {
    cpu->x = cpu->a;

    set_flag(Z, cpu->x == 0x00);
    set_flag(N, cpu->x & 0x80);

    return 0x00;
}

uint8_t TAY() {
    cpu->y = cpu->a;

    set_flag(Z, cpu->y == 0x00);
    set_flag(N, cpu->y & 0x80);

    return 0x00;
}

uint8_t TSX() {
    cpu->x = cpu->sp;

    set_flag(Z, cpu->x == 0x00);
    set_flag(N, cpu->x & 0x80);

    return 0x00;
}

uint8_t TXA() {
    cpu->a = cpu->x;

    set_flag(Z, cpu->a == 0x00);
    set_flag(N, cpu->a & 0x80);

    return 0x00;
}

uint8_t TXS() {
    cpu->sp = cpu->x;

    set_flag(Z, cpu->sp == 0x00);
    set_flag(N, cpu->sp & 0x80);

    return 0x00;
}

uint8_t TYA() {
    cpu->a = cpu->y;

    set_flag(Z, cpu->a == 0x00);
    set_flag(N, cpu->a & 0x80);

    return 0x00;
}