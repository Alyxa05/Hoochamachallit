/*
- File         : DR/inc/dr_helpers.h
- Project      : Hoochamacallit
- Programmers  : Alyx Deland, Josh Ducharme-Baribeau, Erik vanKooy
- First Version: 11/11/2026
- Description  : Helper declarations for the Data Reader (DR) module,
-                covering status text lookup, event logging, MasterList
-                removal/collapsing, and loop timing.
 */

#ifndef DR_HELPERS_H
#define DR_HELPERS_H
//Still think its crazy we are using the name hooch
#include "hooch.h"

#define DC_TIMEOUT_SECONDS 35.0
#define DR_LOG_PATH "/tmp/dataMonitor.log"

const char* getStatusText(int status);
void writeDrLog(const char* message);
void removeDcAt(MasterList* master, int index);
void sleepOnePointFiveSec(void);

#endif