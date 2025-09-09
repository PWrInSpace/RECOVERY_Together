#include "servo_control.h"

static const char *TAG = "SERVO";

static servo_control_t servo = {
    .servo_pin = -1,
    .timer = NULL,
    .oper = NULL,
    .generator = NULL,
    .comparator = NULL,
    .angle = SERVO_CLOSED,
};

static esp_timer_handle_t servo_close_timer;

static void servo_close_callback(void* arg) {
    servo_close();
    ESP_LOGI(TAG, "Servo auto-closed after delay");
}


uint32_t angle_to_compare(int angle) {
    if (angle < SERVO_MIN_ANGLE) angle = SERVO_MIN_ANGLE;
    if (angle > SERVO_MAX_ANGLE) angle = SERVO_MAX_ANGLE;

    return (angle - SERVO_MIN_ANGLE) *
           (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) /
           (SERVO_MAX_ANGLE - SERVO_MIN_ANGLE) +
           SERVO_MIN_PULSEWIDTH_US;
}

void servo_init(void) {
    servo.servo_pin = SERVO;

    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_FREQUENCY_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    if (mcpwm_new_timer(&timer_config, &servo.timer) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create MCPWM timer");
        return;
    }

    mcpwm_operator_config_t operator_config = {
        .group_id = 0,
    };
    if (mcpwm_new_operator(&operator_config, &servo.oper) != ESP_OK ||
        mcpwm_operator_connect_timer(servo.oper, servo.timer) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create MCPWM operator");
        return;
    }

    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    if (mcpwm_new_comparator(servo.oper, &comparator_config, &servo.comparator) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create MCPWM comparator");
        return;
    }

    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = servo.servo_pin,
    };
    if (mcpwm_new_generator(servo.oper, &generator_config, &servo.generator) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create MCPWM generator");
        return;
    }

    mcpwm_comparator_set_compare_value(servo.comparator, angle_to_compare(SERVO_CLOSED));
    servo.angle = SERVO_CLOSED;

    mcpwm_generator_set_action_on_timer_event(
        servo.generator,
        MCPWM_GEN_TIMER_EVENT_ACTION(
            MCPWM_TIMER_DIRECTION_UP,
            MCPWM_TIMER_EVENT_EMPTY,
            MCPWM_GEN_ACTION_HIGH
        )
    );

    mcpwm_generator_set_action_on_compare_event(
        servo.generator,
        MCPWM_GEN_COMPARE_EVENT_ACTION(
            MCPWM_TIMER_DIRECTION_UP,
            servo.comparator,
            MCPWM_GEN_ACTION_LOW
        )
    );

    if (mcpwm_timer_enable(servo.timer) != ESP_OK ||
        mcpwm_timer_start_stop(servo.timer, MCPWM_TIMER_START_NO_STOP) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start MCPWM timer");
        return;
    }

    ESP_LOGI(TAG, "Servo initialized on GPIO %d", servo.servo_pin);

    const esp_timer_create_args_t timer_args = {
        .callback = &servo_close_callback,
        .name = "servo_close_timer"
    };
    esp_timer_create(&timer_args, &servo_close_timer);
}

void servo_set_angle(int angle) {
    if (mcpwm_comparator_set_compare_value(servo.comparator, angle_to_compare(angle)) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set servo angle");
        return;
    }
    servo.angle = angle;
    ESP_LOGI(TAG, "Servo moved to %d degrees", angle);
}

void servo_open(void) {
    servo_set_angle(SERVO_OPENED);
    ESP_LOGI(TAG, "Servo opened");
}

void servo_open_for(uint64_t duration_us) {
    servo_open();  // open normally
    esp_timer_start_once(servo_close_timer, duration_us);
    ESP_LOGI(TAG, "Servo will close automatically after %llu us", duration_us);
}

void servo_close(void) {
    servo_set_angle(SERVO_CLOSED);
    ESP_LOGI(TAG, "Servo closed");
}
