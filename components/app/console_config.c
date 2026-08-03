// Copyright 2022 PWrInSpace, Kuba

#include "console_config.h"

static const char* TAG = "CONSOLE";

static int servo_open_cmd(int argc, char** argv){
    servo_open(&servo);
    return 0;
}

static int servo_close_cmd(int argc, char** argv){
    servo_close(&servo);
    return 0;
}

static int wire_on_cmd(int argc, char** argv){
    gpio_set_level(recovery.config.first_stage_pin, 1);
    ESP_LOGI(TAG, "Resistance wire turned on");
    return 0;
}

static int wire_off_cmd(int argc, char** argv){
    gpio_set_level(recovery.config.first_stage_pin, 0);
    ESP_LOGI(TAG, "Resistance wire turned off");
    return 0;
}

static int open_servo_angle(int argc, char** argv) {
    if (argc != 2) {
        return  -1;
    }

    const int angle = strtol(argv[1], NULL, 10);
    ESP_LOGI("CLI", "OPEN TO %d", angle);
    servo_set_angle(&servo, angle);
    return 0;
}

static int wire_open_time_cmd(int argc, char** argv) {
    if (argc != 2) {
        return -1;
    }

    const int time = strtol(argv[1], NULL, 10);
    ESP_LOGI("CLI", "OPEN TIME %d", time);
    resistance_wire_on_time(time);
    return 0;
}

static int first_stage(int argc, char** argv) {
    recovery.config.first_stage();
    return 0;
}

static int second_stage(int argc, char** argv) {
    recovery.config.second_stage();
    return 0;
}

static int telemetrum_arm_cmd(int argc, char** argv) {
    cots_arm(&telemetrum);
    return 0;
}

static int telemetrum_disarm_cmd(int argc, char** argv) {
    cots_disarm(&telemetrum);
    return 0;
}

static int easymini_arm_cmd(int argc, char** argv) {
    cots_arm(&easymini);
    return 0;
}

static int easymini_disarm_cmd(int argc, char** argv) {
    cots_disarm(&easymini);
    return 0;
}

static esp_console_cmd_t cmd[] = {
    {"servo-open", "Open servo", NULL, servo_open_cmd, NULL, NULL, NULL},
    {"servo-close", "Close servo", NULL, servo_close_cmd, NULL, NULL, NULL},
    {"wire-on", "Turn on resistance wire", NULL, wire_on_cmd, NULL, NULL, NULL},
    {"wire-off", "Turn off resistance wire", NULL, wire_off_cmd, NULL, NULL, NULL},
    {"wire-open-time", "Open resistance wire for specified time", NULL, wire_open_time_cmd, NULL, NULL, NULL},
    {"servo-open-angle", "Open servo angle", NULL, open_servo_angle, NULL, NULL, NULL},
    {"first-stage", "Deploy first stage recovery", NULL, first_stage, NULL, NULL, NULL},
    {"second-stage", "Deploy second stage recovery", NULL, second_stage, NULL, NULL, NULL},
    {"telemetrum-arm", "Arm Telemetrum", NULL, telemetrum_arm_cmd, NULL, NULL, NULL},
    {"telemetrum-disarm", "Disarm Telemetrum", NULL, telemetrum_disarm_cmd, NULL, NULL, NULL},
    {"easymini-arm", "Arm EasyMini", NULL, easymini_arm_cmd, NULL, NULL, NULL},
    {"easymini-disarm", "Disarm EasyMini", NULL, easymini_disarm_cmd, NULL, NULL, NULL},
};

esp_err_t init_console() {
    if (console_register_commands(cmd, sizeof(cmd) / sizeof(cmd[0])) != ESP_OK) {
        return ESP_FAIL;
    }

    return ESP_OK;
}