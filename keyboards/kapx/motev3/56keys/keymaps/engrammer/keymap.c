// Copyright 2022 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "keycodes.h"
#include QMK_KEYBOARD_H

#include "hid_display.h"
#include "display.h"
#include "raw_hid.h"
#include "transactions.h"
#include "user_song_list.h"
#include "drivers/haptic/drv2605l.h"

#include "sm_td.h"
#include "select_word.h"


smtd_resolution on_smtd_action(uint16_t keycode, smtd_action action, uint8_t tap_count) {
    switch (keycode) {
        SMTD_MT(KC_C, KC_LEFT_GUI)
        SMTD_MT(KC_I, KC_LEFT_ALT)
        SMTD_MT(KC_E, KC_LEFT_CTRL)
        SMTD_MT(KC_A, KC_LSFT)
        SMTD_MT(KC_N, KC_RIGHT_GUI)
        SMTD_MT(KC_S, KC_RIGHT_ALT)
        SMTD_MT(KC_T, KC_RIGHT_CTRL)
        SMTD_MT(KC_H, KC_RSFT)
    }

    return SMTD_RESOLUTION_UNHANDLED;
}

#define L_C LGUI_T(KC_C)
#define L_I LALT_T(KC_I)
#define L_E LCTL_T(KC_E)
#define L_A LSFT_T(KC_A)

#define R_N RGUI_T(KC_N)
#define R_S RALT_T(KC_S)
#define R_T RCTL_T(KC_T)
#define R_H RSFT_T(KC_H)


// bottom mods
#define SYM_SPC LT(SYM, KC_SPC)
#define NUM_TAB LT(NUM, KC_TAB)
#define SYS_ESC LT(SYS, KC_ESC)
#define SYM_ENT LT(SYM, KC_ENT)
#define SYS_BSPC LT(SYS, KC_BSPC)
#define RALT_DEL LALT_T(KC_DEL)
#define SYS_WIN LT(SYS, KC_LGUI)

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
    DRG_DN,  // 减少滚动模式DPI
    SELWORD // select word
};
uint16_t SELECT_WORD_KEYCODE = SELWORD;
// 声明一个全局变量来存储当前的CPI值
static uint16_t current_cpi = CPI_DEFAULT;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* QWERTY
     *        .----------------------------------.                    .----------------------------------.
     *        |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |
     * .------+------+------+------+------+------|                    |------+------+------+------+------+------.
     * |shift |   A  |   S  |   D  |   F  |   G  |                    |   H  |   J  |   K  |   L  |   ;  |  '   |
     * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
     * |ctrl  |   Z  |   X  |   C  |   V  |   B  |--------.  .--------|   N  |   M  |   ,  |   .  |   /  |  `   |
     * '-----------------------------------------/       /    \       \-----------------------------------------'
     *                         | Esc  | Tab  |  / Space /      \ Enter \  | Bsps | Del  |
     *                         |_FUNC | _NUM | /_SYMBOL/        \ _FUNC \ | _NAV | RAlt |
     *                         `-------------''-------'          '-------''-------------'
     */
    // clang-format off
  [DEF] = LAYOUT(
  // ╭───────────────────────────────────────────────────────────────╮ ╭───────────────────────────────────────────────────────────╮
        LT(SYS, KC_GRAVE),  KC_1,   KC_2,   KC_3,   KC_4,      KC_5,        KC_6,   KC_7,   KC_8,   KC_9,   KC_0,  LT(SYS,KC_RSFT),
  // ├───────────────────────────────────────────────────────────────┤ ├───────────────────────────────────────────────────────────┤
        KC_LBRC,            KC_B,   KC_Y,   KC_O,   KC_U,   KC_QUOT,     KC_SCLN,   KC_L,   KC_D,   KC_W,   KC_V,          KC_RBRC,
  // ├───────────────────────────────────────────────────────────────┤ ├───────────────────────────────────────────────────────────┤
        KC_COMM,             L_C,    L_I,    L_E,    L_A,      KC_Z,        KC_Q,   R_H,     R_T,    R_S,    R_N,           KC_DOT,
  // ├───────────────────────────────────────────────────────────────┤ ├───────────────────────────────────────────────────────────┤
        KC_SLSH,            KC_G,   KC_X,    KC_J,  KC_K,   KC_MINS,      KC_EQL,   KC_R,   KC_M,   KC_F,    KC_P,         KC_BSLS,
  // ╰───────────────────────────────────────────────────────────────┤ ├───────────────────────────────────────────────────────────╯
                  KC_TAB, LT(NUM,KC_LCTL),   LT(FN,KC_ESC), KC_MUTE,     KC_MUTE, LT(SYM,KC_SPC), LT(FN,KC_ENTER),  LT(MOU,KC_BSPC),
  //                            ╰────────────────────────────────────╯ ╰──────────────────────────────╯
               MO(2),   MO(3),   MO(1),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
                            SYS_ESC, KC_1, RM_PREV,              RM_SPDU, KC_1, SYS_BSPC
    ),
    [NUM] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_TILD, KC_CIRC, KC_HASH, KC_DLR,   KC_AT, KC_EXLM,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_PERC,   KC_P7,   KC_P8,  KC_P9, KC_COLN, KC_VOLU,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         KC_NUM, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_PCMM,     KC_PPLS,  KC_P4,   KC_P5,  KC_P6, KC_PMNS, KC_MUTE,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         TG(NUM), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_PAST,   KC_P1,   KC_P2,  KC_P3, KC_PSLS, KC_VOLD,
    // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                        XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX,    KC_MUTE, KC_RPRN , KC_LPRN, KC_P0,
    //                            ╰───────────────────────────╯ ╰──────────────────╯
      MO(2),   MO(3),   MO(1),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
                            SYS_ESC, KC_1, RM_PREV,              RM_SPDU, KC_1, SYS_BSPC
    ),
    [FN] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_MSEL, KC_MPLY, KC_MNXT, KC_DLR, KC_MSTP, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_VOLU,   KC_F7,   KC_F8,  KC_F9,  KC_F10,  KC_F13,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_SCRL, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_MUTE,   KC_F4,   KC_F5,  KC_F6,  KC_F11,  KC_F14,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         TG(FN), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   KC_VOLD,   KC_F1,   KC_F2,  KC_F3,  KC_F12,  KC_F15,
    // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                            XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX,    XXXXXXX, XXXXXXX , KC_BRID, KC_BRIU,
    //                            ╰───────────────────────────╯ ╰──────────────────╯
      MO(2),   MO(3),   MO(1),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
                            SYS_ESC, KC_1, RM_PREV,              RM_SPDU, KC_1, SYS_BSPC
    ),
    [NAV] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_CUT,  KC_UNDO,  KC_UP, KC_AGIN,  KC_INS, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_CAPS, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_COPY, KC_LEFT, KC_DOWN, KC_RIGHT, SELWORD, KC_PSCR,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        TG(NAV), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_PSTE, KC_HOME, KC_PGUP,  KC_PGDN,  KC_END, XXXXXXX,
    // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                            XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX,    XXXXXXX, XXXXXXX , XXXXXXX, XXXXXXX,
    //                            ╰───────────────────────────╯ ╰──────────────────╯
      MO(2),   MO(3),   MO(1),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
                            SYS_ESC, KC_1, RM_PREV,              RM_SPDU, KC_1, SYS_BSPC
    ),
    [SYM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_TILD, KC_COMM, KC_LPRN, KC_RPRN, KC_SCLN, KC_QUES,    EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         KC_AT, KC_LCBR, KC_DQUO, KC_QUOT, KC_RCBR,  KC_DOT,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_HASH, KC_CIRC, KC_EQL, KC_UNDS, KC_DLR,  KC_ASTR,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_EXLM, KC_LT, KC_PIPE, KC_MINS, KC_GT,   KC_SLSH,     XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(SYM),
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                    KC_BSLS, KC_COLN,  KC_PERC,   KC_AMPR,     KC_PLUS, XXXXXXX,  KC_LBRC ,KC_RBRC,
  //                            ╰───────────────────────────╯ ╰──────────────────╯
    MO(2),   MO(3),   MO(1),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
        SYS_ESC, KC_1, RM_PREV,              RM_SPDU, KC_1, SYS_BSPC
    ),
     [MOU] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_ACL2, KC_WH_L, KC_MS_U, KC_WH_R, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_ACL0, KC_MS_L, KC_MS_D, KC_MS_R, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         DRG_T, KC_ACL1, KC_WH_D, XXXXXXX, KC_WH_U, SNP_T,        DB_TOGG,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(MOU),
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                            KC_BTN1, KC_BTN2,  KC_BTN3,   XXXXXXX,     XXXXXXX, XXXXXXX,  XXXXXXX ,XXXXXXX,
    //                            ╰───────────────────────────╯ ╰──────────────────╯
    MO(2),   MO(3),   MO(1),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
        SYS_ESC, KC_1, RM_PREV,              RM_SPDU, KC_1, SYS_BSPC
    ),
    [SYS] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,  EE_CLR,     EE_CLR, QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        RGB_M_X, RGB_M_SW, RGB_M_SN, XXXXXXX, RGB_M_T, RGB_M_R,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        RGB_M_K, RGB_VAD, RGB_RMOD, RGB_MOD, RGB_VAI, RGB_M_G,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        RGB_M_P, RGB_SAD, RGB_HUD, RGB_HUI, RGB_SAI, RGB_M_B,    DB_TOGG,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TG(SYS),
    // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
            XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX,    XXXXXXX, XXXXXXX , XXXXXXX, XXXXXXX,
            //                            ╰───────────────────────────╯ ╰──────────────────╯
    MO(2),   MO(3),   MO(1),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
        SYS_ESC, KC_1, RM_PREV,              RM_SPDU, KC_1, SYS_BSPC
    ),
    [GAME] = LAYOUT(
    // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_GRAVE,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,      KC_6,    KC_7,    KC_8,    KC_9,    KC_0, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         KC_MINS,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,   XXXXXXX, XXXXXXX,   KC_UP, XXXXXXX, XXXXXXX, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         KC_PLUS,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,   XXXXXXX, KC_LEFT, KC_DOWN,KC_RIGHT, XXXXXXX, XXXXXXX,
    // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
         KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                            XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
            //                            ╰───────────────────────────╯ ╰──────────────────╯
    MO(2),   MO(3),   MO(1),    MO(4),   MO(5),      KC_ENT,  KC_RGHT,  KC_DOWN, KC_LEFT, KC_UP,
        SYS_ESC, KC_1, RM_PREV,              RM_SPDU, KC_1, SYS_BSPC
    ),
    // clang-format on
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    // clang-format off
    [DEF] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [NUM] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [FN]  = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [NAV] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [SYM] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MOU] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [SYS] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
   [GAME] = { ENCODER_CCW_CW(KC_LEFT, KC_RGHT), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }
    // clang-format on
};
#endif // ENCODER_MAP_ENABLE

/* Caps Word processing */
#ifdef CAPS_WORD_ENABLE
void caps_word_set_user(bool active) {
    if (is_display_enabled()) {
        display_process_caps(active);
    } else if (is_keyboard_master() && !is_display_side()) {
        dprintf("RPC_ID_USER_CAPS_WORD_SYNC: %s\n", active ? "active" : "inactive");
        transaction_rpc_send(RPC_ID_USER_CAPS_WORD_SYNC, 1, &active);
    }
}
#endif

// 加入切层音效
float gitar_song[][2] = SONG(GUITAR_SOUND);
float qwert_song[][2] = SONG(QWERTY_SOUND);
float game_song[][2]  = SONG(TETRIS_SONG);

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (scroll_mode) {
        // 计算滚动值
        int8_t scroll_h = (int8_t)(mouse_report.x * scroll_cpi / 100);
        int8_t scroll_v = (int8_t)(-mouse_report.y * scroll_cpi / 100);

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
    if (!process_select_word(keycode, record)) { return false; }
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

// 切层行为
layer_state_t layer_state_set_user(layer_state_t state) {
    if (is_display_enabled()) {
        display_process_layer_state(get_highest_layer(state));
    } else if (is_keyboard_master() && !is_display_side()) {
        uint8_t layer = get_highest_layer(state);
        dprintf("RPC_ID_USER_LAYER_SYNC: %u\n", layer);
        transaction_rpc_send(RPC_ID_USER_LAYER_SYNC, 1, &layer);
    }
    // 切层音效
    static bool is_sys_enabled = false, is_game_enabled = false, is_symbol_enabled = false;
    if (layer_state_cmp(state, SYS) != is_sys_enabled || layer_state_cmp(state, GAME) != is_game_enabled || layer_state_cmp(state, SYM) != is_symbol_enabled) {
        is_sys_enabled    = layer_state_cmp(state, SYS);
        is_game_enabled   = layer_state_cmp(state, GAME);
        is_symbol_enabled = layer_state_cmp(state, SYM);

        if (is_sys_enabled) {
            PLAY_SONG(gitar_song);
        } else if (is_symbol_enabled) {
            PLAY_SONG(qwert_song);
        } else if (is_game_enabled) {
            PLAY_SONG(game_song);
        }
    }
    // 切层震动
    switch (get_highest_layer(state)) {
        case 1:
            drv2605l_pulse(DRV2605L_EFFECT_SOFT_BUMP_100);
            break;
        case 3:
            drv2605l_pulse(DRV2605L_EFFECT_LONG_DOUBLE_SHARP_CLICK_STRONG_1_100);
            break;
        case 4:
            drv2605l_pulse(DRV2605L_EFFECT_SOFT_BUMP_100);
            break;
        case 5:
            drv2605l_pulse(DRV2605L_EFFECT_PULSING_SHARP_1_100);
            break;
        case 6:
            drv2605l_pulse(DRV2605L_EFFECT_SHORT_DOUBLE_SHARP_TICK_1_100);
            break;
        case 7:
            drv2605l_pulse(DRV2605L_EFFECT_SHORT_DOUBLE_SHARP_TICK_1_100);
            break;
    }

    return state;
}

/* Raw HID processing*/
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    dprintf("raw_hid_receive - received %u bytes \n", length);

    if (is_display_enabled()) {
        display_process_raw_hid_data(data, length);
    } else if (is_keyboard_master() && !is_display_side()) {
        dprint("RPC_ID_USER_HID_SYNC \n");
        transaction_rpc_send(RPC_ID_USER_HID_SYNC, length, data);
    }
}

void hid_sync(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
    if (is_display_enabled()) {
        display_process_raw_hid_data((uint8_t *)initiator2target_buffer, initiator2target_buffer_size);
    }
}

void layer_sync(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
    if (is_display_enabled()) {
        display_process_layer_state(*(uint8_t *)initiator2target_buffer);
    }
}

void caps_word_sync(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
    if (is_display_enabled()) {
        display_process_caps(*(bool *)initiator2target_buffer);
    }
}

void keyboard_post_init_user() {
    // sync received hid data
    transaction_register_rpc(RPC_ID_USER_HID_SYNC, hid_sync);
    // sync highest layer (a bit more performant than standard SPLIT_LAYER_STATE_ENABLE)
    transaction_register_rpc(RPC_ID_USER_LAYER_SYNC, layer_sync);
    // sync caps word state
    transaction_register_rpc(RPC_ID_USER_CAPS_WORD_SYNC, caps_word_sync);
}

// 鼠标自动切层
void pointing_device_init_user(void) {
    set_auto_mouse_layer(MOU);  // only required if AUTO_MOUSE_DEFAULT_LAYER is not set to index of <mouse_layer>
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
