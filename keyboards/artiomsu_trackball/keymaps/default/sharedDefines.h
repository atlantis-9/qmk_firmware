#ifndef SHAREDDEFINES_H
#define SHAREDDEFINES_H

#include QMK_KEYBOARD_H

enum custom_keycodes {
  ST_MACRO_SCROLL = SAFE_RANGE, // SAFE_RANGE sets the first one in the list to a value that won't interfere with anything
  ST_QK_BOOT,
};

extern uint16_t custom_dpi;
extern const uint16_t step_size;
extern const uint16_t minimum_dpi;
extern const uint16_t max_dpi;

extern bool set_scrolling;
extern bool set_scrolling_macro;
extern bool set_scrolling_horizontal;
extern const float scroll_divisor_default;
extern float scroll_divisor;
extern const float scroll_divisor_min;
extern const float scroll_divisor_max;

extern float scroll_accumulated_h;
extern float scroll_accumulated_v;

extern const bool scroll_invert_default;
extern bool scroll_invert;

extern bool rgb_show;
extern bool rgb_timed_out;
extern uint32_t rgb_timeout_counter;
extern uint32_t rgb_time_out_value;  // in milliseconds

void manageDPI(bool up);
void manageScroll(bool up);

#endif
