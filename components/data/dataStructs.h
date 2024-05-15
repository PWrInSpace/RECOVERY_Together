#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "config.h"

typedef struct {
    bool isArmed :1;
    bool isTeleActive :1;
    bool easyMiniFirstStage :1;
    bool easyMiniSecondStage :1;
    bool telemetrumFirstStage :1;
    bool telemetrumSecondStage :1;
    bool firstStageDone :1;
    bool secondStageDone :1;
    bool firstStageContinouity :1;
    bool secondStageContinouity :1;
    uint8_t pressure : 6; 


} data_to_mcb;

extern data_to_mcb data_to_send;

typedef struct {

    uint32_t cmd;
    int32_t payload;

}data_from_mcb;

#endif // DATA_STRUCTS_H