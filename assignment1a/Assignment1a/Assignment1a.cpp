#include <sys/types.h>   // for fork, wait
#include <sys/wait.h>    // for wait
#include <unistd.h>      // for fork, pipe, dup, close
#include <stdio.h>       // for NULL, perror
#include <stdlib.h>      // for exit
#include <iostream>      // for cout
using namespace std;

int main(int argc, char** argv) 
{
    int fds[2][2];
    int pid;
    if (argc != 2) 
    {
        cerr << "Usage: processes command" << endl;
        exit(-1);
    }
    // fork a child
    if ((pid = fork()) < 0) 
    {
        perror("fork error");
    }
    else if (pid == 0) 
    {
        // I'm a child
        pid = fork();
        // create a pipe using fds[0]
        pipe(fds[0]);
        // fork a grandchild
        pid = fork();
        // if I'm a grandchild
        if (pid == 0)
        {
            // create a pipe using fds[1]
            pipe(fds[1]);
            // fork a great grandchild
            pid = fork();
            // if I'm a great grandchild
            if (pid == 0)
            {
                //close(fds[0]);
                dup2(*fds[1], 1);
                // execute "ps"
                execlp("ps", "ps", "-A", NULL);
            }
            // else if I'm a grandchild
            else if (pid > 0)
            {
                dup2(*fds[0], 0);
                dup2(*fds[1], 1);
                // execute "grep"
                execlp("grep", "grep", "ssh", NULL);
            }
        }
        // else if I'm a child
        else if (pid == 0)
        {
            //close(fds[1]);
            dup2(*fds[0], 0);
            // execute "wc"
            execlp("wc", "wc", "-l", NULL);
        }
    }
    else 
    {
        // I'm a parent
        wait(NULL);
        cout << "commands completed" << endl;
    }
}