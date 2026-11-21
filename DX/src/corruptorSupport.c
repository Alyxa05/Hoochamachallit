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

int getRandomNapTime() {
    //generate random number between 10 and 30
    int napTime = (rand() % 21) + 10;
    return napTime;
}