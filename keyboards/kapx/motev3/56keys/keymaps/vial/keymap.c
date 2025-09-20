// Copyright 2023 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "user_song_list.h"
#include "drivers/haptic/drv2605l.h"

enum layer_number {
    // clang-format off
    _QWERTY = 0,
    _GAME,
    _NAV,
    _NUMBER,
    _SYMBOL,
    _FUNC,
    _SYS
    // clang-format on
};

// Left-hand home row mods
#define HOME_A LGUI_T(KC_A)
#define HOME_S LALT_T(KC_S)
#define HOME_D LCTL_T(KC_D)
#define HOME_F LSFT_T(KC_F)

// Right-hand home row mods
#define HOME_J RSFT_T(KC_J)
#define HOME_K RCTL_T(KC_K)
#define HOME_L LALT_T(KC_L)
#define HOME_SCLN RGUI_T(KC_SCLN)

// bottom mods
#define SYM_SPC LT(_SYMBOL, KC_SPC)
#define NUM_TAB LT(_NUMBER, KC_TAB)
#define SYS_ESC LT(_SYS, KC_ESC)
#define SYM_ENT LT(_SYMBOL, KC_ENT)
#define SYS_BSPC LT(_SYS, KC_BSPC)
#define RALT_DEL LALT_T(KC_DEL)
#define SYS_WIN LT(_SYS, KC_LGUI)

// game layer mods
#define LALT_EQL LALT_T(KC_EQL)
#define LSFT_MINS LSFT_T(KC_MINS)
#define LCTL_ESC LCTL_T(KC_ESC)
#define LGUI_GRV LGUI_T(KC_GRV)

// 阻击模式相关定义
#define SNP_MIN_CPI 50
#define SNP_MAX_CPI 500
#define SNP_STEP 50
#define SNP_DEFAULT_CPI 100

// 声明一个全局变量来存储阻击模式的CPI值
static uint16_t sniper_cpi  = SNP_DEFAULT_CPI;
static bool     sniper_mode = false;

// 滚动模式相关定义
#define SCROLL_MIN_CPI 1
#define SCROLL_MAX_CPI 30
#define SCROLL_STEP 1
#define SCROLL_DEFAULT_CPI 12

// 声明全局变量来存储滚动模式的CPI值
static uint16_t scroll_cpi  = SCROLL_DEFAULT_CPI;
static bool     scroll_mode = false;

enum custom_keycodes {
    DPI_UP = QK_KB_0, // SAFE_RANGE,
    DPI_DOWN,
    //    DRAG_SCROLL,
    SNP,    // 长按进入阻击模式
    SNP_T,  // 切换阻击模式
    SNP_UP, // 增加阻击模式DPI
    SNP_DN, // 减少阻击模式DPI
    DRG,    // 长按进入滚动模式
    DRG_T,  // 切换滚动模式
    DRG_UP, // 增加滚动模式DPI
    DRG_DN  // 减少滚动模式DPI
};

// 声明一个全局变量来存储当前的CPI值
static uint16_t current_cpi = CPI_DEFAULT;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* QWERTY
     *        .----------------------------------.                    .----------------------------------.
     *        |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |
     * .------+------+------+------+------+------|                    |------+------+------+------+------+------.
     * |  =   |   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |  '   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |  -   |   Z  |   X  |   C  |   V  |   B  |--------.  .--------|   N  |   M  |   ,  |   .  |   /  |  `   |
     * '-----------------------------------------/       /    \       \-----------------------------------------'
     *                         | Esc  | Tab  |  / Space /      \ Enter \  | Bsps | Del  |
     *                         |_FUNC | _NUM | /_SYMBOL/        \ _FUNC \ | _NAV | RAlt |
     *                         `-------------''-------'          '-------''-------------'
     */
    // clang-format off
    [_QWERTY] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      SYS_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, SYS_BSPC,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_MINUS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_EQUAL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_QUOT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                      KC_LALT,  KC_SPACE, KC_LGUI, KC_MUTE,      KC_MUTE,MO(6),  SYM_ENT, RALT_DEL,
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
               MO(2),   MO(3),   MO(1),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
                            RM_NEXT, KC_1, RM_PREV,              RM_SPDU, KC_1, RM_SPDD
    ),

    [_GAME] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      LCTL_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_MINUS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      LSFT_MINS,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_EQUAL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_QUOT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                      LALT_EQL,  KC_SPC, LGUI_GRV, KC_BSPC,      KC_MUTE,KC_LALT,  SYM_ENT, KC_LALT,
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
               MO(1),   MO(3),   MO(2),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
                                                RM_NEXT, KC_1, RM_PREV,              RM_SPDU, KC_1, RM_SPDD

    ),

    [_NAV] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_F12,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MNXT, _______, _______, KC_UP, _______, _______,    _______, _______, _______, _______, _______, KC_VOLU,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPLY, _______,   KC_LEFT, KC_DOWN, KC_RGHT, _______,    _______, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, KC_MUTE,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPRV, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, _______,    _______, _______, _______, _______, _______, KC_VOLD,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         DRG, KC_BTN1, KC_BTN2, _______,    _______, _______, _______, _______,
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    _______,  _______,    _______,    _______,    _______,                 _______,  _______,    _______,    _______,   _______,
                                     KC_1, KC_1, KC_1,              KC_1, KC_1, KC_1

    ),

    [_NUMBER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
      KC_ESC,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_MINUS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_EQUAL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_QUOT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                      KC_LALT,  KC_SPC, KC_LALT, KC_BSPC,      KC_MUTE,KC_LALT,  SYM_ENT, KC_LALT,
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
               MO(1),   MO(3),   MO(2),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
                                                KC_1, KC_1, KC_1,              KC_1, KC_1, KC_1

    ),

 [_SYMBOL] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_GRV, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_CAPS_LOCK, _______, _______, _______, _______, _______,    _______, _______, KC_LCBR, KC_RCBR, _______, KC_PLUS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        _______, _______, _______,   _______, _______,  _______,    _______, _______, KC_LPRN, KC_RPRN, _______, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       _______, _______, _______, _______,  _______, _______,   KC_PLUS, KC_UNDS, KC_LBRC, KC_RBRC, KC_BSLS, KC_PIPE,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         _______, _______, _______, _______,    _______, _______, _______, _______,
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    _______,  _______,    _______,    _______,    _______,                 _______,  _______,    _______,    _______,   _______,
                                     KC_1, KC_1, KC_1,              KC_1, KC_1, KC_1

    ),

    [_FUNC] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_F12,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MNXT, _______, _______, _______, _______, _______,    _______, _______, _______, _______, _______, KC_VOLD,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPLY, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, _______,    _______, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, KC_VOLU,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPRV, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, _______,    _______, _______, _______, _______, _______, KC_MUTE,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         _______, _______, _______, _______,    _______, _______, _______, _______,
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    _______,  _______,    _______,    _______,    _______,                 _______,  _______,    _______,    _______,   _______,
                                     KC_1, KC_1, KC_1,              KC_1, KC_1, KC_1


    ),
    [_SYS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       _______, QK_BOOT, EE_CLR, _______,  _______, _______,    RGB_M_P, _______, _______, EE_CLR, QK_BOOT, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RGB_SPI, HF_PREV, HF_CONU, _______, _______, _______,    RGB_M_B,   _______,   _______,   _______, KC_RBRC, _______,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RGB_TOG, HF_TOGG, _______, CK_TOGG, AU_TOGG, _______,    RGB_M_R,   _______,   _______,   _______, KC_PMNS, KC_PEQL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RGB_SPD, HF_NEXT, HF_COND, _______, _______, _______,    RGB_M_SW,   _______,   _______,   _______, KC_PSLS, KC_PDOT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         _______, _______, _______, _______,     _______,   _______, _______, _______,
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    _______,  _______,    _______,    _______,    _______,                 _______,  _______,    _______,    _______,   _______,
                                     CK_UP, CK_TOGG, CK_DOWN,              HF_PREV, HF_TOGG, HF_NEXT

    )
    // clang-format on
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    // clang-format off
    [_QWERTY] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_GAME]   = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NAV]    = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_NUMBER] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_SYMBOL] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_FUNC]   = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_SYS]    = { ENCODER_CCW_CW(BL_DOWN, BL_UP),   ENCODER_CCW_CW(KC_MPRV, KC_MNXT) }
    // clang-format on
};
#endif // ENCODER_MAP_ENABLE


// 加入切层音效
float gitar_song[][2] = SONG(GUITAR_SOUND);
float qwert_song[][2] = SONG(QWERTY_SOUND);
float game_song[][2]  = SONG(TETRIS_SONG);

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (scroll_mode) {
        // 计算滚动值
        int8_t scroll_h = (int8_t)(mouse_report.x * scroll_cpi / 100);
        int8_t scroll_v = (int8_t)(mouse_report.y * scroll_cpi / 100);

        // 设置滚动值
        mouse_report.h = scroll_h;
        mouse_report.v = scroll_v;

        // 清除鼠标移动
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

// 函数用于调整CPI
void adjust_cpi(bool increase) {
    if (increase) {
        current_cpi += CPI_STEP;
    } else {
        current_cpi -= CPI_STEP;
    }
    if (current_cpi > CPI_MAX) current_cpi = CPI_MAX;
    if (current_cpi < CPI_MIN) current_cpi = CPI_MIN;
    // 设置新的CPI值
    pointing_device_set_cpi(current_cpi);
}

// Function to handle key events and enable/disable drag scrolling
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DPI_UP:
            if (record->event.pressed) {
                adjust_cpi(true);
            }
            return false;
        case DPI_DOWN:
            if (record->event.pressed) {
                adjust_cpi(false);
            }
            return false;
            //        case DRAG_SCROLL:
            //            // Toggle set_scrolling when DRAG_SCROLL key is pressed or released
            //            set_scrolling = record->event.pressed;
            //            break;

        case SNP:
            if (record->event.pressed) {
                sniper_mode = true;
                pointing_device_set_cpi(sniper_cpi);
            } else {
                sniper_mode = false;
                pointing_device_set_cpi(current_cpi);
            }
            return false;
        case SNP_T:
            if (record->event.pressed) {
                sniper_mode = !sniper_mode;
                pointing_device_set_cpi(sniper_mode ? sniper_cpi : current_cpi);
            }
            return false;
        case SNP_UP:
            if (record->event.pressed) {
                sniper_cpi += SNP_STEP;
                if (sniper_cpi > SNP_MAX_CPI) sniper_cpi = SNP_MIN_CPI;
                if (sniper_mode) pointing_device_set_cpi(sniper_cpi);
            }
            return false;
        case SNP_DN:
            if (record->event.pressed) {
                sniper_cpi -= SNP_STEP;
                if (sniper_cpi < SNP_MIN_CPI) sniper_cpi = SNP_MAX_CPI;
                if (sniper_mode) pointing_device_set_cpi(sniper_cpi);
            }
            return false;

        case DRG:
            if (record->event.pressed) {
                scroll_mode = true;
            } else {
                scroll_mode = false;
            }
            return false;
        case DRG_T:
            if (record->event.pressed) {
                scroll_mode = !scroll_mode;
            }
            return false;
        case DRG_UP:
            if (record->event.pressed) {
                scroll_cpi += SCROLL_STEP;
                if (scroll_cpi > SCROLL_MAX_CPI) scroll_cpi = SCROLL_MAX_CPI;
            }
            return false;
        case DRG_DN:
            if (record->event.pressed) {
                scroll_cpi -= SCROLL_STEP;
                if (scroll_cpi < SCROLL_MIN_CPI) scroll_cpi = SCROLL_MIN_CPI;
            }
            return false;

        default:
            break;
    }
    return true;
}

// 鼠标自动切层
void pointing_device_init_user(void) {
    set_auto_mouse_layer(_NAV);  // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of <mouse_layer>
    set_auto_mouse_enable(true); // always required before the auto mouse feature will work
}

// 切层底光换色
#ifdef RGB_MATRIX_ENABLE
// Layer state indicator
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }
    if (host_keyboard_led_state().caps_lock) {
        for (int i = led_min; i <= led_max; i++) {
            if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_INDICATOR)) {
                rgb_matrix_set_color(i, 15, 255, 247);
            }
        }
    }

    uint8_t layer = get_highest_layer(layer_state);
    if (layer > 0) {
        HSV hsv = rgb_matrix_get_hsv();
        switch (get_highest_layer(layer_state)) {
            case 1:
                hsv = (HSV){HSV_BLUE};
                break;
            case 2:
                hsv = (HSV){HSV_RED};
                break;
            case 3:
                hsv = (HSV){HSV_ORANGE};
                break;
            case 4:
                hsv = (HSV){HSV_GREEN};
                break;
            case 5:
                hsv = (HSV){HSV_TEAL};
                break;
            case 6:
                hsv = (HSV){HSV_PURPLE};
                break;
            case 7:
            default:
                hsv = (HSV){HSV_RED};
                break;
        };

        if (hsv.v > rgb_matrix_get_val()) {
            hsv.v = MIN(rgb_matrix_get_val() + 22, 255);
        }
        RGB rgb = hsv_to_rgb(hsv);

        for (uint8_t i = led_min; i < led_max; i++) {
            if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
        }
    }
    return false;
};
#endif // RGB_MATRIX_ENABLE
