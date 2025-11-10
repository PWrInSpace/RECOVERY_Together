#ifndef PINOUT_H
#define PINOUT_H


/******************* PINOUT FOR LEFT ESP WROOM 32D  ********************/

#ifdef ESP_LEFT

#define END_CONE_1 34U
#define END_CONE_2 33U
#define TELE_APOGEE_CHECK 26U 
#define EASY_APOGEE_CHECK 32U
#define SERVO 18U
#define PILOT_DEPLOY 17U
#define EASY_IGNITER_FIRE 35U 
#define TELE_IGNITER_FIRE 25U 
#define EASY_ARMING 21U
#define TELE_ARMING 16U
#define I2C_SCL 27U
#define I2C_SDA 14U
#define LED 19U 

#endif // ESP_LEFT

/******************* PINOUT FOR RIGHT ESP WROOM 32D  *******************/

#ifdef ESP_RIGHT

#define END_CONE 21U
#define TELE_APOGEE_CHECK 19U
#define EASY_APOGEE_CHECK 16U
#define TELE_IGNITER_CONT 25U
#define EASY_IGNITER_CONT 17U
#define PRESSURE 4U
#define PILOT_DEPLOY 25U //TODO: change to real value  
#define EASY_IGNITER_FIRE 33U 
#define TELE_IGNITER_FIRE 26U 
#define EASY_ARMING 32U
#define TELE_ARMING  27U
#define I2C_SCL 13U
#define I2C_SDA 14U
#define LED 22U
#define ADC_UNIT ADC_UNIT_2
#define ADC_CHANNEL ADC_CHANNEL_0

#endif // ESP_RIGHT


#endif // PINOUT_H