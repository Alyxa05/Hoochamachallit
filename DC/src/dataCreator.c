#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include "dc_helpers.c"
#include "rand.c"

/*
* File         : dataCreator.c
* Project      : Hoochamacallit
* Programmers  :
*   Alyx Deland, Josh Ducharme-Baribeau, Erik Vankooy
* First Version: 11/11/2026
* Description  :
*   The data creator "server" main entry point.
*/

int main() {
    struct sigaction sa = {0};
    sa.sa_handler = handleSignal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGHUP, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    initRandom();
    pid_t myPid = getpid();
    key_t msgKey = ftok(FTOK_PATH, HOOCH_KEY_ID);

    // Die if no key, wait for queue, then die if no
    // queue after waiting
    if (msgKey == -1) return EXIT_FAILURE;
    int queueID = waitForMessageQueue(msgKey);
    if (queueID == -1) return EXIT_SUCCESS;

    int currentState = STATE_GOOD;
    if (sendStatus(queueID, myPid, currentState) == -1) return EXIT_FAILURE;
    logStatus(myPid, currentState);

    // If not stop rq'd, sleep for rand time, break if rq'd
    // set rand state, log then break if fail, same if state 6
    while (!stopRequested) {
        int sleepDuration = msgTime();
        sleep ((unsigned int)sleepDuration);

        if (queueID == -1) return EXIT_SUCCESS;
        if (stopRequested) break;

        currentState = msgActual();

        if (sendStatus(queueID, myPid, currentState) == -1) {            
            logStatus(myPid, currentState);
            break;
        }

        if (currentState == STATE_DEAD_OFFLINE) {
            logStatus(myPid, currentState);
            break;
        }
    }
    return EXIT_SUCCESS;
}