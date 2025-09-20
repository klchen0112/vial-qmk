// Copyright 2023 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* stront specific options */
#define STRONT_DISPLAY_LEFT // display is on left by default
#define STRONT_DISPLAY_ROTATION QP_ROTATION_180
#ifdef QUANTUM_PAINTER_ST7789_SPI_ENABLE // default settings for ST7789 1.69" display
#    define STRONT_DISPLAY_WIDTH 240
#    define STRONT_DISPLAY_HEIGHT 280
#    define STRONT_DISPLAY_OFFSET_X 0
#    define STRONT_DISPLAY_OFFSET_Y 20
#endif

/* interconnect config */
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_PIN_SWAP
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1

/* reset config */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U
// 增加晶振启动延迟
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 64

/* any side can be master by default, enable split sync to support it */
#define SPLIT_HAND_MATRIX_GRID GP18, GP8   // 行在前，因为是行到列的二极管方向
#define SPLIT_HAND_MATRIX_GRID_LOW_IS_LEFT // 焊接跳线就是左边，被拉低了。
#define MATRIX_MASKED                      // 打开矩阵蒙版
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_POINTING_ENABLE
#define SPLIT_WPM_ENABLE
#define SPLIT_HAPTIC_ENABLE
#define SPLIT_ACTIVITY_ENABLE

/* VBUS-routed pin for upstream detection */
// #define USB_VBUS_PIN GP19
#define SPLIT_USB_DETECT
#define SPLIT_USB_TIMEOUT 2000
#define SPLIT_USB_TIMEOUT_POLL 10
#define SPLIT_WATCHDOG_ENABLE

/* common SPI/I2C config */
#define SPI_DRIVER SPID0
#define SPI_SCK_PIN GP22
#define SPI_MOSI_PIN GP23
#define SPI_MISO_PIN GP20
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP7
#define I2C1_SCL_PIN GP6

/* common touchpad config */
// #define POINTING_DEVICE_RIGHT // touchpad is right by default
// #define POINTING_DEVICE_CS_PIN GP21
// #ifdef POINTING_DEVICE_DRIVER_azoteq_iqs5xx
// #    define AZOTEQ_IQS5XX_TPS43
// #    define AZOTEQ_IQS5XX_ROTATION_270
// #else
// #    define CIRQUE_PINNACLE_DIAMETER_MM 40
// // #    define CIRQUE_PINNACLE_ATTENUATION EXTREG__TRACK_ADCCONFIG__ADC_ATTENUATE_2X // enable for curved overlay
// #    ifdef POINTING_DEVICE_RIGHT
// #        define POINTING_DEVICE_ROTATION_180
// #    endif
// #endif

/* common LCD config */
#define LCD_CS_PIN GP21
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_B
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 180000

// 音效蜂鸣器配置
#ifdef AUDIO_ENABLE
#    define AUDIO_VOICES
#    define AUDIO_PIN GP2
#    define AUDIO_PWM_DRIVER PWMD1
#    define AUDIO_PWM_CHANNEL RP2040_PWM_CHANNEL_A
#    define AUDIO_CLICKY
#    define AUDIO_CLICKY_FREQ_DEFAULT 1500.0f
#    define AUDIO_CLICKY_FREQ_MIN 61.0f
#    define AUDIO_CLICKY_FREQ_MAX 1500.0f
#    define AUDIO_CLICKY_FREQ_FACTOR 1.18921f
#    define AUDIO_CLICKY_FREQ_RANDOMNESS 1.0f
#    define AUDIO_CLICKY_DELAY_DURATION 6
#    define AUDIO_INIT_DELAY 2
#    define STARTUP_SONG SONG(ROCK_A_BYE_BABY)
#    define GOODBYE_SONG SONG(CAMPANELLA)
#    define MUSIC_ON_SONG SONG(ZELDA_PUZZLE)
#    define AUDIO_VOICES
#    define DEFAULT_LAYER_SONGS {SONG(QWERTY_SOUND), SONG(COLEMAK_SOUND), SONG(DVORAK_SOUND)}
#endif

// 震动配置
#ifdef HAPTIC_ENABLE
#    define HAPTIC_OFF_IN_LOW_POWER 0
#    define NO_HAPTIC_MOD
#    define NO_HAPTIC_FN
#    define NO_HAPTIC_ALPHA
#    define NO_HAPTIC_PUNCTUATION
#    define NO_HAPTIC_NAV
#    define NO_HAPTIC_NUMERIC
#    define DRV2605L_GREETING 7
#    define DRV2605L_DEFAULT_MODE DRV2605L_EFFECT_SHARP_TICK_1_100
// DRV2605L 线性马达设置 LRA
#    define FB_ERM_LRA 1
#    define FB_BRAKEFACTOR 6 /* For 1x:0, 2x:1, 3x:2, 4x:3, 6x:4, 8x:5, 16x:6, Disable Braking:7 */
#    define FB_LOOPGAIN 0    /* For  Low:0, Medium:1, High:2, Very High:3 */
#    define RATED_VOLTAGE 5
#    define F_LRA 220 /* resonance freq */
#endif

#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 100

#define DEBOUNCE 10

// 定义刷机按钮位置
#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 0
#define BOOTMAGIC_LITE_ROW_RIGHT 6
#define BOOTMAGIC_LITE_COLUMN_RIGHT 0

// 定义wpm周期
#define WPM_SAMPLE_SECONDS 5
#define WPM_SAMPLE_PERIODS 25
