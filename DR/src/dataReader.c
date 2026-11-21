/*
 * File         : DR/src/datareader.c
 * Project      : Hoochamacallit
 * Programmers  : Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
 * First Version: 11/11/2026
 * Description  : Data Reader (DR) for Hoochamacallit System - sets up IPC,
 *                maintains MasterList in shared memory, processes DC messages,
 *                and removes offline or non-responsive DCs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include "hooch.h"

#define DC_TIMEOUT_SECONDS 35.0

 /*
  * Removes the DC entry at `index` from `master->dc[]` and shifts any
  * entries after it one slot left so active DCs stay contiguous.
  */
static void removeDcAt(MasterList* master, int index)
{
    int i;

    if (master == NULL || index < 0 || index >= master->numberOfDCs)
    {
        return;
    }

    for (i = index + 1; i < master->numberOfDCs; i++)
    {
        master->dc[i - 1] = master->dc[i];
    }

    master->numberOfDCs--;
}

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

    // Check if queue already exists first, otherwise create it
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

    // Allocate our shared memory for the master list
    shmId = shmget(ipcKey, sizeof(MasterList), IPC_CREAT | 0660);
    if (shmId == -1)
    {
        perror("(DR) shmget failed");
        msgctl(msgQId, IPC_RMID, NULL);
        return 3;
    }

    // Attaching shared memory and set starting values
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

    // Give the DC processes and DX time to start up before entering the loop otherwise it will get all fucky?
    sleep(15);

    // Main processing loop: drain queue messages, then check for >35s silence
    while (1)
    {
        MachineMsg msg;

		// This loop will drain all messages in the queue until there are none left (msgrcv returns -1 and errno is ENOMSG)
        while (msgrcv(msgQId, &msg, sizeof(msg) - sizeof(long), 0, IPC_NOWAIT) != -1)
        {
            int foundIndex = -1;
            int i;
            time_t now = time(NULL);

            // Look for an existing one that is matching this DC's PID
            for (i = 0; i < master->numberOfDCs; i++)
            {
                if (master->dc[i].dcProcessID == msg.machinePid)
                {
                    foundIndex = i;
                    break;
                }
            }

            if (msg.state == STATE_DEAD_OFFLINE)
            {
                if (foundIndex != -1)
                {
                    // Log DC has gone OFFLINE - removing from master-list
                    removeDcAt(master, foundIndex);
                }
            }
            else if (foundIndex != -1)
            {
                master->dc[foundIndex].lastTimeHeardFrom = now;
                // Log DC updated in master list - MSG RECEIVED - Status (...)
            }
            else if (master->numberOfDCs < MAX_DC_ROLES)
            {
                master->dc[master->numberOfDCs].dcProcessID = msg.machinePid;
                master->dc[master->numberOfDCs].lastTimeHeardFrom = now;
                master->numberOfDCs++;
                // Log DC added to master list - NEW DC - Status 0 (Everything is OKAY)
            }
        }

        if (errno != ENOMSG)
        {
            perror("(DR) msgrcv failed");
            break;
        }

        // Check all DCs in master list for >35 seconds of inactivity so we can obliterate it
        {
            time_t now = time(NULL);
            int i;

            // Walk backwards so collapsing a later index does not skip earlier ones
            for (i = master->numberOfDCs - 1; i >= 0; i--)
            {
                if (difftime(now, master->dc[i].lastTimeHeardFrom) > DC_TIMEOUT_SECONDS)
                {
                    // Log DC-%02d [%d] removed from master list - NON-RESPONSIVE
                    removeDcAt(master, i);
                }
            }
        }

        sleep(1);
    }

    // Cleanup the IPC resources when exiting the loop
    shmdt(master);
    shmctl(shmId, IPC_RMID, NULL);
    msgctl(msgQId, IPC_RMID, NULL);

    return 0;
}