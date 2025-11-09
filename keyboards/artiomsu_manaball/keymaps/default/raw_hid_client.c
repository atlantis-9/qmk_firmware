#include "raw_hid_client.h"
#include "sharedDefines.h"
                                    //vid, pid
//struct raw_hid_client trackball = {0x9000, 0x9001};

struct raw_hid_client pc = {0x0000, 0x0000};
struct raw_hid_client manaball = {0x4A4A, 0xBA11};
struct raw_hid_client dactyl = {0xa340, 0xf00d};
struct raw_hid_client broadcast = {0xffff, 0xffff};

bool is_packet_for_broadcast(struct hid_packet *packet){
    if(packet->to_vid == 0xffff && packet->to_pid == 0xffff){
        return true;
    }
    return false;
}

bool is_packet_for_manaball(struct hid_packet *packet){
    // this is more like what devices are allowed to send to manaball
    if(packet->to_vid == dactyl.vid && packet->to_pid == dactyl.pid){
        return true;
    }
    return false;
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    if(length != RAW_EPSIZE){
        return;
    }
    struct hid_packet *packet = (struct hid_packet *)data;
    char info[HID_PACKET_PAYLOAD_LEN];

    if(packet->header != HID_PACKET_HEADER){
        char c[8];
        SEND_STRING("header is bad for manaball ");
        snprintf(c, sizeof(c), "%X ", packet->header);
        SEND_STRING(c);

        for (int i = 0; i < length; i++){
            snprintf(c, sizeof(c), "%02X", data[i]);
            SEND_STRING(c);
        }
        return;
    }


    // debug if packet is not for manaball
    // if(!is_packet_for_manaball(packet)){
    //     snprintf(info, sizeof(info), "v: %04X p: %04X ", packet->to_vid, packet->to_pid);
    //     raw_hid_send_info(&pc, info, sizeof(info));
    // }

    // check if the packet is either for us or is a broadcast
    if(!is_packet_for_manaball(packet) && !is_packet_for_broadcast(packet)){
        return;
    }

    switch (packet->operation){
        case HID_RAW_OP_SIMPLE_KEY_2:
            if(packet->to_pid == 0xffff && packet->to_vid == 0xffff){
                snprintf(info, sizeof(info), "received broadcast");
                raw_hid_send_info(&pc, info, sizeof(info));
            }else{
                snprintf(info, sizeof(info), "received data");
                raw_hid_send_info(&pc, info, sizeof(info));
            }
        break;
        case HID_RAW_CUSTOM_KEY:
            if(!is_packet_for_manaball(packet)){
                break;
            }
            switch (packet->payload[0])
            {
                case HID_TB_DRAG_SCROLL:
                    set_scrolling = packet->payload[1];
                    set_scrolling_horizontal = false;
                break;
                case HID_TB_D_S_1:
                    set_scrolling_horizontal = packet->payload[1];
                    set_scrolling = false;
                break;
                case HID_QK_BOOT:
                    rgblight_enable_noeeprom();
                    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
                    rgblight_sethsv_noeeprom(0, 250, 100);
                    reset_keyboard();
                break;
                case HID_QK_COMPILE:
                    SEND_STRING("cd /temp/GIT/qmk_firmware && make clean && qmk flash -kb artiomsu_manaball -km default");
                break;
                default:
                break;
            }
        break;
        case HID_RAW_SET_SETTING:
            switch (packet->payload[0])
            {
                case HID_RAW_TB_S_DPI:
                    if(!is_packet_for_manaball(packet)){
                        break;
                    }
                    switch (packet->payload[1])
                    {
                        case 0x01:
                            manageDPI(true);
                            snprintf(info, sizeof(info), "DPI: %d", custom_dpi);
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case 0x02:
                            manageDPI(false);
                            snprintf(info, sizeof(info), "DPI: %d", custom_dpi);
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case 0x03:
                            pointing_device_set_cpi(400);
                            custom_dpi=400;
                            snprintf(info, sizeof(info), "DPI: %d", custom_dpi);
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        default:
                        break;
                    }
                break;
                case HID_RAW_TB_S_SCROLL:
                    if(!is_packet_for_manaball(packet)){
                        break;
                    }
                    switch (packet->payload[1])
                    {
                        case 0x01:
                            manageScroll(true);
                            snprintf(info, sizeof(info), "SCROLL SPEED: %d", (int)(scroll_divisor));
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case 0x02:
                            manageScroll(false);
                            snprintf(info, sizeof(info), "SCROLL SPEED: %d", (int)(scroll_divisor));
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case 0x03:
                            scroll_divisor = scroll_divisor_default;
                            snprintf(info, sizeof(info), "SCROLL SPEED: %d", (int)(scroll_divisor));
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case 0x04:
                            scroll_invert = !scroll_invert;
                            snprintf(info, sizeof(info), "SCROLL INVERT: %s", scroll_invert ? "ON" : "OFF");
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        default:
                        break;
                    }
                break;
                case HID_RAW_LED:
                    switch (packet->payload[1])
                    {
                        case HID_TB_LED_TIMEOUT_30S:
                            rgb_time_out_value = 30000;
                            snprintf(info, sizeof(info), "LED TIMEOUT: 30s");
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case HID_TB_LED_TIMEOUT_1M:
                            rgb_time_out_value = 60000;
                            snprintf(info, sizeof(info), "LED TIMEOUT: 1m");
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case HID_TB_LED_TIMEOUT_5M:
                            rgb_time_out_value = 300000;
                            snprintf(info, sizeof(info), "LED TIMEOUT: 5m");
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case HID_TB_LED_TIMEOUT_10M:
                            rgb_time_out_value = 600000;
                            snprintf(info, sizeof(info), "LED TIMEOUT: 10m");
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case HID_TB_LED_TIMEOUT_OFF:
                            rgb_timed_out = true;
                            rgblight_disable_noeeprom();
                            snprintf(info, sizeof(info), "LED TIMEOUT: OFF");
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case HID_TB_LED_TOGGLE:
                            rgb_show = !rgb_show;
                            if(rgb_show){
                                snprintf(info, sizeof(info), "LEDs: ON");
                            }else{
                                snprintf(info, sizeof(info), "LEDs: OFF");
                            }
                            raw_hid_send_info(&pc, info, sizeof(info));
                        break;
                        case HID_TB_LED_BRIGHTNESS_UP:
                            rgblight_increase_val_noeeprom();
                        break;
                        case HID_TB_LED_BRIGHTNESS_DOWN:
                            rgblight_decrease_val_noeeprom();
                        break;
                        case HID_TB_LED_HUE_UP:
                            rgblight_increase_hue_noeeprom();
                        break;
                        case HID_TB_LED_HUE_DOWN:
                            rgblight_decrease_hue_noeeprom();
                        break;
                        case HID_TB_LED_SAT_UP:
                            rgblight_increase_sat_noeeprom();
                        break;
                        case HID_TB_LED_SAT_DOWN:
                            rgblight_decrease_sat_noeeprom();
                        break;
                        case HID_TB_LED_MODE_UP:
                            rgblight_step_noeeprom();
                        break;
                        case HID_TB_LED_MODE_DOWN:
                            rgblight_step_reverse_noeeprom();
                        break;
                        case HID_TB_LED_MODE_SNAKE:
                            rgblight_mode_noeeprom(RGBLIGHT_MODE_SNAKE + 1);
                        break;
                        case HID_TB_LED_MODE_KNIGHT:
                            rgblight_mode_noeeprom(RGBLIGHT_MODE_RAINBOW_SWIRL + 1);
                        break;
                        case HID_TB_LED_MODE_TWINKLE:
                            rgblight_mode_noeeprom(RGBLIGHT_MODE_TWINKLE + 3);
                        break;
                        default:
                        break;
                    }
                default:
                break;
            }
        default:
        break;
    }
}

void raw_hid_send_to_router(struct raw_hid_client *client,
    uint8_t operation,
    uint8_t *payload,
    uint8_t payload_len
){
    if(client == NULL){
        return;
    }

    if(payload_len > HID_PACKET_PAYLOAD_LEN){
        return;
    }

    struct hid_packet pack;

    pack.header = HID_PACKET_HEADER;
    pack.to_vid = client->vid;
    pack.to_pid = client->pid;
    pack.operation = operation;
    pack.payload_length = payload_len;
    memset(pack.payload, 0, sizeof(pack.payload));
    memcpy(pack.payload, payload, payload_len);

    raw_hid_send((uint8_t *)&pack, RAW_EPSIZE);
}

void simple_hid_test_send_key(struct raw_hid_client *client, uint8_t key){
    if(client == NULL){
        return;
    }
    uint8_t test[4] = {key, 0x02, 0x03, 0xff};
    raw_hid_send_to_router(
        client,
        HID_RAW_OP_SIMPLE_KEY_2,
        test,
        4
    );
}

void raw_hid_set_setting(struct raw_hid_client *client, uint8_t setting, uint8_t data){
    if(client == NULL){
        return;
    }
    uint8_t s[2] = {setting, data};
    raw_hid_send_to_router(
        client,
        HID_RAW_SET_SETTING,
        s,
        2
    );
}

void raw_hid_custom_key(struct raw_hid_client *client, uint8_t key, bool pressed){
    if(client == NULL){
        return;
    }
    uint8_t data[2] = {key, pressed};
    raw_hid_send_to_router(
        client,
        HID_RAW_CUSTOM_KEY,
        data,
        2
    );
}

void raw_hid_send_info(struct raw_hid_client *client, char* info, uint8_t length){
    if(client == NULL){
        return;
    }
    raw_hid_send_to_router(
        client,
        HID_RAW_OP_INFO,
        (uint8_t *)info,
        length
    );
}
