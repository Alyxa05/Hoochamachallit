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

int main() {

    bool shmAttached = false;

    //try 100 times to get the shared memory id
    for(int attachAttempts = 0; attachAttempts < ATTACH_ATTEMPTS + 1; attachAttempts++)
    {
        //Get the shared memory id
        key_t shmKey = ftok(".", 16535);

        //sleep if failed
        if (shmKey == -1) 
        {
            sleep(10);
        }
        else
        {
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

        //1. sleep 10-30 sec
        sleep(getRandomNapTime());

        //2, check for existing message queue
            //if message queue does not exist
                //a)log this event (assumes the dc's and dr's have exited)
                    //"{DATETIMESTAMP} : DX detected that the msgQ is gone - assuming DR/DCs done"
                //b)detach from shared memory
                //c)exit
        //3. spin wheel of destruction (call fuction)

        //4. execute action that was selected
            //if killing a process send SIGHUP
            //when delteing files, killing processes or removing messages, ensure to handle potential errors
    }

}