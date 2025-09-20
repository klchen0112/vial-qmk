
# choose pointing device driver:
# POINTING_DEVICE_DRIVER = cirque_pinnacle_spi
# POINTING_DEVICE_DRIVER = cirque_pinnacle_i2c
POINTING_DEVICE_DRIVER = azoteq_iqs5xx

QUANTUM_PAINTER_LVGL_INTEGRATION = yes
# choose one display driver:
# QUANTUM_PAINTER_DRIVERS = st7789_spi # rectangular display
QUANTUM_PAINTER_DRIVERS = gc9a01_spi # round display

SRC += lvgl_helpers.c \
	   display.c

DEFAULT_FOLDER = motev3/56keys

# 打开音频功能
AUDIO_ENABLE = yes
AUDIO_DRIVER = pwm_hardware

# 打开震动功能
HAPTIC_ENABLE = yes
HAPTIC_DRIVER = drv2605l

# 关闭vial的安全模式
VIAL_INSECURE = yes
# 打开鼠标键
MOUSEKEY_ENABLE = yes
WPM_ENABLE = yes
# 打开signalrgb支持
SIGNALRGB_SUPPORT_ENABLE = yes




