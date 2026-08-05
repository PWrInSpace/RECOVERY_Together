#include "app.h"

void read_data() {
}

void execute_cmd(uint32_t data) {
    //
    // switch (data)
    // {
    // case EASYMINI_ARM_CMD:
    //
    //     cots_arming(COTS_DEVICE_2);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case EASYMINI_DISARM_CMD:
    //
    //     cots_disarm(COTS_DEVICE_2);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case TELEMETRUM_ARM_CMD:
    //
    //     cots_arming(COTS_DEVICE_1);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case TELEMETRUM_DISARM_CMD:
    //
    //     cots_disarm(COTS_DEVICE_1);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case FORCE_FIRST_STAGE_CMD:
    //
    //     first_Stage_Deploy(true);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // case FORCE_SECOND_STAGE_CMD:
    //
    //     second_Stage_Deploy(true);
    //     rx_buffer.cmd.command = 0x00;
    //     break;
    //
    // default:
    //     break;
    // }
}

void app_main(void) {
    app_init();
    // if(I2C_slave_init() != RET_SUCCESS){
    //     ESP_LOGE(TAG,"I2C slave init failed, restarting ...");
    //     esp_restart();
    //  }
    // /************************ INTERRUPT HANDLERS *****************************/
    // gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    // gpio_isr_handler_add(TELE_APOGEE_CHECK, tele_apogee_isr_handler, NULL);
    // gpio_isr_handler_add(EASY_APOGEE_CHECK, easy_apogee_isr_handler, NULL);
    // gpio_isr_handler_add(TELE_IGNITER_FIRE, tele_main_isr_handler, NULL);
    //
    // ESP_LOGI(TAG,"Init completed, entering recovery loop");
    // gpio_set_level(LED,1);
    //
    //
    // read_data();
    //
    // while (true) {
    //     I2C_buffer_read();
    //     execute_cmd(rx_buffer.cmd.command);
    //     read_data();
    //     check_Cont();
    //     I2C_buffer_write();
    //     vTaskDelay(pdMS_TO_TICKS(100));
    //     log_debug_data();
    // }
}