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
    cpu->status = 0x00;
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
        cpu->opcode = cpu_read( cpu->pc++);
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
	if (!(instructions[cpu->opcode].address_mode == &MODE_IMP) || !(instructions[cpu->opcode].address_mode == &MODE_ACC))
		cpu->fetched_data = cpu_read(cpu->fetched_address);
	return cpu->fetched_data;
}

uint8_t ILL() {
    return 0x00;
}

uint8_t MODE_IMP() {
    return 0x00;
}

uint8_t MODE_ACC() {
    cpu->fetched_data = cpu->a;
    return 0x00;
}

uint8_t MODE_IMM() {
    cpu->fetched_data = cpu_read(cpu->pc++);
    return 0x00;
}

uint8_t MODE_ABS() {
    uint8_t low_byte = cpu_read(cpu->pc++);
    uint8_t high_byte = cpu_read(cpu->pc++);

    cpu->fetched_address = (high_byte << 8) | low_byte;
    return 0x00;
}

uint8_t MODE_ABSX() {
    uint8_t low_byte = cpu_read(cpu->pc++);
    uint8_t high_byte = cpu_read(cpu->pc++);

    cpu->fetched_address = ((high_byte << 8) | low_byte) + cpu->x;

    if ((0xFF00 & cpu->fetched_address) != (high_byte << 8))
        return 0x01;
    return 0x00;
}

uint8_t MODE_ABSY() {
    uint8_t low_byte = cpu_read(cpu->pc++);
    uint8_t high_byte = cpu_read(cpu->pc++);

    cpu->fetched_address = ((high_byte << 8) | low_byte) + cpu->y;

    if ((0xFF00 & cpu->fetched_address) != (high_byte << 8))
        return 0x01;
    return 0x00; 
}

uint8_t MODE_ZP() {
    cpu->fetched_address = cpu_read(cpu->pc++);
    cpu->fetched_address &= 0x00FF;
 
    return 0x00;
}

uint8_t MODE_ZPX() {
    cpu->fetched_address = cpu_read(cpu->pc++) + cpu->x;
    cpu->fetched_address &= 0x00FF;

    return 0x00;
}

uint8_t MODE_ZPY() {
    cpu->fetched_address = cpu_read(cpu->pc++) + cpu->y;
    cpu->fetched_address &= 0x00FF;

    return 0x00;
}

uint8_t MODE_REL() {
    cpu->fetched_address = cpu_read(cpu->pc++);
    cpu->fetched_address &= 0x00FF;

    cpu->fetched_address = cpu->pc + cpu->fetched_address;
}

uint8_t MODE_IND() {
    uint8_t low_byte = cpu_read(cpu->pc++);
    uint8_t high_byte = cpu_read(cpu->pc++);

    uint16_t addr = ((high_byte << 8) | low_byte);
    cpu->fetched_address = (cpu_read(addr + 1) << 8) | cpu_read(addr);

    return 0x00;
}

uint8_t MODE_INDINX() {
    return 0x00;

}

uint8_t MODE_INXIND() {
    return 0x00;

}