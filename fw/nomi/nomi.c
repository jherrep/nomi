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

#include "spi_master.h"
#include "nomi.h"
#include "config.h"

bool g_spi_init = false;

uint8_t g_matrix[ROWS][COLS] = {};

typedef struct hw_key {
    uint8_t x;
    uint8_t y;
} hw_key;

/*
 * We need a translation layer between the position on the keyboard's matrix
 * and the data we get from the IO expanders. This sort of made the PCB design
 * a bit easier, although it can be reworked in such a way this shim is not
 * needed.
 */
struct hw_key g_hw_map[NUM_KEYS] = {
    // ----- iox0 ----- //
    // Col 1
    HW_K(0, 0),     // iox1(b0)
    HW_K(1, 0),     // iox1(b1)
    HW_K(2, 0),     // iox1(b2)
    HW_K(3, 0),     // iox1(b3)
    HW_K(4, 0),     // iox1(b4)

    // Col 2
    HW_K(0, 1),     // iox1(b5)
    HW_K(1, 1),     // iox1(b6)
    HW_K(2, 1),     // iox1(b7)
    HW_K(3, 1),     // iox1(a0)
    HW_K(4, 1),     // iox1(a1)

    // Col 3
    HW_K(0, 2),     // iox1(a2)
    HW_K(1, 2),     // iox1(a3)
    HW_K(2, 2),     // iox1(a4)
    HW_K(3, 2),     // iox1(a5)
    HW_K(4, 2),     // iox1(a6)
    // mk1
    HW_K(0, 12),    // iox1(a7)
    // ----- iox1 ----- //
    // Col 4
    HW_K(0, 3),     // iox2(b0)
    HW_K(1, 3),     // iox2(b1)
    HW_K(2, 3),     // iox2(b2)
    HW_K(3, 3),     // iox2(b3)
    HW_K(4, 3),     // iox2(b4)
    // Col 5
    HW_K(0, 4),     // iox2(b5)
    HW_K(1, 4),     // iox2(b6)
    HW_K(2, 4),     // iox2(b7)
    HW_K(3, 4),     // iox2(a0)
    HW_K(4, 4),     // iox2(a1)
    // Col 6
    HW_K(0, 5),     // iox2(a2)
    HW_K(1, 5),     // iox2(a3)
    HW_K(2, 5),     // iox2(a4)
    HW_K(3, 5),     // iox2(a5)
    HW_K(4, 5),     // iox2(a6)
    // mk2
    HW_K(1, 12),    // iox2(a7)

    // ----- iox3 ----- //
    // col 7/
    HW_K(0, 6),     // iox3(b0)
    HW_K(1, 6),     // iox2(b1)
    HW_K(2, 6),     // iox2(b2)
    HW_K(3, 6),     // iox2(b3)
    HW_K(4, 6),     // iox2(b4)
    // col 8
    HW_K(0, 7),     // iox2(b5)
    HW_K(1, 7),     // iox2(b6)
    HW_K(2, 7),     // iox2(b7)
    HW_K(3, 7),     // iox2(a0)
    HW_K(4, 7),     // iox2(a1)
    // col 9
    HW_K(0, 8),     // iox2(a2)
    HW_K(1, 8),     // iox2(a3)
    HW_K(2, 8),     // iox2(a4)
    HW_K(3, 8),     // iox2(a5)
    HW_K(4, 8),     // iox2(a6)
    //mk3
    HW_K(2, 12),     // iox2(a7)

    // ----- iox4 ---- //
    HW_K(0, 9),     // iox3(b0)
    HW_K(1, 9),     // iox3(b1)
    HW_K(2, 9),     // iox3(b2)
    HW_K(3, 9),     // iox3(b3)
    HW_K(4, 9),     // iox3(b4)

    HW_K(0, 10),     // iox3(b5)
    HW_K(1, 10),     // iox3(b6)
    HW_K(2, 10),     // iox3(b7)
    HW_K(3, 10),     // iox3(a0)
    HW_K(4, 10),     // iox3(a1)

    HW_K(0, 11),     // iox3(a2)
    HW_K(1, 11),     // iox3(a3)
    HW_K(2, 11),     // iox3(a4)
    HW_K(3, 11),     // iox3(a5)
    HW_K(4, 11),     // iox3(a6)

    // mk4
    HW_K(3, 12),     // iox3(a7)
};

uint8_t mcp23x_read(uint8_t id, uint8_t reg) {
    /* Assumes a transaction has been started */
    uint8_t data = 0x0;
    uint8_t opcode = MCP23SX_OP | (id << 1) | MCP23SX_R;

    spi_start(SPI_CS_PIN, SPI_LSB_DIS, SPI_MODE, SPI_DIV);
    spi_write(opcode);
    spi_write(reg);
    data = spi_read();
    spi_stop();

    return data;
}

void mcp23x_write(uint8_t id, uint8_t reg, uint8_t data){
    /* Assumes a transaction has been started */
    uint8_t msg[3] = {};

    msg[0] = MCP23SX_OP | (id << 1) | MCP23SX_W;
    msg[1] = reg;
    msg[2] = data;

    spi_start(SPI_CS_PIN, SPI_LSB_DIS, SPI_MODE, SPI_DIV);
    spi_transmit((const uint8_t *)msg, 3);
    spi_stop();
}

void iox_bank_init(void) {
    // There might me a scenario where we execute the init callback multiple
    // times, so we need a guard.
    if (!g_spi_init) {

        setPinOutput(SPI_CS_PIN);
        spi_init();
        chThdSleepMicroseconds(10000);      // 10ms
        mcp23x_write(IOX0_ID, IODIRA, 0xff);
        mcp23x_write(IOX0_ID, IODIRB, 0xff);
        mcp23x_write(IOX0_ID, GPPUB, 0xff);
        mcp23x_write(IOX0_ID, IPOLB, 0xff);
        mcp23x_write(IOX0_ID, GPPUA, 0xff);
        mcp23x_write(IOX0_ID, IPOLA, 0xff);
        mcp23x_write(IOX0_ID, IOCON_REG, IOCON_VAL);

        g_spi_init = true;
    }
}

void iox_bank_read(uint8_t iox_data[8]) {
    /* Read all the IO expanders in such a way that it matches g_hw_map */
    iox_data[0] = mcp23x_read(IOX0_ID, GPIO_REGB);
    iox_data[1] = mcp23x_read(IOX0_ID, GPIO_REGA);

    iox_data[2] = mcp23x_read(IOX1_ID, GPIO_REGB);
    iox_data[3] = mcp23x_read(IOX1_ID, GPIO_REGA);

    iox_data[4] = mcp23x_read(IOX2_ID, GPIO_REGB);
    iox_data[5] = mcp23x_read(IOX2_ID, GPIO_REGA);

    iox_data[6] = mcp23x_read(IOX3_ID, GPIO_REGB);
    iox_data[7] = mcp23x_read(IOX3_ID, GPIO_REGA);
}

uint8_t iox_process_bank(uint8_t hw_map_idx, uint8_t iox_data[8], uint8_t bank_idx) {
    /* Translate the data from one expander into the global matrix */
    int i = 0;
    uint8_t bitmask = 0, row = 0, col = 0;

    for (i=0; i<BANK_SIZE; i++) {
        bitmask = 1 << i;
        row = g_hw_map[i+hw_map_idx].x;
        col = g_hw_map[i+hw_map_idx].y;
        if (iox_data[bank_idx] & bitmask) {
            g_matrix[row][col] = 1;
        }
    }

    return i + hw_map_idx;
}

void iox_data2mtx(uint8_t iox_data[8]) {
    /* Translate the serial data into a temporal keyboard matrix */
    int i = 0, idx = 0;

    for (i = 0; i < NUM_BANKS; i++) {
        idx = iox_process_bank(idx, iox_data, i);
    }
}


