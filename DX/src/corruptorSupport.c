/*
* File          :
*   dataCorrupter.c
* Project       :
*   Hoochamacallit
* Programmers   :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version :
*   21/11/2026
* Description   :
*   supporting file for the data corrupter process. This file contains supporting functions for the data corruptor.
*   
*/

#include "dataCorruptor.h"
#include "hooch.h"
#include <sys/ipc.h>




int getRandomNapTime() {
    //generate random number between 10 and 30
    int napTime = (rand() % 21) + 10;
    return napTime; 
}

int spinWheel() {
    //generate random number between 0 and 20
    int action = (rand() % 21);
    return action; 
}

int defineAction(int action) {
    //define action based on wheel result
    int corruptAction = 0;

    if (noActionCondition) {
        corruptAction = no_action; // No action
    }
    else if (deleteMsgQCondition)
    {
        corruptAction = delete_msgQ; // Delete message queue
    }
    else if(action <= 20 && action >= 0)
    {
        corruptAction = kill_a_DC; // Kill DC
    }
    else
    {
        corruptAction = action_error; //error selecting action
    }

    return corruptAction; 
}

int targetDCIndex(int wheelResult, MasterList* master) {
    
    int targetDCIndex;
    //cool way to stack cases for multiple conditions that execute one block
    switch(wheelResult)
    {
        case 1:
        case 4:
        case 11:
            targetDCIndex = 1;
            break;

        case 3:
        case 6:
        case 13:
            targetDCIndex = 2;
            break;
        
        case 2:
        case 5:
        case 15:
            targetDCIndex = 3;
            break;
        
        case 7:
            targetDCIndex = 4;
            break;
        case 9:
            targetDCIndex = 5;
            break;
        case 12:        
            targetDCIndex = 6;
            break;
        case 14:
            targetDCIndex = 7;
            break;
        case 16:
            targetDCIndex = 8;
            break;
        case 18:
            targetDCIndex = 9;
            break;
        case 20:
            targetDCIndex = 10;
            break;
        default:
            //error selecting action
            targetDCIndex = -1;
            break;
    }

    return targetDCIndex; // Placeholder return
}

void kill_DC(int targetDCIndex, int shmId, MasterList* master) 
{
    pid_t pidToKill;

    pidToKill = master->dc[targetDCIndex].dcProcessID;

    if (kill(pidToKill, SIGHUP) == -1) 
    {
        // perror("Failed to send SIGHUP to DC process");
        //log unsuccessful attempt to send SIGHUP to DC
    } 
    else 
    {
        //log successful kill attempt
    }
    
    return;

}

void deleteMsgQueue(int shmKey) 
{
    int msgQId = msgget(shmKey, 0);
    if (msgQId != -1) 
    {
        //CONTROL to remove the queue, 3rd argument is usually null
        if (msgctl(msgQId, IPC_RMID, NULL) == -1) 
        {
            //log unsuccessful attempt to delete message queue
            // perror("Failed to delete message queue");
        } 
        else 
        {
            //log successful deletion of message queue
        }
    } 
    else 
    {
        // perror("Message queue does not exist");
        //log unsuccessful attempt to delete message queue
    }
}