/*
- File         : DR/src/dr_helpers.c
- Project      : Hoochamacallit
- Programmers  : Alyx Deland, Josh Ducharme-Baribeau, Erik vanKooy
- First Version: 11/11/2026
- Description  : Implementation of Data Reader (DR) helper functions for
-                status descriptions, timestamped log output, removing
-                DC entries from MasterList, and 1.5s loop sleep.
 */

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "dr_helpers.h"

 //Status text lookup for DC state codes
const char* getStatusText(int status)
{
    switch (status)
    {
    case 0:
        return "Everything is OKAY";
    case 1:
        return "Hydraulic Pressure Failure";
    case 2:
        return "Safety Button Failure";
    case 3:
        return "No Raw Material in the Process";
    case 4:
        return "Operating Temperature Out of Range";
    case 5:
        return "Operator Error";
    case 6:
        return "Machine is Off-line";
    default:
        return "Unknown Status";
    }
}

/*
* Name: writeDrLog
* Description: Writes a log message to /tmp/dataMonitor.log with a timestamp.
* Parameters:
* - message: The log message to write.
* Returns: None
*/
void writeDrLog(const char* message)
{
    FILE* logFile;
    time_t now;
    struct tm* localInfo;
    char timeStr[32];

    if (message == NULL)
    {
        return;
    }

    now = time(NULL);
    localInfo = localtime(&now);
    if (localInfo != NULL && strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localInfo) > 0)
    {
        logFile = fopen(DR_LOG_PATH, "a");
        if (logFile != NULL)
        {
            fprintf(logFile, "[%s] : %s\n", timeStr, message);
            fclose(logFile);
        }
    }
}

/*
* Name: removeDcAt
* Description: Removes a DC entry from the MasterList at the specified index.
* Parameters:
*  - master: Pointer to the MasterList structure containing DC entries.
*  - index: The index of the DC entry to remove.
* Returns: None
*/
void removeDcAt(MasterList* master, int index)
{
    int i;

    if (master == NULL || index < 0 || index >= master->numberOfDCs)
    {
        return;
    }

    for (i = index + 1; i < master->numberOfDCs; i++)
    {
        master->dc[i - 1] = master->dc[i];
    }

    master->numberOfDCs--;
}

/*
* Name: sleepOnePointFiveSec
* Description: Sleeps for 1.5 seconds between loop passes and picks back up if a signal interrupts the nap.
* Parameters: None
* Returns: None
  */
void sleepOnePointFiveSec(void)
{
    struct timespec req;

    req.tv_sec = 1;
    req.tv_nsec = 500000000L;

    while (nanosleep(&req, &req) == -1 && errno == EINTR)
    {
        // Keep sleeping whatever time is left
    }
}