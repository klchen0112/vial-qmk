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
#pragma once


#ifdef VIA_ENABLE
/* VIA configuration. */
#    define DYNAMIC_KEYMAP_LAYER_COUNT 8

#    ifdef VIAL_ENABLE
/** Vial-specific configuration. */

#        define VIAL_KEYBOARD_UID \
            { 0x6D, 0xA5, 0xCD, 0x8D, 0xC7, 0x3D, 0x7B, 0xA8 }
#        define VIAL_UNLOCK_COMBO_ROWS \
            { 0, 5 }
#        define VIAL_UNLOCK_COMBO_COLS \
            { 0, 0 }

/** Disable unused vial features. */

// In addition to RGB Matrix effects, VialRGB also provides direct LED control
// with a script running on your computer.  Remove to reenable.
#        define VIALRGB_NO_DIRECT
#    endif // VIAL_ENABLE
#endif     // VIA_ENABLE

/* Disable unused features. */
#define NO_ACTION_ONESHOT

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_MODS_ENABLE

#ifndef TAPPING_TERM
/**
 * \brief Configure the global tapping term (default: 200ms).
 *
 * If you have a lot of accidental mod activations, crank up the tapping term.
 *
 * See docs.qmk.fm/using-qmk/software-features/tap_hold#tapping-term
 */
#    define TAPPING_TERM 200


#endif // TAPPING_TERM


/* Miryoku */
#define BILATERAL_COMBINATIONS
#define BILATERAL_COMBINATIONS_LIMIT_CHORD_TO_N_KEYS 4 /* GUI, Alt, Ctrl, Shift */
#define BILATERAL_COMBINATIONS_DELAY_MODS_THAT_MATCH MOD_MASK_GUI
#define BILATERAL_COMBINATIONS_DELAY_MATCHED_MODS_BY 120  /* ms */
#define BILATERAL_COMBINATIONS_ALLOW_CROSSOVER_AFTER 80  /* ms */
#define BILATERAL_COMBINATIONS_ALLOW_SAMESIDED_AFTER 3000 /* ms */
#define BILATERAL_COMBINATIONS_TYPING_STREAK_TIMEOUT 160 /* ms */
#define BILATERAL_COMBINATIONS_TYPING_STREAK_MODMASK (~MOD_MASK_SHIFT)



/* Charybdis-specific features. */

// Configuration Syncing
#define CHARYBDIS_CONFIG_SYNC

#ifdef POINTING_DEVICE_ENABLE
// #define CHARYBDIS_MINIMUM_DEFAULT_DPI 1200
// #define CHARYBDIS_DEFAULT_DPI_CONFIG_STEP 400
// #define CHARYBDIS_MINIMUM_SNIPING_DPI 200
// #define CHARYBDIS_SNIPING_DPI_CONFIG_STEP 100
// // Enable pointer acceleration, which increases the speed by ~2x for large
// // displacement, while maintaining 1x speed for slow movements.
// // - `CHARYBDIS_POINTER_ACCELERATION_FACTOR`
#define CHARYBDIS_POINTER_ACCELERATION_ENABLE

// // Automatically enable the pointer layer when moving the trackball.  See also:
// // - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// // - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
#define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
// #undef ROTATIONAL_TRANSFORM_ANGLE
// #define ROTATIONAL_TRANSFORM_ANGLE 0

// // Double trackball build.
// #undef POINTING_DEVICE_RIGHT
// #define POINTING_DEVICE_LEFT

// #define POINTING_DEVICE_INVERT_Y
// #undef POINTING_DEVICE_INVERT_X
#endif // POINTING_DEVICE_ENABLE


/* RGB Matrix. */

#ifdef RGB_MATRIX_ENABLE
// Limit maximum brightness to keep power consumption reasonable, and avoid
// disconnects.
#    undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 64

// Rainbow swirl as startup mode.
#    define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#    define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_LEFT_RIGHT

// Slow swirl at startup.
#    define RGB_MATRIX_STARTUP_SPD 32

// Startup values.
#    define RGB_MATRIX_STARTUP_HUE 0
#    define RGB_MATRIX_STARTUP_SAT 255
#    define RGB_MATRIX_STARTUP_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
#    define RGB_MATRIX_STARTUP_HSV RGB_MATRIX_STARTUP_HUE, RGB_MATRIX_STARTUP_SAT, RGB_MATRIX_STARTUP_VAL
#endif  // RGB_MATRIX_ENABLE
