#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "config.h"

typedef struct {
    bool isArmed :1; // Easymini arming signall
    bool isTeleActive :1; 
    bool easyMiniFirstStage :1;
    bool easyMiniSecondStage :1;
    bool telemetrumFirstStage :1;
    bool telemetrumSecondStage :1;
    bool firstStageDone :1;
    bool secondStageDone :1;
    bool firstStageContinouity :1;
    bool secondStageContinouity :1;
    bool separationSwitch1 :1;
    bool separationSwitch2 :1;
    uint16_t pressure1;
} recovery_data_t;

extern recovery_data_t data_to_send;

typedef struct {

    uint32_t cmd;
    int32_t payload;

}data_from_mcb;

#endif // DATA_STRUCTS_H