#include <stdio.h>
#include "../include/bus.h"
#include "../include/cpu.h"

int main() {
    Bus bus;
    bus_init(&bus);

    cpu_init();
    cpu_connect_bus(&bus);

    bus_write(&bus, 0xFFFC, 0x00);
    bus_write(&bus, 0xFFFD, 0x80);

    cpu_reset();

    //fill ram
    int p = 0x8000; 
    get_cpu()->a = 0x01;
    bus_write(&bus, p++, 0x69);
    bus_write(&bus, p++, 0x02);

    while (p < RAM_SIZE) {
        bus_write(&bus, p, 0xea);
        p++;
    }


        printf("PC: %#06x\n", get_cpu()->pc);
    while (get_cpu()->pc < RAM_SIZE) {
       // printf("PC: %#06x\n", get_cpu()->pc);
        cpu_clock();
    }
    printf("A register = %#06x\n", get_cpu()->a);

    cpu_free();
    bus_free(&bus);

    return 0;
}