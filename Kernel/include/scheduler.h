#include <process.h>
#include <stdint.h>
#include "kalloc.h"
#include "output.h"

 struct llnode{
 	struct llnode * next;
 	struct llnode * prev;
 	Process process;
 };

 struct schedulerLL{
 	struct llnode * current;
 	uint64_t size;	
 };

 struct scheduler{
 	struct schedulerLL * waitingpq;
 	struct schedulerLL * blockedpq;
 };

typedef struct llnode * LLnode; 
typedef struct schedulerLL * SchedulerLL;
typedef struct scheduler * Scheduler;
 

void schedulerInit(void);
Process schedule(void);
SchedulerLL queueInit(void);
void printProcesses(void);
int addProcess(Process p, SchedulerLL q);
Process removeProcess(uint64_t pid, SchedulerLL q);
Process getCurrentWaiting();
Process getCurrentBlocked();
int addProcessWaiting(Process p);
int addProcessBlocked(Process p);
Process getCurrent(SchedulerLL q);
int getCurrentPid();
Process removeProcessWaiting(uint64_t pid);
Process removeProcessBlocked(uint64_t pid);
Process getProcess(int pid);
int isWaiting(int pid);
int isBlocked(int pid);
void endProcess();
int killProcess(int pid);
int blockProcess(int pid);
int unblockProcess(int pid);
Process getLastProcess();