#ifndef PINOUT_H
#define PINOUT_H


/******************* PINOUT FOR LEFT ESP PICO  ********************/

#ifdef ESP_LEFT

#define END_CONE 34U
#define TELE_APOGEE_CHECK 35U
#define EASY_APOGEE_CHECK 22U
#define TELE_IGNITER_CONT 19U
#define EASY_IGNITER_CONT 9U
#define PILOT_DEPLOY 27U 
#define EASY_IGNITER_FIRE 26U
#define TELE_IGNITER_FIRE 14U
#define EASY_ARMING 25U
#define TELE_ARMING 13U
#define I2C_SCL 23U
#define I2C_SDA 18U
#define LED 21U
#define PRESSUER 4U

#endif // ESP_LEFT

/******************* PINOUT FOR RIGHT ESP PICO  *******************/

#ifdef ESP_RIGHT

#define END_CONE 9U
#define TELE_APOGEE_CHECK 19U
#define EASY_APOGEE_CHECK 34U
#define TELE_IGNITER_CONT 22U
#define EASY_IGNITER_CONT 35U
#define PRESSURE 4U
#define PILOT_DEPLOY 27U 
#define EASY_IGNITER_FIRE 26U
#define TELE_IGNITER_FIRE 14U
#define EASY_ARMING 25U
#define TELE_ARMING 13U
#define I2C_SCL 23U
#define I2C_SDA 18U
#define LED 21U

#endif // ESP_RIGHT


#endif // PINOUT_H