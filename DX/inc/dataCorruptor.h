#ifndef DATACORRUPTOR_H
#define DATACORRUPTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>

#include "hooch.h"

#define ATTACH_ATTEMPTS 100
#define LOG_MSG_LEN 255

//defining wheel of destruction compund conditions as replacements for readability
#define deleteMsgQCondition ((action == 10)||(action == 17))
#define noActionCondition ((action == 0)||(action == 8)||(action == 19))

//defining wheel of destruction actions as constants to remove magic numbers
#define msgQ_Gone 3
#define kill_a_DC 2
#define delete_msgQ 1
#define no_action 0
#define action_error -1
#define cannot_attach -2


int getRandomNapTime();

int spinWheel();

int defineAction(int wheelResult);

int getTargetDCIndex(int wheelResult);

pid_t kill_DC(int targetDCID, MasterList* master);

void deleteMsgQueue(int smhKey);

void writeLog(const char* logMessage);

void generateLogMsg(char* buffer, size_t size, int wheelResult, int targetDCIndex, pid_t victimID, int corruptAction);


#endif