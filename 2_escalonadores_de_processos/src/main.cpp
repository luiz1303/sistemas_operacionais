#include "headers.h"

int main() {
    srand(static_cast<unsigned>(time(NULL)));

    int option = -1;
    list<Process> processes;

    createNamedProcess(processes, "P1", 0, 10, 3);
    createNamedProcess(processes, "P2", 1, 1, 1);
    createNamedProcess(processes, "P3", 2, 2, 3);
    createNamedProcess(processes, "P4", 3, 1, 4);
    createNamedProcess(processes, "P5", 4, 5, 2);

    clearConsole();

    while (option != 0) {
        cout << "\033[34mProcess scheduler Simulator\033[0m" << endl;
        cout << "0 - Exit" << endl;
        cout << "1 - List Processes" << endl;
        cout << "2 - FCFS - First Come First Served" << endl;
        cout << "3 - SJF - Shortest Job First" << endl;
        cout << "4 - Prioridades" << endl;
        cout << "5 - RR" << endl;
        cout << "\033[34mChoose an option >\033[0m ";
        cin >> option;
        clearConsole();

        switch (option) {
            case 1:
                printProcesses(processes);
                break;
            case 2:
                runFCFS(processes);
                break;
            case 3:
                runSJF(processes);
                break;
            case 4:
                runPriorities(processes);
                break;
            case 5:
                runRoundRobin(processes);
                break;
            case 0:
                cout << "\033[1;32mExiting...\033[0m" << endl;
                break;

            default:
                cout << "\033[1;31mERROR: Choose a valid option\033[0m" << endl;
                break;
        }
    }

    return 0;
}
