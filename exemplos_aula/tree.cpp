#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    cout << "Root process (PID: " << getpid() << ")" << endl;
    fork();
    fork();
    fork();
    cout << "PID: " << getpid() << ", PPID: " << getppid() << endl;

    return 0;
}
