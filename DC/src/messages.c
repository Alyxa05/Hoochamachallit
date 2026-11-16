#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
/*
* File         :
*   messages.c
* Project      :
*   Hoochamacallit
* Programmers  :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version:
*   16/11/2026
* Description  :
*   
*/

key_t hooch_key;
int mid;

typedef struct tagAMESSAGE {
    int command;
    int param1;
    int param2;
    int result;
    char rString[128];
} aMessage;

// Won't let me declare the struct with a title(?)
typedef struct {
    long type;
    aMessage data;
} Envelope;

int makeQueue() {
    if (!hooch_key) {
        hooch_key = ftok("../../", 'H');
    }
    if (!(mid = msgget(hooch_key, 0))) {
        mid = msgget(hooch_key, IPC_CREAT | 0660);
    }
    return 0;
}

void loop() {
    while (!mid) {
        sleep(10);
    }
}

void killQueue() {
    msgctl(mid, IPC_RMID, (struct msqid_ds *)NULL);
}