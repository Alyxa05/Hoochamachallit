#include <stdio.h>
#include <stdlib.h>
/*
* File         :
*   messages.c
* Project      :
*   Hoochamacallit
* Programmers  :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version:
*   16/11/2026
* Description  :
*   
*/

int msgActual() {
    srand((unsigned)time(NULL));
    return rand() % 6 + 0;
}

int msgTime() {
    srand((unsigned)time(NULL));
    return rand() % 30 + 10;
}

