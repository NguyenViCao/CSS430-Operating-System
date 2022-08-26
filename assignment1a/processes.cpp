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
        // create a pipe using fds[0]
        if(pipe(fds[0]) < 0)
            perror("pipe not created");
        else
            cout << "child pipe" << endl;
        // fork a grandchild
        pid = fork();
        if(pid < 0)
            perror("fork error");
        // if I'm a grandchild
        else if (pid == 0)
        {
            // create a pipe using fds[1]
            if(pipe(fds[1]) < 0)
                perror("pipe not created");
            else
                cout << "grandchild pipe" << endl;
            // fork a great grandchild
            pid = fork();
            if(pid < 0)
                perror("fork error");
            // if I'm a great grandchild
            else if(pid == 0)
            {
                dup2(fds[1][1], STDOUT_FILENO);
                close(fds[1][0]);
                close(fds[1][1]);
                // execute "ps"
                execlp("ps", "ps", "-A", NULL);
            }
            // else if I'm a grandchild
            else if (pid > 0)
            {
                dup2(fds[0][0], STDIN_FILENO);
                dup2(fds[1][1], STDOUT_FILENO);
                close(fds[0][0]);
                close(fds[0][1]);
                close(fds[1][0]);
                close(fds[1][1]);
                // execute "grep"
                execlp("grep", "grep", "ssh", NULL);
            }
        }
        // else if I'm a child
        else if (pid == 0)
        {
            dup2(fds[0][0], STDIN_FILENO);
            close(fds[0][0]);
            close(fds[0][1]);
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