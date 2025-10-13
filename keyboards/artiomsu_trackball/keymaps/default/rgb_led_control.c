#include "sharedDefines.h"

bool rgb_show = true;
bool rgb_timed_out = false;
uint32_t rgb_time_out_value = 300000; // in milliseconds // 5 minutes by default
uint32_t rgb_timeout_counter = 0;

void keyboard_post_init_user(void) {
    pointing_device_set_cpi(400);
    scroll_divisor = scroll_divisor_default;
    scroll_invert = scroll_invert_default;
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(13, 250, 150);
    rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL + 1);
    rgblight_set_speed_noeeprom(200);
}

void suspend_power_down_user(void) {
    rgb_show = false;
}

void suspend_wakeup_init_user(void) {
    rgb_show = true;
}

void rgb_manage_timeout(bool timed_out){
    if(timed_out){
        if(rgblight_is_enabled()){
            rgblight_disable_noeeprom();
        }
    }else{
        if(!rgblight_is_enabled()){
            if(rgb_show){
                rgblight_enable_noeeprom();
            }
        }else{
            if(!rgb_show){
                rgblight_disable_noeeprom();
            }
        }
    }
}

void housekeeping_task_user(void) {
//void matrix_scan_user(void) {
  uint32_t current_timer_value = timer_read32();

  if(rgb_timeout_counter == 0){ // this is reset in macros
     rgb_timeout_counter = current_timer_value;
  }

  if(!rgb_timed_out){
      if(timer_elapsed32(rgb_timeout_counter) > rgb_time_out_value){
          rgb_timed_out = true;
          rgb_timeout_counter = current_timer_value;
      }
  }

  rgb_manage_timeout(rgb_timed_out);
}
