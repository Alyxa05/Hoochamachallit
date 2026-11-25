#include <stdlib.h>
#include <time.h>
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
* Description :
*   Seeds the randomness.
* Parameters  : None.
* Returns     : None.
*/
void initRandom() {
    srand((unsigned int)time(NULL) ^ (unsigned int)getpid());
}

/*
* Function    : initRandom
* Description : 
*   Random for message signal generation.
* Parameters  : None.
* Returns     : None.
*/
int msgActual() {
    return rand() % 7;
}

/*
* Function    : initRandom
* Description : 
*   Random for time between messages.
* Parameters  : None.
* Returns     : None.
*/
int msgTime() {
    return 10 + (rand() % 21);
}
