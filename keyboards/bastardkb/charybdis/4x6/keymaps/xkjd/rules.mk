VIA_ENABLE=yes

VIAL_ENABLE = yes

VIALRGB_ENABLE = yes

MOUSEKEY_ENABLE = yes

AUDIO_ENABLE = no          # Audio output

SPLIT_KEYBOARD = yes       # Split common

SRC += features/select_word.c
SRC += features/custom_shift_keys.c

# https://docs.qmk.fm/#/feature_caps_word?id=caps-word
CAPS_WORD_ENABLE = yes

# https://docs.qmk.fm/#/custom_quantum_functions?id=deferred-execution
DEFERRED_EXEC_ENABLE = yes

# https://github.com/qmk/qmk_firmware/issues/19593#issuecomment-1387476045
NO_USB_STARTUP_CHECK = yes
