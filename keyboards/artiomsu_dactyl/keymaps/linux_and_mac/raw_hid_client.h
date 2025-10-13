#ifndef RAW_HID_CLIENT_DEFINES_H
#define RAW_HID_CLIENT_DEFINES_H

#include<string.h>
#include "usb_descriptor.h"
//#include "hid_display.h"
#include "raw_hid.h"
#include QMK_KEYBOARD_H


#define HID_PACKET_PAYLOAD_LEN 23
#define HID_PACKET_HEADER 0xfeed
struct hid_packet
{
	uint16_t header; // 2 bytes now
	uint16_t to_vid; // 4 bytes now
	uint16_t to_pid; // 6 bytes now
	uint8_t operation; // 7 bytes now
	uint8_t payload_length; // 8 bytes now
	uint8_t payload[HID_PACKET_PAYLOAD_LEN];
};

enum operation {
    HID_RAW_OP_SIMPLE_KEY = 0x01, // just for test
    HID_RAW_SET_SETTING = 0x02, // setting settings on other devices
    HID_RAW_CUSTOM_KEY = 0x03, // almost like keypress
    HID_RAW_OP_INFO = 0xee, // mostly for sending info to pc
    HID_RAW_OP_SIMPLE_KEY_2 = 0xff // just for test
};

enum trackball_settings {
    HID_RAW_TB_S_DPI = 0x01,
    HID_RAW_TB_S_SCROLL = 0x02,
    HID_RAW_LED = 0x03,
};

enum trackball_custom_key {
    HID_TB_DRAG_SCROLL = 0x01,
    HID_TB_D_S_1 = 0x02,
    HID_QK_BOOT = 0x03,
    HID_QK_COMPILE = 0x04,
};

enum trackball_settings_led_timeout {
    HID_TB_LED_TIMEOUT_30S = 0x01,
    HID_TB_LED_TIMEOUT_1M = 0x02,
    HID_TB_LED_TIMEOUT_5M = 0x03,
    HID_TB_LED_TIMEOUT_10M = 0x04,
    HID_TB_LED_TIMEOUT_OFF = 0x05,
    HID_TB_LED_TOGGLE = 0x06,
    HID_TB_LED_BRIGHTNESS_UP = 0x07,
    HID_TB_LED_BRIGHTNESS_DOWN = 0x08,
    HID_TB_LED_HUE_UP = 0x09,
    HID_TB_LED_HUE_DOWN = 0x0a,
    HID_TB_LED_SAT_UP = 0x0b,
    HID_TB_LED_SAT_DOWN = 0x0c,
    HID_TB_LED_MODE_UP = 0x0d,
    HID_TB_LED_MODE_DOWN = 0x0e,
    HID_TB_LED_MODE_SNAKE = 0x0f,
    HID_TB_LED_MODE_KNIGHT = 0x10,
    HID_TB_LED_MODE_TWINKLE = 0x11
};

struct raw_hid_client
{
	uint16_t vid;
	uint16_t pid;
};

extern struct raw_hid_client trackball;
extern struct raw_hid_client pc;
extern struct raw_hid_client broadcast;

void simple_hid_test_send_key(struct raw_hid_client *client, uint8_t key);
void raw_hid_set_setting(struct raw_hid_client *client, uint8_t setting, uint8_t data);
void raw_hid_custom_key(struct raw_hid_client *client, uint8_t key, bool pressed);
void raw_hid_send_info(struct raw_hid_client *client, char* info, uint8_t length);
#endif
