#include <iostream>
#include <unistd.h>

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
        cout << "Child: Child process" << endl;
    }
    else
    {
        cout << "Parent: Parent process" << endl;
    }

    return 0;
}
