#include <stdio.h>
#include "../include/bus.h"
#include "../include/cpu.h"

int main() {
    Bus bus;
    bus_init(&bus);

    cpu_init();
    cpu_connect_bus(&bus);

    bus_write(&bus, 0xFFFC, 0x80);
    bus_write(&bus, 0xFFFD, 0x00);

    cpu_reset();

    //fill ram
    int p = 0x8000; 
    get_cpu()->a = 0x03;
    bus_write(&bus, p++, 0x2a);
   // bus_write(&bus, p++, 0x04);

    while (p < RAM_SIZE) {
        bus_write(&bus, p, 0xea);
        p++;
    }


    while (get_cpu()->pc < RAM_SIZE) {
        //printf("PC: %#06x\n", get_cpu()->pc);
        cpu_clock();
    }
    printf("A register = %#06x\n", get_cpu()->a);

    cpu_free();
    bus_free(&bus);

    return 0;
}