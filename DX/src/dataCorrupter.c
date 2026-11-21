/*
* File         :
*   dataCorrupter.c
* Project      :
*   Hoochamacallit
* Programmers  :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version:
*   11/11/2026
* Description  :
*   
*/

#include "dataCorruptor.h"
#include <hooch.h>

int main() {

    key_t shmKey;
    bool shmAttached = false;
    int msgQueueId;
    
    int shmId;
    MasterList* master;

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
            shmAttached = true;

            //exit loop if shared memory id retrieved
            break;
        }
    }

    if (!shmAttached) 
    {
        //log error and exit
        //"DX was Unable to get shared memory id after 100 attempts - exiting"
        exit(EXIT_FAILURE);
    }
    

    //Main processing loop
    while(shmAttached)
    {

        int wheelResult;
        int corruptAction;

        //1. sleep 10-30 sec
        sleep(getRandomNapTime());

        //2, check for existing message queue
        
        //if message queue does not exist
        if(msgget(shmKey, 0) == -1)
        {
            //queue does not exist
            //a)log this event (assumes the dc's and dr's have exited)
                //"{DATETIMESTAMP} : DX detected that the msgQ is gone - assuming DR/DCs done"
            //b)detach from shared memory
            //c)exit
        }

        //3. spin wheel of destruction (call fuction)
        wheelResult = spinWheel();
        corruptAction = defineAction(wheelResult); 

        

        //4. execute action that was selected
            //if killing a process send SIGHUP
            //when delteing files, killing processes or removing messages, ensure to handle potential errors
        if(corruptAction == kill_a_DC)
        {
            int targetDCIndex = getTargetDCIndex(wheelResult, master);
            kill_DC(targetDCIndex, shmId, master);
            //log killing a DC
            //"{DATETIMESTAMP} : DX spun the wheel and got {wheelResult} - killing a DC"
        }
        else if(corruptAction == no_action)
        {
            
        }
        else if(corruptAction == delete_msgQ)
        {
            deleteMsgQueue(shmKey);

            //log message queue deletion
            //"{DATETIMESTAMP} : DX spun the wheel and got {wheelResult} - deleting message queue"

        }
        else
        {
            //log error selecting action
            //"{DATETIMESTAMP} : DX spun the wheel and got {wheelResult} - error selecting action"
        }


    }

}