// Copyright 2022 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define VIAL_KEYBOARD_UID \
    { 0x37, 0xD1, 0xA9, 0xDF, 0x86, 0xA2, 0xC5, 0xCA }

/* VIAL secure unlock keystroke - currently both inner thumb keys (typ. SPACE/ENTER) */
#define VIAL_UNLOCK_COMBO_ROWS \
    { 0, 7 }
#define VIAL_UNLOCK_COMBO_COLS \
    { 0, 0 }

#define DYNAMIC_KEYMAP_LAYER_COUNT 8
#define VIAL_COMBO_ENTRIES 10
#define DYNAMIC_KEYMAP_MACRO_COUNT 20

#define SPLIT_LAYER_STATE_ENABLE         // 开启层状态同步，会让左边闪烁。。。最好不开
#define SPLIT_WPM_ENABLE
#define SPLIT_POINTING_ENABLE
#define SPLIT_HAPTIC_ENABLE
#define SPLIT_ACTIVITY_ENABLE           // 开启层状态同步，会让左边闪烁。。。最好不开
#define SPLIT_TRANSPORT_MIRROR          // 开启层状态同步，会让左边闪烁。。。最好不开
// #define SPLIT_LED_STATE_ENABLE
// #define SPLIT_MODS_ENABLE
// #define SPLIT_OLED_ENABLE

