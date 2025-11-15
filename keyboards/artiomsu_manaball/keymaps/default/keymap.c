#include "sharedDefines.h"
// make clean && qmk flash -kb artiomsu_manaball -km default


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [Layer_main] = LAYOUT_1x5(
        MS_BTN1, MS_BTN2, MS_BTN3, ST_MACRO_SCROLL, ST_QK_BOOT
    ),
    [Layer_secondary] = LAYOUT_1x5(
        _______, MS_BTN3, _______, _______, _______
    ),
    [Layer_raw_hid_router] = LAYOUT_1x5(
        MS_BTN4, MS_BTN5, _______, _______, _______
    ),
};

