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
#include <hooch.h>



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

int targetDCID(int wheelResult, MasterList* master) {
    
    int targetDCID;
    //cool way to stack cases for multiple conditions that execute one block
    switch(wheelResult)
    {
        case 1:
        case 4:
        case 11:
            //
            break;

        case 3:
        case 6:
        case 13:
            //
            break;
        
        case 2:
        case 5:
        case 15:
            //
            break;
        
        case 7:
            //
            break;
        case 9:
            //
            break;
        case 12:        
            //
            break;
        case 14:
            //
            break;
        case 16:
            //
            break;
        case 18:
            //
            break;
        case 20:
            //
            break;
        default:
            //error selecting action
            break;
    }

    return 0; // Placeholder return
}

void kill_DC(int corruptAction, int targetDCID, int shmId, MasterList* master) 
{

}