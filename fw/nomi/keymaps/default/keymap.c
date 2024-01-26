/* Copyright 2023 Matthijs Muller

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum {_DEFAULT,  _LOWER, _RAISE};

// MO(1)
#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)

enum custom_keycodes {
    MACRO0 = SAFE_RANGE,
    MACRO1,
    MACRO2,
    MACRO3,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case MACRO0:
        if (record->event.pressed) {
            SEND_STRING("macro0");
        } else {
        }
        break;
    case MACRO1:
        if (record->event.pressed) {
            SEND_STRING("macro1");
        } else {
        }
        break;
    case MACRO2:
        if (record->event.pressed) {
            SEND_STRING("macro2");
        } else {
        }
        break;
    case MACRO3:
        if (record->event.pressed) {
            SEND_STRING("macro3");
        } else {
        }
        break;
    }
    return true;
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_DEFAULT] = LAYOUT(
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,  KC_LBRC,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,   KC_RBRC,
    KC_GRV,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,  KC_MINS,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENTER, KC_EQL,
    KC_LCTL, KC_LALT, KC_RGUI, KC_LGUI,  RAISE,   KC_SPC,  KC_SPC,  LOWER,   KC_LEFT, KC_DOWN, KC_UP,   KC_RIGHT, XXXXXXX
    ),

  [_LOWER] = LAYOUT(
    KC_TRNS, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_INS,  KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_DEL,   KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,  KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,   XXXXXXX
    ),

  [_RAISE] = LAYOUT(
    KC_TRNS,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,      KC_F8,     KC_F9,      KC_F10,     KC_INS,     MACRO0,
    S(KC_GRV), S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5), S(KC_6), S(KC_7),    S(KC_8),   S(KC_9),    S(KC_0),    KC_DEL,     MACRO1,
    KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, S(KC_MINS), S(KC_EQL), S(KC_LBRC), S(KC_RBRC), S(KC_BSLS), MACRO2,
    KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS,   KC_TRNS,    KC_TRNS,    KC_TRNS,    MACRO3,
    KC_TRNS,   KC_TRNS, KC_TRNS, KC_CAPS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_HOME,   KC_PGDN,    KC_PGUP,    KC_END,  XXXXXXX
    )
};

#ifdef OLED_ENABLE

bool oled_task_user(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("nomi\n"), false);
    oled_write_P(PSTR("layer: "), false);

    switch (get_highest_layer(layer_state)) {
        case _DEFAULT:
            oled_write_P(PSTR("default\n"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("raise\n"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("lower\n"), false);
            break;
        default:
            oled_write_ln_P(PSTR("undefined"), false);
    }

    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.caps_lock ? PSTR("CAPS LOCK") : PSTR("         "), false);

    return false;
}

#endif
