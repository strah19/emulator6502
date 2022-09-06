#include <stdio.h>
#include "../include/bus.h"
#include "../include/cpu.h"

int main() {
    Bus bus;
    bus_init(&bus);

    Cpu cpu;
    cpu_init(&cpu);
    cpu_connect_bus(&cpu, &bus);

    bus_write(&bus, 0xFFFC, 0x80);
    bus_write(&bus, 0xFFFD, 0x00);

    cpu_reset(&cpu);
    
    //fill ram
    int p = 0x8000; 
    while (p < RAM_SIZE) {
        bus_write(&bus, p, 0x00);
        p++;
    }

    while (cpu.pc < RAM_SIZE) {
        printf("PC: %#06x\n", cpu.pc);
        cpu_clock(&cpu);
    }

    cpu_free(&cpu);
    bus_free(&bus);

    return 0;
}