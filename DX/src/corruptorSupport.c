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
#define _POSIX_C_SOURCE 200809L

#include "dataCorruptor.h"
#include "hooch.h"
#include <sys/ipc.h>
#include <signal.h>





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

int getTargetDCIndex(int wheelResult) {
    
    int targetDCIndex;
    //cool way to stack cases for multiple conditions that execute one block
    switch(wheelResult)
    {
        case 1:
        case 4:
        case 11:
            targetDCIndex = 0;
            break;

        case 3:
        case 6:
        case 13:
            targetDCIndex = 1;
            break;
        
        case 2:
        case 5:
        case 15:
            targetDCIndex = 2;
            break;
        
        case 7:
            targetDCIndex = 3;
            break;
        case 9:
            targetDCIndex = 4;
            break;
        case 12:        
            targetDCIndex = 5;
            break;
        case 14:
            targetDCIndex = 6;
            break;
        case 16:
            targetDCIndex = 7;
            break;
        case 18:
            targetDCIndex = 8;
            break;
        case 20:
            targetDCIndex = 9;
            break;
        default:
            //error selecting action
            targetDCIndex = -1;
            break;
    }

    return targetDCIndex; // Placeholder return
}

pid_t kill_DC(int targetDCIndex, MasterList* master) 
{
    pid_t pidToKill;

    //check in case of error
    if(master == NULL)
    {
        return -1;
    }
    if(targetDCIndex < 0 || targetDCIndex >= master->numberOfDCs)
    {
        return -1;
    }


    pidToKill = master->dc[targetDCIndex].dcProcessID;

    if (kill(pidToKill, SIGHUP) == -1) 
    {
        return -1; //error indicator
    } 
    // else 
    // {
    //     //kill was successful, return the pid
    // }
    
    return pidToKill;

}

void deleteMsgQueue(int shmKey) 
{
    int msgQId = msgget(shmKey, 0);
    //printf("DX deleting queue id = %d\n", msgQId); //debug line 
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

//logging functions

void generateLogMsg(char* buffer, size_t size, int wheelResult, int targetDCIndex, pid_t victimID, int corruptAction)
{
    time_t now = time(NULL);
    
    char timestamp[20];
    struct tm *time_data = localtime(&now);
    //time formatting https://www.geeksforgeeks.org/cpp/strftime-function-in-c/
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_data);

    switch(corruptAction)
    {
        case kill_a_DC:
            if(victimID == -1)
            {
                snprintf(buffer, size, 
                    "[%s] : WOD Action %d - DC-%02d [%d] this DC does not exist",timestamp, wheelResult, (targetDCIndex+1), (int)victimID
                );
            }
            else{

                snprintf(buffer, size, 
                    "[%s] : WOD Action %d - DC-%02d [%d] TERMINATED",timestamp, wheelResult, (targetDCIndex+1), (int)victimID
                );
            }
            break;
        case delete_msgQ:
            snprintf(buffer, size, 
                "[%s] : DX deleted the msgQ - the DR/DCs can't talk anymore - exiting",
                timestamp
            );
            break;
        case msgQ_Gone:
            snprintf(buffer, size, 
                "[%s] : DX detected that msgQ is gone - assuming DR/DCs done",
                timestamp
            );
            break;
        case cannot_attach:
            snprintf(buffer, size, 
                "[%s] : DX was unable to get shared memory id after 100 attempts - exiting",
                timestamp
            );
            break;
        default:
            snprintf(buffer, size, 
                "[%s] : Unknown action log",
                timestamp
            );

    }

    return; 
}

void writeLog(const char *logMessage)
{
    //printf("LOGGING");
    FILE *logFile = fopen("/tmp/dataCorruptor.log", "a");

    if (logFile == NULL)
    {
        //perror("fopen");
        return;
    }

    fprintf(logFile, "%s\n", logMessage);
    fclose(logFile);
}