/*
 * Common/inc/hooch.h
 * Stuff shared between DR, DC, and DX so we don't accidentally use different structs.
 * Keep adding things in here as needed gang.
 */

#ifndef HOOCH_H
#define HOOCH_H

#include <sys/types.h>
#include <time.h>

#define MAX_DC_ROLES 10
#define HOOCH_KEY_ID 16535

 // Status codes coming in from the Hoochamacallit machines feel free to change the names just let me know
#define STATE_GOOD          0
#define STATE_HYDRAULIC_BAD 1
#define STATE_SAFETY_BAD    2
#define STATE_NO_STUFF      3
#define STATE_TOO_HOT       4
#define STATE_HUMAN_OOPS    5
#define STATE_DEAD_OFFLINE  6

//Message packet DC sends over the queue
typedef struct
{
    long mtype;
    pid_t machinePid;
    int state;
} MachineMsg;

// DC machine slot in shared memory
typedef struct
{
    pid_t dcProcessID;
    time_t lastTimeHeardFrom;
} DCInfo;

//The one list above all - master list living in shared memory
typedef struct
{
    int msgQueueID;
    int numberOfDCs;
    DCInfo dc[MAX_DC_ROLES];
} MasterList;

#endif
