#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/bus.h"
#include "../include/cpu.h"

FILE* rom = NULL;
Bus bus;

void close_rom();

void load_rom(const char* filepath) {
    if (rom)
        close_rom();

    rom = fopen(filepath, "r");

    if (!rom) {
        printf("Unable to open rom file '%s'.\n", filepath);
        return;
    }

    fseek(rom, 0L, SEEK_END);
    int size = ftell(rom);
    fseek(rom, 0L, SEEK_SET);

    char buf[4];
    uint16_t i = 0;
    while (fscanf(rom, "%s", buf) != EOF) {
        uint8_t data = (int)strtol(buf, NULL, 0);
        bus_write(&bus, i, data);
        i++;
    }
}

bool same_16_bytes(int addr) {
    for (int j = 0; j < 16; j++) {
        if (bus_read(&bus, addr + j) == bus_read(&bus, addr + j + 16)) continue;
        return false;
    }  
}

void print_rom() {
    for (int i = 0; i < 0xFFFF; i += 16) {
        for (int j = 0; j < 16; j++) {
            printf(" %d ", bus_read(&bus, i + j));
        }
        printf("\n");
    }
}

void close_rom() {
    fclose(rom);
}

int main() {
    bus_init(&bus);
    
    load_rom("test.txt");
    print_rom();

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