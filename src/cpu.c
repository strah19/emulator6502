#include "../include/cpu.h"
#include <stdlib.h>
#include <stdio.h>

void cpu_init(Cpu* cpu) {
    cpu->a = 0x00;
    cpu->x = 0x00;
    cpu->y = 0x00;
    cpu->status = 0x00;
    cpu->bus = NULL;
    cpu->opcode = 0x00;
    cpu->pc = 0x00;
    cpu->clock_count = 0x00;
    cpu->cycles = 0x00;
}

void cpu_reset(Cpu* cpu) {
    uint8_t high = cpu_read(cpu, 0xFFFC);
    uint8_t low = cpu_read(cpu, 0xFFFD);
    cpu->pc = (high << 8) | low;
    cpu->a = 0x00;
    cpu->x = 0x00;
    cpu->y = 0x00;
    cpu->status = 0x00;
    cpu->clock_count = 0;
    cpu->cycles += 8;
}

void cpu_free(Cpu* cpu) {

}

void cpu_connect_bus(Cpu* cpu, Bus* bus) {
    if (!bus) {
        fprintf(stderr, "Cpu cannot connect to a NULL bus.\n");
        exit(EXIT_FAILURE);
    }
    cpu->bus = bus;
}

bool cpu_clock(Cpu* cpu) {
    bool executed = false;

    if (cpu->cycles == 0) {
        cpu->opcode = cpu_read(cpu, cpu->pc++);
        cpu->cycles += instructions[cpu->opcode].cycles;

        uint8_t additional_cycles = instructions[cpu->opcode].address_mode();
        additional_cycles += instructions[cpu->opcode].opcode();

        executed = true;
    }

    cpu->cycles--;
    cpu->clock_count++;
    return executed;
}

uint8_t cpu_read(Cpu* cpu, uint16_t address) {
    return bus_read(cpu->bus, address);
}

void cpu_write(Cpu* cpu, uint16_t address, uint8_t data) {
    bus_write(cpu->bus, address, data);
}

uint8_t ILL() {
    return 0x00;
}

uint8_t MODE_IMP() {
    return 0x00;
}