// Copyright 2023 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// pmw3389配置选项
#define PMW33XX_CS_PIN POINTING_DEVICE_CS_PIN
#define PMW33XX_CPI 400
#define POINTING_DEVICE_INVERT_Y
#define PMW33XX_LIFTOFF_DISTANCE 0x01
#define ROTATIONAL_TRANSFORM_ANGLE 0
// 设备除数，越小越快
#define PMW33XX_SPI_DIVISOR 16

// 点设备的任务更新时间
#define POINTING_DEVICE_TASK_THROTTLE_MS 15

#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
