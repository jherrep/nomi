/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define MCP23SX_OP   0x40       // LSB act as the HW address
#define MCP23SX_W    0x0
#define MCP23SX_R    0x1

/**** mcp23s17 registers, iocon.bank = 0 (default) ****/
#define IODIRA        0x00       // The Input/Output Register
#define IODIRB        0x01       // The Input/Output Register
#define IPOLA         0x02
#define IPOLB         0x03
#define GPPUA         0x0C
#define GPPUB         0x0D
#define GPIO_REGA     0x12       // The GPIO Register
#define GPIO_REGB     0x13       // The GPIO Register
#define IOCON_REG     0x0A
#define IOCON_VAL     0x38      // DISSLW | SEQOP | HAEN

#define SPI_DIV       128
#define SPI_MODE      0
#define SPI_LSB_DIS   false

/**** Nomi ****/
#define IOX0_ID     0
#define IOX1_ID     1
#define IOX2_ID     2
#define IOX3_ID     3

#define NUM_BANKS   8     // two IO banks per IOx chip
#define BANK_SIZE   8     // Num of IOs per bank

#define NUM_KEYS    65
#define ROWS        5
#define COLS        13

#define HW_K(X, Y) {.x = X, .y = Y}

uint8_t mcp23x_read(uint8_t id, uint8_t reg);
void mcp23x_write(uint8_t id, uint8_t reg, uint8_t data);

void iox_bank_init(void);
void iox_bank_read(uint8_t iox_data[8]);
uint8_t iox_process_bank(uint8_t hw_map_idx, uint8_t iox_data[8], uint8_t bank_idx);
void iox_data2mtx(uint8_t iox_data[8]);
