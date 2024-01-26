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

#include "quantum.h"
#include "nomi.h"
#include "spi_master.h"
#include "matrix.h"

// TODO: Doesn't need to be a global
extern uint8_t g_matrix[ROWS][COLS];

void clean_matrix(void) {
    /*
     * Keyboard's matrix data is valid for only one scan
     */
    int i = 0, j = 0;

    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            g_matrix[i][j] = 0;
        }
    }
}

void matrix_init_custom(void) {
    iox_bank_init();
    chThdSleepMicroseconds(10000);      // 10ms
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    uint8_t iox_data[8] = {};
    bool changed = false;

    iox_bank_read(iox_data);
    iox_data2mtx(iox_data);

    for (uint8_t row = 0; row < ROWS; row++) {
        matrix_row_t tmp = 0;
        matrix_row_t old_row = current_matrix[row];
        for (uint8_t col = 0; col < COLS; col++) {
            tmp  |= g_matrix[row][col] << col;
        }
        if (tmp != old_row) {
            current_matrix[row] = tmp;
            changed = true;
        }
    }

    clean_matrix();

    return changed;
}
