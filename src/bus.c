#include "../include/bus.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void bus_init(Bus* bus) {
    bus->ram = (uint8_t*) malloc(sizeof(uint8_t) * RAM_SIZE);

    if (!bus->ram) {
        fprintf(stderr, "Unable to allocate memory for ram.\n");
        exit(EXIT_FAILURE);
    }
    memset(bus->ram, 0, sizeof(uint8_t) * RAM_SIZE);
}

void bus_free(Bus* bus) {
    free(bus->ram);
}

void bus_write(Bus* bus, uint16_t address, uint8_t data) {
    if (address_in_range(address))
        bus->ram[address] = data;
}

uint8_t bus_read(Bus* bus, uint16_t address) {
    if (address_in_range(address))
        return bus->ram[address];
    return 0x00;
}

bool address_in_range(uint16_t address) {
    return (address >= 0x00 && address <= 0xFFFF);
}