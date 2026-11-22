/*
* File         :
*   dataCorrupter.c
* Project      :
*   Hoochamacallit
* Programmers  :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version:
*   11/11/2026
* Description  : main program entry for the DX application and has the main loop
*   
*/

#include "dataCorruptor.h"
#include <hooch.h>
#include <sys/ipc.h>
#include <signal.h>

int main() {

    key_t shmKey;
    bool shmAttached = false;

    int wheelResult;
    int corruptAction;
    int targetDCIndex = -1;
    pid_t victimID = -1;
    char logMsg[LOG_MSG_LEN];
    
    int shmId;
    MasterList* master = NULL;

    srand(time(NULL));

    //try 100 times to get the shared memory id
    for(int attachAttempts = 0; attachAttempts < ATTACH_ATTEMPTS + 1; attachAttempts++)
    {
        //Get the shared memory id
        shmKey = ftok(".", HOOCH_KEY_ID);

        //sleep if failed to get a shared memory id
        if (shmKey == -1) 
        {
            sleep(10);
        }
        else
        {
            //get the shared memory id
            shmId = shmget(shmKey, sizeof(MasterList), 0660);
            if(shmId == -1)
            {
                sleep(10);
            }
            else
            {
                //attempt to attach to the memory to access the list of DCs
                master = (MasterList*)shmat(shmId, NULL, 0);

                if(master == (void*)-1)
                {
                    master = NULL;
                    sleep(10);
                }
                else
                {
                    //exit loop if shared memory id retrieved and we attached
                    shmAttached = true;
                    break;
                }
            }


        }
    }

    if (!shmAttached) 
    {
        //log error and exit
        generateLogMsg(
            logMsg, sizeof(logMsg), action_error,
            action_error, action_error, cannot_attach  
        );
        writeLog(logMsg);
        exit(EXIT_FAILURE);
    }
    

    //Main processing loop
    while(shmAttached)
    {
        wheelResult = -1;
        corruptAction = -1;
        targetDCIndex = -1;
        victimID = -1;


        //1. sleep 10-30 sec
        sleep(getRandomNapTime());

        //2, check for existing message queue
        
        //if message queue does not exist
        if(msgget(shmKey, 0) == -1)
        {
            //queue does not exist
            generateLogMsg(
                logMsg, sizeof(logMsg), action_error,
                action_error, action_error, msgQ_Gone
            );
            writeLog(logMsg);


            //detach from shared memory
            if(master != NULL)
            {
                shmdt(master);
                master = NULL;
            }
            //exit
            exit(EXIT_SUCCESS);
        }

        //3. spin wheel of destruction (call fuction)
        wheelResult = spinWheel();
        corruptAction = defineAction(wheelResult); 

        

        //4. EXECUTE THE ACTION
        if(corruptAction == kill_a_DC)
        {
            targetDCIndex = getTargetDCIndex(wheelResult);
            victimID = kill_DC(targetDCIndex, master);
            //log killing a DC
            generateLogMsg(
                logMsg, sizeof(logMsg), wheelResult,
                targetDCIndex, victimID, kill_a_DC
            );
            writeLog(logMsg);
        }
        else if(corruptAction == no_action)
        {
            //don't need to log anything
            //this elseif condition happens more often than the following conditions
            //thus it is here to help the checks en as early as possible
        }
        else if(corruptAction == delete_msgQ)
        {
            deleteMsgQueue(shmKey);

            //log message queue deletion
            generateLogMsg(
                logMsg, sizeof(logMsg), wheelResult,
                action_error, action_error, delete_msgQ
            );
            writeLog(logMsg);

            //detach immediately
            if(master != NULL)
            {
                shmdt(master);
                master = NULL;
            }
            //exit
            exit(EXIT_SUCCESS);
        }

    }
    return EXIT_SUCCESS;

}