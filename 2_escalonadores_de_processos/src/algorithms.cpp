#include "headers.h"

float getProcessWaitingTime(list<Process> processList, Process currentProcess) {
    list<Process>::iterator it = processList.begin();
    float waitingTime = 0;

    while (it != processList.end()) {
        if (it->pid != currentProcess.pid) {
            waitingTime += it->executionTime;
        } else {
            return waitingTime;
        }
        ++it;
    }

    return 0;
}

float getAvgWaitingTime(list<Process> processes) {
    list<Process>::iterator it;
    float avgWaitingTime = 0;

    for (it = processes.begin(); it != processes.end(); ++it) {
        Process currentProcess = *it;
        avgWaitingTime += getProcessWaitingTime(processes, currentProcess);
    }

    return (float)avgWaitingTime / (float)processes.size();
}

int generatePID() {
    int topLimit = 300;
    int pid = (rand() % topLimit) + 100;

    return abs(pid);
}

Process createProcess(list<Process> &processes, int arrivalTime,
                      int executionTime, int priority) {
    Process newProcess;
    newProcess.pid = generatePID();
    newProcess.priority = priority;
    newProcess.arrivalTime = arrivalTime;
    newProcess.executionTime = executionTime;
    processes.push_back(newProcess);
    return newProcess;
}

Process createNamedProcess(list<Process> &processes, string name,
                           int arrivalTime, int executionTime, int priority) {
    Process newProcess;
    newProcess.pid = generatePID();
    newProcess.priority = priority;
    newProcess.name = name;
    newProcess.arrivalTime = arrivalTime;
    newProcess.executionTime = executionTime;
    processes.push_back(newProcess);
    return newProcess;
}

void removeProcess(list<Process> &processes, int pid) {
    list<Process>::iterator it;

    for (it = processes.begin(); it != processes.end(); ++it) {
        if (it->pid == pid) {
            processes.erase(it);
            cout << "\033[1;32mRemovido processo PID: \033[0m" << pid << endl;
            break;
        }
    }
}

void printProcesses(list<Process> processes) {
    string spacing = "\t";
    list<Process>::iterator it;
    cout << "\033[32mLista de Processos\033[0m" << endl;
    cout << "======================================================" << endl;

    for (it = processes.begin(); it != processes.end(); ++it) {
        Process currentProcess = *it;
        cout << "PID: " << currentProcess.pid << spacing;
        cout << "Name: " << currentProcess.name << spacing;
        cout << "Priority: " << currentProcess.priority << spacing;
        cout << "arrivalTime: " << currentProcess.arrivalTime << spacing;
        cout << "executionTime: " << currentProcess.executionTime << endl;
    }
    cout << "======================================================" << endl;
}

void renderGanttChart(list<Process> processes) {
    list<Process>::iterator it;
    string waitChar = "-";
    string activeChar = "#";

    cout << "====================== Gantt Chart ======================" << endl;

    for (it = processes.begin(); it != processes.end(); ++it) {
        Process currentProcess = *it;
        string spacing = "\t";
        int currentWaitingTime =
            getProcessWaitingTime(processes, currentProcess);

        cout << "PID: " << currentProcess.pid << spacing;
        cout << "name: " << currentProcess.name << spacing;
        cout << "Start: " << currentWaitingTime << spacing;

        for (int i = 0; i < currentWaitingTime; i++) {
            cout << waitChar;
        }

        for (int i = 0; i < currentProcess.executionTime; i++) {
            cout << activeChar;
        }

        cout << endl;
    }

    cout << "=========================================================" << endl;
    cout << "\033[31mAverage Waiting Time: \033[0m"
         << getAvgWaitingTime(processes) << endl;
}

bool compareByArrival(const Process &process1, const Process &process2) {
    return process1.arrivalTime < process2.arrivalTime;
}

bool compareByPriority(const Process &process1, const Process &process2) {
    return process1.priority < process2.priority;
}

bool compareByDuration(const Process &process1, const Process &process2) {
    return process1.executionTime < process2.executionTime;
}

void runFCFS(list<Process> processes) {
    processes.sort(compareByArrival);
    cout << "\033[32mFirst Come First Served (FCFS)\033[0m" << endl;
    renderGanttChart(processes);
}

void runSJF(list<Process> processes) {
    processes.sort(compareByDuration);
    cout << "\033[32mShortest Job First (SFJ)\033[0m" << endl;
    renderGanttChart(processes);
}

void runPriorities(list<Process> processes) {
    processes.sort(compareByPriority);
    cout << "\033[32mPriorities\033[0m" << endl;
    renderGanttChart(processes);
}

bool checkProcessState(list<int> executionIntervals, int target) {
    list<int>::iterator it;

    for (it = executionIntervals.begin(); it != executionIntervals.end();
         ++it) {
        if (*it == target) {
            return true;
        }
    }

    return false;
}

int getTimeStamp(list<Process> processes) {
    list<Process>::iterator it;
    int timestamp = 0;

    for (it = processes.begin(); it != processes.end(); ++it) {
        timestamp += it->executionTime;
    }

    return timestamp;
}

void renderRRChart(list<Process> processes) {
    list<Process>::iterator it;
    int timestamp = getTimeStamp(processes);
    string waitChar = "-";
    string activeChar = "#";

    cout << "\033[32mRound Robin (RR)\033[0m" << endl;
    cout << "====================== Gantt Chart ======================" << endl;
    for (it = processes.begin(); it != processes.end(); ++it) {
        Process currentProcess = *it;

        cout << "PID: " << currentProcess.pid << '\t';
        cout << "name: " << currentProcess.name << '\t';

        for (int timer = 0; timer < timestamp; timer++) {
            bool isActive = checkProcessState(it->executionIntervals, timer);
            if (isActive) {
                cout << activeChar;
            } else {
                cout << waitChar;
            }
        }
        cout << endl;
    }
    cout << "=========================================================" << endl;
}

// Round Robin para quantum = 1u.t.
void runRoundRobin(list<Process> processes) {
    list<Process> processQueue = processes;
    int timer = 0;
    int timestamp = getTimeStamp(processQueue);
    long unsigned int position = 0;

    vector<int> waitTime(processes.size(), 0);

    list<Process>::iterator it = processQueue.begin();
    list<Process>::iterator it2 = processes.begin();

    while (timer < timestamp) {
        Process &currentProcess = *it;
        Process &equivalentProcess = *it2;

        if (currentProcess.executionTime > 0) {
            currentProcess.executionTime--;
            equivalentProcess.executionIntervals.push_back(timer);

            for (long unsigned int i = 0; i < waitTime.size(); i++) {
                if (i != position) {
                    waitTime[i]++;
                }
            }
        }

        // Circula entre os elementos da lista
        if (position == processQueue.size() - 1) {
            position = 0;
            it = processQueue.begin();
            it2 = processes.begin();
        } else {
            position++;
            it++;
            it2++;
        }

        timer++;
    }

    float avgWaitTime = 0;
    for (long unsigned int i = 0; i < waitTime.size(); i++) {
        avgWaitTime += waitTime[i];
    }

    avgWaitTime = avgWaitTime / waitTime.size();
    renderRRChart(processes);
    cout << "\033[31mAverage Waiting Time: \033[0m" << avgWaitTime << endl;
}

// Sobrecarda de Round Robin com quantum customizável
void runRoundRobin(list<Process> processes, int quantum) {
    list<Process> processQueue = processes;
    int timer = 0;
    unsigned long int position = 0;
    bool shouldContinue = true;

    list<Process>::iterator it = processQueue.begin();
    list<Process>::iterator it2 = processes.begin();

    while (shouldContinue) {
        Process &currentProcess = *it;
        Process &equivalentProcess = *it2;

        if (currentProcess.executionTime > 0) {
            if (currentProcess.executionTime < quantum) {
                for (int i = timer; i <= timer + currentProcess.executionTime;
                     i++) {
                    currentProcess.executionIntervals.push_back(i);
                    equivalentProcess.executionIntervals.push_back(i);
                }

                timer += currentProcess.executionTime;

            } else {
                for (int i = timer; i <= timer + quantum; i++) {
                    currentProcess.executionIntervals.push_back(i);
                    equivalentProcess.executionIntervals.push_back(i);
                }
                currentProcess.executionTime -= quantum;
                timer += quantum;
            }
        }

        // Circula entre os elementos da lista
        if (position == processQueue.size() - 1) {
            position = 0;
            it = processQueue.begin();
            it2 = processes.begin();
        } else {
            position++;
            it++;
            it2++;
        }

        shouldContinue = false;
        list<Process>::iterator it3;
        for (it3 = processQueue.begin(); it3 != processQueue.end(); ++it3) {
            if (it3->executionTime > 0) {
                shouldContinue = true;
                break;
            }
        }
    }

    renderRRChart(processes);
}

void clearConsole() {
    system("cls");
    system("clear");
}