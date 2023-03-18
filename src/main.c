#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/bus.h"
#include "../include/cpu.h"

#define PC_START 0x8000

FILE* rom = NULL;
Bus bus;
uint16_t program_end = 0x00;

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
    program_end = ftell(rom) + PC_START;
    fseek(rom, 0L, SEEK_SET);

    char buf[4];
    uint16_t i = 0;
    while (fscanf(rom, "%s", buf) != EOF) {
        uint8_t data = (int)strtol(buf, NULL, 0);
        bus_write(&bus, PC_START + i, data);
        i++;
    }
}

void print_rom(uint16_t start, uint16_t end) {
    for (int i = start; i < end; i += 16) {
        printf("0x%04x |", i);
        for (int j = 0; j < 16; j++) {
            printf(" 0x%02x ", bus_read(&bus, i + j));

            if (j == 7)
                printf("|");
        }
        printf("\n");
    }
}

void close_rom() {
    fclose(rom);
}

int main(int argc, char* argv[]) {
    bus_init(&bus);
    
    if (!argv[1]) {
        printf("Must enter a file to be run...\n");
        exit(EXIT_FAILURE);
    }
    load_rom(argv[1]);

    cpu_init();
    cpu_connect_bus(&bus);

    bus_write(&bus, 0xFFFC, (PC_START & 0x00FF));
    bus_write(&bus, 0xFFFD, (PC_START >> 8));

    cpu_reset();

    while (get_cpu()->pc < program_end) {
        cpu_clock();
    }

    printf("A register = 0x%02x\n", get_cpu()->a);
    printf("X register = 0x%02x\n", get_cpu()->x);
    printf("Y register = 0x%02x\n", get_cpu()->y);
    printf("Staus register = 0x%02x\n", get_cpu()->status);
    printf("PC = 0x%04x\n", get_cpu()->pc);

    cpu_free();
    bus_free(&bus);
    close_rom();

    return 0;
}