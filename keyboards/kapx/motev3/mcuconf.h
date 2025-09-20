// Copyright 2023 zzeneg (@zzeneg)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include_next <mcuconf.h>

// 打开方波给音频用
#undef RP_PWM_USE_PWM1
#define RP_PWM_USE_PWM1 TRUE
// 打开方波给lcd背光用
#undef RP_PWM_USE_PWM5
#define RP_PWM_USE_PWM5 TRUE

// 打开i2c给震动模块用
#undef RP_I2C_USE_I2C1
#define RP_I2C_USE_I2C1 TRUE
// 打开spi给lcd和触控板用
#undef RP_SPI_USE_SPI0
#define RP_SPI_USE_SPI0 TRUE
