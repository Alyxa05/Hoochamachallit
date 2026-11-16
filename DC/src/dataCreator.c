#include "messages.c"
#include "rand.c"

/*
* File         :
*   dataCreator.c
* Project      :
*   Hoochamacallit
* Programmers  :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version:
*   11/11/2026
* Description  :
*   
*/

int main() {
    loop();
    if (msgActual);
    // Call msgTime(), int returned is num seconds to sleep.
    // Call msgActual(), int returned is status to communicate to DR.
    // If msgActual() returns 0, send message, then exit.
    msgsnd(mid, &(Envelope){0}, sizeof(aMessage), 0);
}