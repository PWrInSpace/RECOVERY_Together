#ifndef SERVO_CONFIG_H
#define SERVO_CONFIG_H

#include "servo_control.h"

extern servo_control_t servo;

esp_err_t init_servo(void);

#endif
