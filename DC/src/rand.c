#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
* File         :
*   rand.c
* Project      :
*   Hoochamacallit
* Programmers  :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version:
*   16/11/2026
* Description  :
*   Contains all rand operations for security
*/


/*
* Function    : initRandom
* Description : Seeds the pseudo-random number generator using current time and PID
*               so simultaneous DC instances generate distinct sequences.
* Parameters  : None.
* Returns     : None.
*/
void initRandom() {
    srand((unsigned)time(NULL) ^ (unsigned)getpid());
}

/*
* Function    : initRandom
* Description : Seeds the pseudo-random number generator using current time and PID
*               so simultaneous DC instances generate distinct sequences.
* Parameters  : None.
* Returns     : None.
*/
int msgActual() {
    return rand() % 7;
}

/*
* Function    : initRandom
* Description : Seeds the pseudo-random number generator using current time and PID
*               so simultaneous DC instances generate distinct sequences.
* Parameters  : None.
* Returns     : None.
*/
int msgTime() {
    return 10 + (rand() % 21);
}

