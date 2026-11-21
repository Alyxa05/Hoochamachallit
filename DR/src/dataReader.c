#include <stdio.h>
/*
* File         :
*   DR/src/dr_main.c
* Project      :
*   Hoochamacallit
* Programmers  :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version:
*   11/11/2026
* Description  :
*   
*/

/*
 * Starting on the Data Reader: grab the key, set up the queue and
 * shared memory for the master list, and wait 15s for DCs/DX to show up.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "hooch.h"

int main(void)
{
    key_t ipcKey;
    int msgQId;
    int shmId;
    MasterList* master;

    // Key comes from current folder + project ID (DR and DX run from same folder)
    ipcKey = ftok(".", HOOCH_KEY_ID);
    if (ipcKey == -1)
    {
        perror("(DR) ftok failed");
        return 1;
    }

    // Check if queue already exists first, otherwise we makin dat
    msgQId = msgget(ipcKey, 0);
    if (msgQId == -1)
    {
        msgQId = msgget(ipcKey, IPC_CREAT | 0660);
        if (msgQId == -1)
        {
            perror("(DR) msgget failed");
            return 2;
        }
    }

    // Allocate shared memory for the master list
    shmId = shmget(ipcKey, sizeof(MasterList), IPC_CREAT | 0660);
    if (shmId == -1)
    {
        perror("(DR) shmet failed");
        msgctl(msgQId, IPC_RMID, NULL);
        return 3;
    }

    // Attach to shared memory and set starting values
    master = (MasterList*)shmat(shmId, NULL, 0);
    if (master == (MasterList*)-1)
    {
        perror("(DR) shmat failed");
        shmctl(shmId, IPC_RMID, NULL);
        msgctl(msgQId, IPC_RMID, NULL);
        return 4;
    }

    master->msgQueueID = msgQId;
    master->numberOfDCs = 0;

    // Give DC processes and DX time to start up before entering the loop
    sleep(15);

    // Cleanup placeholders for now until loop/exit logic is added
    shmdt(master);
    shmctl(shmId, IPC_RMID, NULL);
    msgctl(msgQId, IPC_RMID, NULL);

    return 0;
}
// Temp loop pulling any waiting messages and track seen DC PIDs
while (1)
{
    MachineMsg msg;

    // Loop while messages are sitting in the queue
    while (msgrcv(msgQId, &msg, sizeof(msg) - sizeof(long), 0, IPC_NOWAIT) != -1)
    {
        int foundIndex = -1;
        int i;
        time_t now = time(NULL);

        // Checking if we already have this DC PID in the master list
        for (i = 0; i < master->numberOfDCs; i++)
        {
            if (master->dc[i].dcProcessID == msg.machinePid)
            {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex != -1)
        {
            master->dc[foundIndex].lastTimeHeardFrom = now;
        }
        else if (master->numberOfDCs < MAX_DC_ROLES)
        {
            master->dc[master->numberOfDCs].dcProcessID = msg.machinePid;
            master->dc[master->numberOfDCs].lastTimeHeardFrom = now;
            master->numberOfDCs++;
        }
    }

    if (errno != ENOMSG)
    {
        perror("(DR) msgrcv failed");
        break;
    }

    sleep(1);
}