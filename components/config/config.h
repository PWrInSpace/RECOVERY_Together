#ifndef CONFIG_H
#define CONFIG_H

/************************ COMMANDS FROM MCB **********************************/

#define EASYMINI_ARM_CMD 0x01
#define EASYMINI_DISARM_CMD 0x02
#define TELEMETRUM_ARM_CMD 0x03
#define TELEMETRUM_DISARM_CMD 0x04
#define FORCE_FIRST_STAGE_CMD 0xa5
#define FORCE_SECOND_STAGE_CMD 0x5a

/**************************  RETURN STATUS  ***********************************/

#define I2C_SLAVE_PORT I2C_NUM_0
#define I2C_SLAVE_TX_BUF_LEN 256
#define I2C_SLAVE_RX_BUF_LEN 256
#define I2C_SLAVE_MAX_SPEED 40
#define MCB_ADR 0x00
#define DATA_SIZE_FROM_MCB 8
#define DATA_SIZE_TO_MCB 4

#define DEV_ADR 0x0B

#endif //  CONFIG_H