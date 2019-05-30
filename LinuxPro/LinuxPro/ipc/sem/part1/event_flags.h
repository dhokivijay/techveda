/* 

   Header file for event_flags.c.

   The event flags operations are:

        set a flag:              setEventFlag(semId, semNum)
        clear a flag:            clearEventFlag(semId, semNum)
        wait for flag to be set: waitForEventFlag(semId, semNum)
        read a flag's value:     getFlagState(semId, semNum, &isSet)

   NB: The semantics of System V semaphores require that the "set"
   value for a flag is 0 and the "clear" value is 1.
*/
#ifndef EVENT_FLAGS_H
#define EVENT_FLAGS_H           /* Prevent accidental double inclusion */


int waitForEventFlag(int semId, int semNum);

int clearEventFlag(int semId, int semNum);

int setEventFlag(int semId, int semNum);

int getFlagState(int semId, int semNum, Boolean *isSet);

#endif
