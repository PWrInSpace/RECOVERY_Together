#include "servo_control.h"

static const char* TAG = "SERVO";

static esp_timer_handle_t servo_close_timer;

static void servo_close_callback(void* arg) {
    servo_close(arg);
    ESP_LOGI(TAG, "Servo auto-closed after delay");
}

uint32_t angle_to_compare(const servo_control_t *servo, int angle) {
    if (angle < servo->config.min_angle) angle = servo->config.min_angle;
    if (angle > servo->config.max_angle) angle = servo->config.max_angle;

    return (angle - servo->config.min_angle) *
           (servo->config.max_pulse_width_us - servo->config.min_pulse_width_us) /
           (servo->config.max_angle - servo->config.min_angle) +
           servo->config.min_pulse_width_us;
}

esp_err_t servo_init(const servo_config_t *config, servo_control_t *servo) {
    *servo = (servo_control_t){
        .config = *config,
        .timer = NULL,
        .oper = NULL,
        .generator = NULL,
        .comparator = NULL,
        .angle = 0,
    };

    const mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = servo->config.frequency_hz,
        .period_ticks = servo->config.timebase_period,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };

    if (mcpwm_new_timer(&timer_config, &servo->timer) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create MCPWM timer");
        return ESP_FAIL;
    }

    const mcpwm_operator_config_t operator_config = {
        .group_id = 0,
    };

    if (mcpwm_new_operator(&operator_config, &servo->oper) != ESP_OK || mcpwm_operator_connect_timer(servo->oper, servo->timer) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create MCPWM operator");
        return ESP_FAIL;
    }

    const mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };

    if (mcpwm_new_comparator(servo->oper, &comparator_config, &servo->comparator) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create MCPWM comparator");
        return ESP_FAIL;
    }

    const mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = servo->config.servo_pin,
    };

    if (mcpwm_new_generator(servo->oper, &generator_config, &servo->generator) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create MCPWM generator");
        return ESP_FAIL;
    }

    mcpwm_comparator_set_compare_value(servo->comparator, angle_to_compare(servo, servo->config.close_angle));
    servo->angle = servo->config.close_angle;

    mcpwm_generator_set_action_on_timer_event(
        servo->generator,
        MCPWM_GEN_TIMER_EVENT_ACTION(
            MCPWM_TIMER_DIRECTION_UP,
            MCPWM_TIMER_EVENT_EMPTY,
            MCPWM_GEN_ACTION_HIGH
        )
    );

    mcpwm_generator_set_action_on_compare_event(
        servo->generator,
        MCPWM_GEN_COMPARE_EVENT_ACTION(
            MCPWM_TIMER_DIRECTION_UP,
            servo->comparator,
            MCPWM_GEN_ACTION_LOW
        )
    );

    if (mcpwm_timer_enable(servo->timer) != ESP_OK ||
        mcpwm_timer_start_stop(servo->timer, MCPWM_TIMER_START_NO_STOP) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start MCPWM timer");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Servo initialized on GPIO %d", servo->config.servo_pin);

    const esp_timer_create_args_t timer_args = {
        .callback = &servo_close_callback,
        .name = "servo_close_timer",
        .arg = servo,
    };

    if (esp_timer_create(&timer_args, &servo_close_timer) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create servo timer");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t servo_set_angle(servo_control_t *servo, int angle) {
    if (mcpwm_comparator_set_compare_value(servo->comparator, angle_to_compare(servo, angle)) != ESP_OK) {
        return ESP_FAIL;
    }
    servo->angle = angle;
    return ESP_OK;
}

esp_err_t servo_open(servo_control_t *servo) {
    if (servo_set_angle(servo, servo->config.open_angle) != ESP_OK) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t servo_open_for(servo_control_t *servo, uint64_t duration_us) {
    servo_open(servo);  // open normally
    esp_timer_start_once(servo_close_timer, duration_us);
    ESP_LOGI(TAG, "Servo will close automatically after %llu us", duration_us);

    return ESP_OK;
}

esp_err_t servo_close(servo_control_t *servo) {
    if (servo_set_angle(servo, servo->config.close_angle) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to close servo");
        return ESP_FAIL;
    }
    return ESP_OK;
}
