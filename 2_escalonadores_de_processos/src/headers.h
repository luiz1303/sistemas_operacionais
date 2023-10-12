#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

typedef struct Process Process;

struct Process {
    int pid;
    string name;
    int priority;
    int arrivalTime;
    int executionTime;
    list<int> executionIntervals;
};

float getProcessWaitingTime(list<Process> processList, Process currentProcess);
float getAvgWaitingTime(list<Process> processes);
int getTimeStamp(list<Process> processes);
int generatePID();

bool compareByArrival(const Process &process1, const Process &process2);
bool compareByPriority(const Process &process1, const Process &process2);
bool compareByDuration(const Process &process1, const Process &process2);
bool checkProcessState(list<int> executionIntervals, int target);

Process createProcess(list<Process> &processes, int arrivalTime,
                      int executionTime, int priority);

Process createNamedProcess(list<Process> &processes, string name,
                           int arrivalTime, int executionTime, int priority);

void removeProcess(list<Process> &processes, int pid);
void printProcesses(list<Process> processes);
void renderGanttChart(list<Process> processes);
void renderRRChart(list<Process> processes);

void runFCFS(list<Process> processes);
void runSJF(list<Process> processes);
void runPriorities(list<Process> processes);
void runRoundRobin(list<Process> processes);
void runRoundRobin(list<Process> processes, int quantum);

void clearConsole();