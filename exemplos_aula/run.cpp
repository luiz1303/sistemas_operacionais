#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        cerr << "Error creating child process" << endl;
        return 1;
    }
    else if (pid == 0)
    {
        execlp("/bin/ls", "ls", NULL); // Execute the "ls" command
        cerr << "Error executing the command" << endl;
    }
    else
    {
        wait(NULL); // Wait for the child process to finish
        cout << "Parent: Child process completed" << endl;
    }

    return 0;
}
