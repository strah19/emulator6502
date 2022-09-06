#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include <stdbool.h>

#define RAM_SIZE 0xFFFF

typedef struct {
    uint8_t* ram;
} Bus;

extern void bus_init(Bus* bus);

extern void bus_free(Bus* bus);

extern void bus_write(Bus* bus, uint16_t address, uint8_t data);

extern uint8_t bus_read(Bus* bus, uint16_t address);

extern bool address_in_range(uint16_t address);

#endif // !BUS_H