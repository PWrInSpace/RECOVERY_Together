// Copyright 2022 PWrInSpace, Kuba
#include "console_config.h"

#include "console.h"
#include "esp_log.h"
#include "servo_control.h"
#include "recovery_control.h"

#define TAG "CONSOLE_CONFIG"

int servo_open_cmd(int argc, char** argv){
    servo_open();
    return 0;
}

int servo_close_cmd(int argc, char** argv){
    servo_close();
    return 0;
}

int wire_on_cmd(int argc, char** argv){
    gpio_set_level(recovery_system.pilotDeployPin, 1);
    ESP_LOGI(TAG, "Resistance wire turned on");
    return 0;
}

int wire_off_cmd(int argc, char** argv){
    gpio_set_level(recovery_system.pilotDeployPin, 0);
    ESP_LOGI(TAG, "Resistance wire turned off");
    return 0;
}

int open_servo_angle(int argc, char** argv) {
    if (argc != 2) {
        return  -1;
    }

    int angle = atoi(argv[1]);
    ESP_LOGI("CLI", "OPEN TO %d", angle);
    servo_set_angle(angle);

    return 0;
}

int wire_open_time_cmd(int argc, char** argv) {
    if (argc != 2) {
        return -1;
    }

    int time = atoi(argv[1]);
    ESP_LOGI("CLI", "OPEN TIME %d", time);


    return 0;
}

int first_stage_deploy(int argc, char** argv) {
    return first_Stage_Deploy();
}

int second_stage_deploy(int argc, char** argv) {
    return second_Stage_Deploy();
}

static esp_console_cmd_t cmd[] = {
    {"servo-open", "Open servo", NULL, servo_open_cmd, NULL, NULL, NULL},
    {"servo-close", "Close servo", NULL, servo_close_cmd, NULL, NULL, NULL},
    {"wire-on", "Turn on resistance wire", NULL, wire_on_cmd, NULL, NULL, NULL},
    {"wire-off", "Turn off resistance wire", NULL, wire_off_cmd, NULL, NULL, NULL},
    {"servo-open-angle", "Open servo angle", NULL, open_servo_angle, NULL, NULL, NULL},
    // {"wire-open-time", "Open resistance wire for specified time (ms)", NULL, wire_open_time, NULL, NULL, NULL},
    {"first-stage", "Deploy first stage recovery", NULL, first_stage_deploy, NULL, NULL, NULL},
    {"second-stage", "Deploy second stage recovery", NULL, second_stage_deploy, NULL, NULL, NULL}
};

esp_err_t init_console() {
    esp_err_t ret;
    ret = console_init();
    ret = console_register_commands(cmd, sizeof(cmd) / sizeof(cmd[0]));
    // ESP_LOGW(TAG, "%s", esp_err_to_name(ret));
    return ret;
}