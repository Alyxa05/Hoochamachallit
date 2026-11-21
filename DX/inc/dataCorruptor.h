#ifndef DATACORRUPTOR_H
#define DATACORRUPTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ATTACH_ATTEMPTS 100

//defining wheel of destruction compund conditions as constants for readability
// #define killDCOneCondition ((action == 1)||(action == 4)||(action == 11))

// #define killDCTwoCondition ((action == 3)||(action == 6)||(action == 13))

// #define killDCThreeCondition ((action == 2)||(action == 5)||(action == 15))

// #define killDCFourCondition ((action == 7))

// #define killDCFiveCondition ((action == 9))

// #define killDCSixCondition ((action == 12))

// #define killDCSevenCondition ((action == 14))

// #define killDCEightCondition ((action == 16))

// #define killDCNineCondition ((action == 18))

// #define killDCTenCondition ((action == 20))

#define deleteMsgQCondition ((action == 10)||(action == 17))

#define noActionCondition ((action == 0)||(action == 8)||(action == 19))

//defining wheel of destruction actions as constants to remove magic numbers
#define kill_a_DC 2
#define delete_msgQ 1
#define no_action 0
#define action_error -1



int getRandomNapTime();

int spinWheel();

int defineAction(int wheelResult);

int targetDCID(int wheelResult, MasterList* master);

void kill_DC(int action, int targetDCID, int shmId, MasterList* master);

#endif