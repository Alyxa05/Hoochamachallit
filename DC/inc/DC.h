/*
* DC/inc/DC.h
* Defines and prototypes for dataCreator.c
*/
#ifndef DC_H
#define DC_H

#define LOG_FILE_PATH "/tmp/DC.log"
#define QUEUE_WAIT_SEC 10
#define FTOK_PATH "../../DR/bin"

// Prototpes
static void handleSignal(int sigNum);
static int waitForMessageQueue(key_t msgKey);
static int sendStatus(int queueID, pid_t pid, int state);
static void logStatus(pid_t pid, int state);
static const char* getStatusDescription(int state);
void initRandom(void);
int msgActual(void);
int msgTime(void);

#endif
