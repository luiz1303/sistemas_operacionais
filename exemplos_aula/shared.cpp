#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    int shared_data = 5;
    pid_t pid = fork();

    if (pid < 0)
    {
        cerr << "Error creating child process" << endl;
        return 1;
    }
    else if (pid == 0)
    {
        shared_data += 10;
        cout << "Child: Shared value = " << shared_data << endl;
    }
    else
    {
        wait(NULL); // Wait for the child process to finish
        cout << "Parent: Shared value = " << shared_data << endl;
    }

    return 0;
}
