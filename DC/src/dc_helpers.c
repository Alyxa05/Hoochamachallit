#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/msg.h>
#include "../../Common/inc/hooch.h"
#include "../inc/DC.h"
/*
* File         :
*   dc_helpers.c
* Project      :
*   Hoochamacallit
* Programmers  :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version:
*   21/11/2026
* Description  :
*   Contains dataCreator functions
*/

static volatile sig_atomic_t stopRequested = 0;
/*
* Function   : handleSignal
* Desctiption:
*   Signal handler for graceful termination.
* Parameters :
*   int sigNum - Recieved signal
* Returns    : None
*/
static void handleSignal(int sigNum) {
    (void)sigNum;
    stopRequested = 1;
}

/*
* Function   : getStatusDescripton
* Desctiption:
*   Maps Status codes to human readable string
* Parameters :
*   int state - Status code
* Returns    :
*   const char* - Status descrtiption
*/
static const char* getStatusDescription(int state) {
    switch (state)
    {
        case STATE_GOOD: return "Clear Skies";
        case STATE_HYDRAULIC_BAD: return "Hydraulic Pressure Fault";
        case STATE_SAFETY_BAD: return "Safety Button Fault";
        case STATE_NO_STUFF: return "No Raw Materials";
        case STATE_TOO_HOT: return "Temperature Out Of Range";
        case STATE_HUMAN_OOPS: return "Operator Error";
        case STATE_DEAD_OFFLINE: return "Machine Off-line";
        default: return "Unknown Status";
    }
}

/*
* Function   : waitForMessageQueue
* Desctiption:
*   Pings for a queue, waits 10 seconds if none,
*   repeats until queue found.
* Parameters :
*   key_t msgKey - IPC key for the queue.
* Returns    :
*   int - Valid queue ID, or -1 if interrupted.
*/
static int waitForMessageQueue(key_t msgKey) {
    int queueID = -1;

    while (!stopRequested) {
        queueID = msgget(msgKey, 0);
        if (queueID != -1) return queueID;
        sleep(QUEUE_WAIT_SEC);
    }
    return -1;
}

/*
* Function   : sendStatus
* Desctiption:
*   Builds and sends MachineMsg packets to the queue.
* Parameters :
*   int queueID - Target msg's queue ID.
*   pid_t pid - Current DC instance's pid.
*   int state - Sent status code. 
* Returns    :
* int - 0 on success, -1 on fail.
*/
static int sendStatus(int queueID, pid_t pid, int state) {
    MachineMsg msg;
    msg.mtype = 1;
    msg.machinePid = pid;
    msg.state = state;

    size_t payloadSize = sizeof(MachineMsg) - sizeof(long);

    if (msgsnd(queueID, &msg, payloadSize,0) == -1) return -1;
    else return 0;
}

/*
* Function   : logStatus
* Desctiption:
*   Appends a formated log to LOG_FILE_PATH.
* Parameters :
*   pid_t pid - Current DC instance's pid.
*   int state - Sent status code.
* Returns    :
*   Null - Die if no logFile.
*/
static void logStatus(pid_t pid, int state) {
    FILE* logFile = fopen(LOG_FILE_PATH, "a");
    if (logFile == NULL) return;
    
    time_t now = time(NULL);
    struct tm* localTime = localtime(&now);
    char timeBuff[32] = {0};

    if (localTime != NULL) {
        strftime(timeBuff, sizeof(timeBuff), "%Y-%m-%d %H:%M:%S", localTime);
    }
    fprintf(logFile, "[%s]: DC [%d] MSG SENT; Status %d (%s)\n",
            timeBuff, (int)pid, state, getStatusDescription(state));
    fclose(logFile);
}