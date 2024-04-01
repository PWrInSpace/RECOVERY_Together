#ifndef CONFIG_H
#define CONFIG_H

/***************************  ESP DEFINE  ************************************/

//#define ESP_LEFT
#define ESP_RIGHT

/************************ COMMANDS FROM MCB **********************************/

#define COTS_ARM 0x00
#define COTS_DISARM 0x01
#define PILOT_DEPLOY_CMD 0xAA
#define MAIN_DEPLOY 0xBB

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



#endif //  CONFIG_H