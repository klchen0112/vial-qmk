/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
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
#include "features/select_word.h"

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NUMBER,
    LAYER_FUNCTION,
    LAYER_CURSOR,
    LAYER_POINTER,
    LAYER_SYMBOL,
    LAYER_SYSTEM,
    LAYER_QWERTY,
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define LG_T LGUI_T(KC_T)
#define LA_H LALT_T(KC_H)
#define LC_E LCTL_T(KC_E)
#define LS_D LSFT_T(KC_D)

#define RG_Y RGUI_T(KC_Y)
#define RA_F RALT_T(KC_F)
#define RC_N RCTL_T(KC_N)
#define RS_B RSFT_T(KC_B)

#define LT_TAB LT(LAYER_NUMBER, KC_TAB)
#define LT_SPC LT(LAYER_FUNCTION, KC_SPC)
#define LT_ESC LT(LAYER_CURSOR, KC_ESC)

#define LT_BSPC LT(LAYER_POINTER, KC_BSPC)
#define LT_DEL  LT(LAYER_SYMBOL, KC_DEL)
#define LT_ENT  LT(LAYER_SYSTEM, KC_ENT)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define DPI_RMOD KC_NO
#    define S_D_MOD KC_NO
#    define S_D_RMOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

enum userspace_keycodes {
  SELECT_WORD = CHARYBDIS_SAFE_RANGE + 1,
  STICKY_LAYER_TOGGLE,
};
#define STLT STICKY_LAYER_TOGGLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_charybdis_4x6(
  // ╭───────────────────────────────────────────────────────────╮ ╭────────────────────────────────────────────────────────────╮
          KC_GRAVE,    KC_1,    KC_2,    KC_3,    KC_4,     KC_5,         KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_EQUAL,
  // ├───────────────────────────────────────────────────────────┤ ├────────────────────────────────────────────────────────────┤
       KC_LBRC,    KC_U,    KC_I,    KC_O,    KC_A, KC_QUOTE,           KC_SCLN,    KC_R,    KC_P,    KC_S,    KC_M, KC_RBRC,
  // ├───────────────────────────────────────────────────────────┤ ├────────────────────────────────────────────────────────────┤
           KC_COMM,    LG_T,    LA_H,    LC_E,    LS_D,     KC_C,          KC_Z,    RS_B,    RC_N,    RA_F,  RG_Y,    KC_DOT,
  // ├───────────────────────────────────────────────────────────┤ ├────────────────────────────────────────────────────────────┤
           KC_LSFT,    KC_J,    KC_K,    KC_X,    KC_Q,    KC_MINS,       KC_SLSH,    KC_L, KC_V,  KC_W, KC_G, KC_RSFT,
  // ╰───────────────────────────────────────────────────────────┤ ├────────────────────────────────────────────────────────────╯
                                   LT_TAB, LT_SPC,   DF(LAYER_QWERTY),      LT_DEL,  LT_BSPC,
                                           LT_ESC,  KC_BSLS,     LT_ENT
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),



  [LAYER_NUMBER] = LAYOUT_charybdis_4x6(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,     KC_TILD, KC_CIRC, KC_HASH, KC_DLR,   KC_AT, KC_EXLM,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PDOT,    KC_PERC,   KC_P6,   KC_P7,  KC_P9, KC_COLN, KC_VOLU,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_NUM, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_PCMM,     KC_PPLS,   KC_P4,   KC_P5,  KC_P6, KC_PMNS, KC_MUTE,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
          STLT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DB_TOGG,    KC_PAST,   KC_P1,   KC_P2,  KC_P3, KC_PSLS, KC_VOLD,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, XXXXXXX, XXXXXXX,     KC_LPRN, KC_P0,
                                           XXXXXXX, XXXXXXX,     KC_RPRN
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_FUNCTION] = LAYOUT_charybdis_4x6(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,     KC_MSEL, KC_MPLY, KC_MNXT, KC_DLR, KC_MSTP, KC_EJCT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_VOLU,   KC_F6,   KC_F8,  KC_F9,  KC_F10,  KC_F13,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_SCRL, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_MUTE,   KC_F4,   KC_F5,  KC_F6,  KC_F11,  KC_F14,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
          STLT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  DB_TOGG,   KC_VOLD,   KC_F1,   KC_F2,  KC_F3,  KC_F12,  KC_F15,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  XXXXXXX, _______, XXXXXXX,    XXXXXXX, KC_BRIU,
                                           XXXXXXX, XXXXXXX,    KC_BRID
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

 [LAYER_CURSOR] = LAYOUT_charybdis_4x6(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, EE_CLR,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_CUT,  KC_UNDO,  KC_UP, KC_AGIN,  KC_INS, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_CAPS, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_COPY, KC_LEFT, KC_DOWN, KC_RIGHT, SELECT_WORD,KC_PSCR,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
          STLT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  DB_TOGG,   KC_PSTE, KC_HOME, KC_PGUP,  KC_PGDN,  KC_END, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX,
                                           _______, XXXXXXX,    XXXXXXX
  //                            ╰───────────────────────────╯ ╰──────────────────╯

  ),

 [LAYER_POINTER] = LAYOUT_charybdis_4x6(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       XXXXXXX, DPI_RMOD, DPI_MOD, S_D_RMOD, S_D_MOD, XXXXXXX,    EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_ACL2, KC_WH_L, KC_MS_U, KC_WH_R, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       DRGSCRL, KC_ACL0, KC_MS_L, KC_MS_D, KC_MS_R, SNIPING,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       DRG_TOG, KC_ACL1, KC_WH_D, XXXXXXX, KC_WH_U, SNP_TOG,    DB_TOGG,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, STLT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  KC_BTN1, KC_BTN3, KC_BTN4,    XXXXXXX, _______,
                                           KC_BTN2, KC_BTN5,    XXXXXXX
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_SYMBOL] = LAYOUT_charybdis_4x6(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_TILD, KC_COMM, KC_LPRN, KC_RPRN, KC_SCLN, KC_QUES,    EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         KC_AT, KC_LCBR, KC_DQUO, KC_QUOT, KC_RCBR,  KC_DOT,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_HASH, KC_CIRC, KC_EQL, KC_UNDS, KC_DLR,  KC_ASTR,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_EXLM, KC_LT, KC_PIPE, KC_MINS, KC_GT,   KC_SLSH,    DB_TOGG,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, STLT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  KC_BSLS, KC_COLN, KC_PERC,    _______, KC_LBRC,
                                           KC_AMPR, KC_PLUS,    KC_RBRC
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),



  [LAYER_SYSTEM] = LAYOUT_charybdis_4x6(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_PWR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RGB_M_X, RGB_M_SW, RGB_M_SN, XXXXXXX, RGB_M_T, RGB_M_R,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RGB_M_K, RGB_VAD, RGB_RMOD, RGB_MOD, RGB_VAI, RGB_M_G,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RGB_M_P, RGB_SAD, RGB_HUD, RGB_HUI, RGB_SAI, RGB_M_B,    DB_TOGG,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, STLT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  XXXXXXX, KC_PAUS, RGB_TOG,    XXXXXXX, XXXXXXX,
                                           KC_WAKE, KC_SLEP,    _______
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_QWERTY] = LAYOUT_charybdis_4x6(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_GRAVE,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_LBRC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_COMM, LGUI_T(KC_A), LALT_T(KC_S), LCTL_T(KC_D), LSFT_T(KC_F),    KC_G,       KC_H,LSFT_T(KC_J),    LCTL_T(KC_K),    LALT_T(KC_L),    LGUI_T(KC_SCLN), KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                   LT_TAB, LT_SPC, DF(LAYER_BASE),      LT_DEL,  LT_BSPC,
                                           LT_ESC, KC_MINS ,     LT_ENT
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),
};


bool process_record_user(uint16_t keycode, keyrecord_t* record) {

    if (!process_select_word(keycode, record, SELECT_WORD)) { return false; }
    // handle sticky holding of Miryoku thumb cluster layer-taps as toggles
    if (keycode == STLT && record->event.pressed) {
        default_layer_set(default_layer_state ? 0 : (1 << get_highest_layer(layer_state)));
    }

    return true;
}

// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_EFFECT_MAX);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif

// #ifdef RGB_MATRIX_ENABLE
// void keyboard_post_init_user(void) {
//     // https://docs.qmk.fm/#/feature_rgb_matrix?id=rgb-matrix-effects
//     // set initial effect on keyboard start; ignore what's in EEPROM!
// //     rgb_matrix_mode_noeeprom(
// //       // RGB_MATRIX_TYPING_HEATMAP
// //       RGB_MATRIX_KEYREACTIVE_ENABLED
// //     );
// }
// #endif

void shutdown_user(void) {
#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(1);
    rgblight_setrgb_red(RGB_RED);
#endif // RGBLIGHT_ENABLE
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_color_all(RGB_RED);
    rgb_matrix_update_pwm_buffers();
#endif // RGB_MATRIX_ENABLE
}

#ifdef RGB_MATRIX_ENABLE

// dim indicator colors by this much (default is MAX brightness!)
#define DIM_BY 2
#define DIMMED(rgb_color) DIMMED_(rgb_color)
#define DIMMED_(r, g, b) ((r)/DIM_BY), ((g)/DIM_BY), ((b)/DIM_BY)

#define INDEX_FINGER_COLOR  DIMMED(RGB_YELLOW)
#define MIDDLE_FINGER_COLOR DIMMED(RGB_GREEN)
#define RING_FINGER_COLOR   DIMMED(RGB_CYAN)
#define PINKY_FINGER_COLOR  DIMMED(RGB_MAGENTA)
#define LEFT_THUMB_COLOR    DIMMED(RGB_RED)
#define RIGHT_THUMB_COLOR   DIMMED(RGB_PURPLE)
#define EXTRA_LAYER_COLOR   DIMMED(RGB_BLUE)
#define CAPS_LOCK_COLOR     DIMMED(RGB_BLUE)
#define CAPS_WORD_COLOR     DIMMED(RGB_SPRINGGREEN)

#define MOD_MASK_LCTRL (MOD_BIT(KC_LEFT_CTRL))
#define MOD_MASK_RCTRL (MOD_BIT(KC_RIGHT_CTRL))
#define MOD_MASK_LSHIFT (MOD_BIT(KC_LEFT_SHIFT))
#define MOD_MASK_RSHIFT (MOD_BIT(KC_RIGHT_SHIFT))
#define MOD_MASK_LALT (MOD_BIT(KC_LEFT_ALT))
#define MOD_MASK_RALT (MOD_BIT(KC_RIGHT_ALT))
#define MOD_MASK_LGUI (MOD_BIT(KC_LEFT_GUI))
#define MOD_MASK_RGUI (MOD_BIT(KC_RIGHT_GUI))

// see ../../4x6.c
/**
 * \brief LEDs index.
 *
 * ╭────────────────────────╮                 ╭────────────────────────╮
 *    0   7   8  15  16  20                     49  45  44  37  36  29
 * ├────────────────────────┤                 ├────────────────────────┤
 *    1   6   9  14  17  21                     50  46  43  38  35  30
 * ├────────────────────────┤                 ├────────────────────────┤
 *    2   5  10  13  18  22                     51  47  42  39  34  31
 * ├────────────────────────┤                 ├────────────────────────┤
 *    3   4  11  12  19  23                     52  48  41  40  33  32
 * ╰────────────────────────╯                 ╰────────────────────────╯
 *                       26  27  28     53  54  XX
 *                           25  24     55  XX
 *                     ╰────────────╯ ╰────────────╯
 *
 * Note: the LED config simulates 58 LEDs instead of the actual 56 to prevent
 * confusion when testing LEDs during assembly when handedness is not set
 * correctly.  Those fake LEDs are bound to the physical bottom-left corner.〕
 */
static const uint8_t
    LEFT_INDEX_FINGER_HOME   = 16,  RIGHT_INDEX_FINGER_HOME   = 45,
    LEFT_MIDDLE_FINGER_HOME  = 15,  RIGHT_MIDDLE_FINGER_HOME  = 44,
    LEFT_RING_FINGER_HOME    = 8, RIGHT_RING_FINGER_HOME    = 37,
    LEFT_PINKY_FINGER_HOME   = 7, RIGHT_PINKY_FINGER_HOME   = 36,
    LEFT_THUMB_CLUSTER_HOME  = 21,  RIGHT_THUMB_CLUSTER_HOME  = 50,
    LEFT_THUMB_CLUSTER_INNER = 22,  RIGHT_THUMB_CLUSTER_INNER = 51,
    LEFT_THUMB_CLUSTER_OUTER = 23,  RIGHT_THUMB_CLUSTER_OUTER = 52,
    LEFT_CAPS_LOCK           = 20,  RIGHT_CAPS_LOCK           = 49;


void rgb_matrix_indicators_user(void) {

    uint8_t mods = get_mods();

    if (mods & MOD_MASK_LSHIFT) {
        rgb_matrix_set_color( LEFT_INDEX_FINGER_HOME, INDEX_FINGER_COLOR);
    }
    if (mods & MOD_MASK_RSHIFT) {
        rgb_matrix_set_color(RIGHT_INDEX_FINGER_HOME, INDEX_FINGER_COLOR);
    }

    if (mods & MOD_MASK_LCTRL) {
        rgb_matrix_set_color( LEFT_MIDDLE_FINGER_HOME, MIDDLE_FINGER_COLOR);
    }

    if (mods & MOD_MASK_RCTRL) {
        rgb_matrix_set_color(RIGHT_MIDDLE_FINGER_HOME, MIDDLE_FINGER_COLOR);
    }

    if (mods & MOD_MASK_LALT) {
        rgb_matrix_set_color( LEFT_RING_FINGER_HOME, RING_FINGER_COLOR);
    }

    if (mods & MOD_MASK_RALT) {
        rgb_matrix_set_color(RIGHT_RING_FINGER_HOME, RING_FINGER_COLOR);
    }

    if (mods & MOD_MASK_LGUI) {
        rgb_matrix_set_color( LEFT_PINKY_FINGER_HOME, PINKY_FINGER_COLOR);
    }

    if (mods & MOD_MASK_RGUI) {
        rgb_matrix_set_color(RIGHT_PINKY_FINGER_HOME, PINKY_FINGER_COLOR);
    }

    switch (get_highest_layer(layer_state|default_layer_state)) {
        case 1:
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  LEFT_THUMB_COLOR);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  LEFT_THUMB_COLOR);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, RGB_OFF);
            break;

        case 2:
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, LEFT_THUMB_COLOR);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, LEFT_THUMB_COLOR);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, RGB_OFF);
            break;

        case 3:
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, LEFT_THUMB_COLOR);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, LEFT_THUMB_COLOR);
            break;

        case 4:
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  RIGHT_THUMB_COLOR);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  RIGHT_THUMB_COLOR);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, RGB_OFF);
            break;

        case 5:
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, RIGHT_THUMB_COLOR);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, RIGHT_THUMB_COLOR);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, RGB_OFF);
            break;

        case 6:
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, RIGHT_THUMB_COLOR);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, RIGHT_THUMB_COLOR);
            break;
        case 7:
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  EXTRA_LAYER_COLOR);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  EXTRA_LAYER_COLOR);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, RGB_OFF);
            break;
        case 8:
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, EXTRA_LAYER_COLOR);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, EXTRA_LAYER_COLOR);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, RGB_OFF);
            break;
        case 9:
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, RGB_OFF);
            rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, EXTRA_LAYER_COLOR);
            rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, EXTRA_LAYER_COLOR);
            break;
        default:
            // rgb_matrix_set_color( LEFT_THUMB_CLUSTER_HOME,  RGB_OFF);
            // rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_HOME,  RGB_OFF);
            // rgb_matrix_set_color( LEFT_THUMB_CLUSTER_INNER, RGB_OFF);
            // rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_INNER, RGB_OFF);
            // rgb_matrix_set_color( LEFT_THUMB_CLUSTER_OUTER, RGB_OFF);
            // rgb_matrix_set_color(RIGHT_THUMB_CLUSTER_OUTER, RGB_OFF);
            break;
    }

    if (host_keyboard_led_state().caps_lock) {
      rgb_matrix_set_color( LEFT_CAPS_LOCK, CAPS_LOCK_COLOR);
      rgb_matrix_set_color(RIGHT_CAPS_LOCK, CAPS_LOCK_COLOR);
    }
    else if (is_caps_word_on()) {
      rgb_matrix_set_color( LEFT_CAPS_LOCK, CAPS_WORD_COLOR);
      rgb_matrix_set_color(RIGHT_CAPS_LOCK, CAPS_WORD_COLOR);
    }
}
#endif // RGB_MATRIX_ENABLE
