#ifndef CONFIG_H
#define CONFIG_H

/***************************  ESP DEFINE  ************************************/

// #define ESP_LEFT
 #define ESP_RIGHT

/************************ COMMANDS FROM MCB **********************************/

#define PILOT_DEPLOY_CMD 0x01
#define MAIN_DEPLOY_CMD 0x02
#define TELEMETRUM_ARM_CMD 0x03
#define TELEMETRUM_DISARM_CMD 0x04
#define EASYMINI_ARM_CMD 0x05
#define EASYMINI_DISARM_CMD 0x06

/**************************  ARMING STATUS  ***********************************/

#define DISARMED 0x00
#define ARMED 0x01

/***************************  COTS STATES  ************************************/

#define OTHER_STATE 0x00
#define APOGEE 0xAA
#define MAIN_ALTITUDE 0xBB 

/**************************  RETURN STATUS  ***********************************/

#define RET_SUCCESS 0x00
#define RET_FAILTURE 0x01

#ifdef ESP_LEFT
#define DEV_ADR 0x0B
#endif

#ifdef ESP_RIGHT
#define DEV_ADR 0x0A
#endif

#define I2C_SLAVE_PORT I2C_NUM_0
#define I2C_SLAVE_TX_BUF_LEN 256
#define I2C_SLAVE_RX_BUF_LEN 256
#define I2C_SLAVE_MAX_SPEED 40
#define MCB_ADR 0x00
#define DATA_SIZE_FROM_MCB 8
#define DATA_SIZE_TO_MCB 4


#endif //  CONFIG_H