#ifndef PINOUT_H
#define PINOUT_H


/******************* PINOUT FOR LEFT ESP PICO  ********************/

#ifdef ESP_LEFT

#define END_CONE 4U
#define TELE_APOGEE_CHECK 7U
#define EASY_APOGEE_CHECK 9U
#define TELE_IGNITER_CONT 8U
#define EASY_IGNITER_CONT 10U
#define PILOT_DEPLOY 22U 
#define EASY_IGNITER_FIRE 5U
#define TELE_IGNITER_FIRE 23U
#define EASY_ARMING 18U
#define TELE_ARMING 19U
#define I2C_SCL 34U
#define I2C_SDA 35U
#define LED 21U


#endif // ESP_LEFT

/******************* PINOUT FOR RIGHT ESP PICO  *******************/

#ifdef ESP_RIGHT

#define END_CONE 12U
#define TELE_APOGEE_CHECK 8U
#define EASY_APOGEE_CHECK 18U
#define TELE_IGNITER_CONT 7U
#define EASY_IGNITER_CONT 5U
#define PRESSURE 18U
#define PILOT_DEPLOY 4U 
#define EASY_IGNITER_FIRE 10U
#define TELE_IGNITER_FIRE 17U
#define EASY_ARMING 9U
#define TELE_ARMING 16U
#define I2C_SCL 13U
#define I2C_SDA 15U
#define LED 27U

#endif // ESP_RIGHT


#endif // PINOUT_H