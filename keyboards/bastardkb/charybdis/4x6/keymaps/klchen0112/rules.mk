VIA_ENABLE=yes

VIAL_ENABLE = yes

VIALRGB_ENABLE = yes

MOUSEKEY_ENABLE = yes

AUDIO_ENABLE = no          # Audio output

SPLIT_KEYBOARD = yes       # Split common

SRC += features/select_word.c

# https://docs.qmk.fm/#/feature_caps_word?id=caps-word
CAPS_WORD_ENABLE = yes

# https://docs.qmk.fm/#/custom_quantum_functions?id=deferred-execution
DEFERRED_EXEC_ENABLE = yes

TAP_DANCE_ENABLE = no
COMBO_ENABLE = no
QMK_SETTINGS = no
SPACE_CADET_ENABLE = no
KEY_OVERRIDE_ENABLE = no
