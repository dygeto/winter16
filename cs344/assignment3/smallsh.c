/*****************************************
 ** Filename:    smallsh.c
 ** Author:      Dylan Tomlinson
 ** Date:        02/29/16
 **
 ** Description: This program is a shell that will run command line instructions
                 and return the results. It allows redirection of
 **              standard input and standard output and allows both
 **              foreground and background processes. It supports three
 **              built in commands: exit, cd, and status. It also supports
 **              comments, which are lines beginning with the # character.
 ******************************************/

#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>

#define DEBUG             0 //change to 1 for debugging 
#define MAX_ARGUMENTS   512 
#define MAX_PIDS       1000 
#define MAX_LENGTH     2048 

typedef enum { false, true } bool;

// declare global variables
int signalNum = 0;
int completedCurrent= 0;
int current= 0;
pid_t pidForeground = INT_MAX;     //running foreground process
pid_t pidBackground[MAX_PIDS];     //array of open background pids
pid_t pidCompleted[MAX_PIDS];      //array of completed background pids


//function prototypes
void handlerSigint();
void handlerBackground(int sig, siginfo_t* info, void* vp);

//where just about all of the magic happens
int main(int argc, char** argv)
{
	
    int status;
    int statusBackground; 
    int exitStatus;
    int exitStatusBackground;
    int fileDescriptor;
    int fileDescriptor2;
    int numArguments;
    char *token;
    char input[MAX_LENGTH];
    char *arguments[MAX_ARGUMENTS + 1];
    bool isBackgroundProcess = false;
    bool repeat = true;
    pid_t pidCurrent;

    //allocate memory for argument array
    int i = 0;
    for (; i <= MAX_ARGUMENTS; i++)
    {
        arguments[i] = (char *) malloc((MAX_LENGTH + 1) * sizeof(char)); 
    }

    //initialize arrays for background processes
    for (i = 0; i < MAX_PIDS; i++)
    {
        pidCompleted[i] = pidBackground[i] = INT_MAX;
    }   


    //creates sigaction structs for background and foreground
    //processes, as well as for when neither are running
    struct sigaction foreground_act;
    struct sigaction background_act;
    struct sigaction restOfTheTime_act;

    foreground_act.sa_handler = handlerSigint;
    foreground_act.sa_flags = SA_RESTART;
    sigfillset(&(foreground_act.sa_mask));
    sigaction(SIGINT, &foreground_act, NULL);
    
    background_act.sa_sigaction = handlerBackground;     
    background_act.sa_flags = SA_SIGINFO|SA_RESTART;
    sigfillset(&(background_act.sa_mask));

    //set up signal handler for completed child process
    sigaction(SIGCHLD, &background_act, NULL);
    
    restOfTheTime_act.sa_handler = SIG_IGN;
    restOfTheTime_act.sa_flags = SA_RESTART;
    sigfillset(&(restOfTheTime_act.sa_mask));
    sigaction(SIGINT, &restOfTheTime_act, NULL); 

      
    //loop that runs until the shell is closed
    do
    {
        //create array of pointers to the strings in the argument array
        char **next = arguments;

        for (i = 0; i <= MAX_ARGUMENTS; i++)
        {
            strcpy(arguments[i], "\n");
        }

        //clears input buffer
        strcpy(input, "\0");
        i = 0;

        //loop that cleans up zombies in pidCompleted
        while (i < MAX_PIDS && pidCompleted[i] != INT_MAX)
        {
            if (DEBUG) 
            {
                printf("Now cleaning up process %d\n", pidCompleted[i]);
            }

            //wait on current process
            pidCompleted[i] = waitpid(pidCompleted[i], &statusBackground, 0);

            //print pid and exit status
            if (WIFEXITED(statusBackground)) 
            {
                exitStatusBackground = WEXITSTATUS(statusBackground);
                printf("background pid %d is done: exit value %d.\n", pidCompleted[i], exitStatusBackground);
            }
            else
            {
                exitStatusBackground = WTERMSIG(statusBackground);
                printf("background pid %d is done: terminated by signal %d\n", pidCompleted[i], exitStatusBackground);
 
            }

            //removes current processs from pidBackground
            int j = 0;
            while (j < MAX_PIDS && pidBackground[j] != INT_MAX)
            {
		//searches array for match
                if (pidBackground[j] == pidCompleted[i])
                {
                    if (DEBUG)
                    {
                        printf("Now removing process %d from array.\n", pidBackground[j]);
                    }                   

                    //replaces value of current background process 
                    pidBackground[j] = INT_MAX;
 
                    //shifts pids to fill the gap
                    int k = j;                       
                    while (k + 1 < MAX_PIDS && pidBackground[k+1] != INT_MAX)
                    {
                        pidBackground[k] = pidBackground[k+1];
                        pidBackground[k+1] = INT_MAX;
                        k++;
                    }      
                    current--; 
                }
                j++;
            }

            //replace value of current completed process
            pidCompleted[i] = INT_MAX;
            i++; 
        }

        //reset completedCurrent (background process array counter)
        completedCurrent = 0;

        fflush(stdin);
        fflush(stdout);
        printf(": ");
        fgets(input, MAX_LENGTH, stdin);
        fflush(stdin);

        //check for blank lines, and NULL
        if (input[0] == '\n' || input[0] == '\0')
        {
            continue;
        }
 
        //process and parse input
        numArguments = 0;
        token = strtok(input, " "); //check for multiple arguments

        if (DEBUG)
        {
            printf("token is %s\n", token); 
        }

        //process arguments
        while (token != NULL && numArguments < MAX_ARGUMENTS)  
        {
 
            //ignore leading, trailing, and duplicate spaces 
            if (strlen(token) == 0)
            {
                continue;
            }   

            if (DEBUG)
            {
                printf("overwriting %s with %s\n", *next, token);
            }

            //copy current arg to arg array
            strcpy(*next, token);

            if (DEBUG)
            {
                printf("arguments[%d] is: %s\n", numArguments, arguments[numArguments]); 
            }

            numArguments++;
 
            //get next arg, if any
            token = strtok(NULL, " ");

            if (token != NULL)
            {
                *next++;
            } 
        }

        if (DEBUG)
        {
            printf("overwriting %s", *next);
        }

        //remove \n from last argument
        token = strtok(*next, "\n"); 
        if (token != NULL)
        {
            strcpy(*next, token);
        }

        if (DEBUG)
        {
            printf(" with %s\n", *next);
            printf("arguments[%d] is: %s\n", numArguments - 1, arguments[numArguments - 1]); 
        }
        
        //if it's a background process, set it as such
        if (strcmp(arguments[numArguments - 1], "&") == 0)
        {
            isBackgroundProcess = true;
            numArguments--; 
  
        }
        else
        {
            *next++;
        }

        if (DEBUG)
        {
            printf("overwriting %s with NULL\n", *next);
        }
		
        if (strncmp(arguments[0], "#", 1) == 0)
        {
        }
        else if (strcmp(arguments[0], "exit") == 0)
        {

            //kill any processes or jobs that shell has started
            i = 0;
            while (i < MAX_PIDS && pidBackground[i] != INT_MAX)
            {
                if (DEBUG)
                {
                    printf("Now killing process %d\n", pidBackground[i]);
                }
 
                kill(pidBackground[i], SIGKILL);
                i++;
            }

            //free allocated memory
            for (i = 0; i <= MAX_ARGUMENTS; i++)
            {
                if (DEBUG)
                {
                    printf("Now freeing memory for arguments[%d], which has a value of %s\n",i, arguments[i]);
                } 
                free(arguments[i]); 
            }  
            repeat = false;
        }
        else if (strcmp(arguments[0], "cd") == 0)
        {
            //if no arguments, change to HOME
            if (numArguments == 1)
            {
                chdir(getenv("HOME"));
            }
            //if one arg, change to dir provided
            else
            {
                chdir(arguments[1]);
            }
        }
        else if (strcmp(arguments[0], "status") == 0)
        {
            if (WIFEXITED(status))
            {
                exitStatus = WEXITSTATUS(status);
                printf("exit value %d\n", exitStatus);
            }
            else if (signalNum != 0)
            {
                printf("terminated by signal %d\n", signalNum);
            } 

        }
        else //pass through to BASH to interpret command there
        {
            pidCurrent = fork();
            if (pidCurrent == 0)
            {
                bool checkStatus = false; 
                bool redirectInput = false;
                bool redirectOutput = false;
                int inputOffset = 0;
                int outputOffset = 0;

                if (numArguments > 4 && strcmp(arguments[numArguments-4], "<") == 0)
                {
                    if (DEBUG)
                    {
                        printf("1) input redirected to %s\n", arguments[numArguments-3]);     
                    }

                    redirectInput = true;
                    inputOffset = 3; 
                }
                else if (numArguments > 2 && strcmp(arguments[numArguments-2], "<") == 0)
                {
                    if (DEBUG)
                    {
                        printf("2) input redirected to %s\n", arguments[numArguments-1]);     
                    }

                    redirectInput = true;
                    inputOffset = 1; 
                }
                if (numArguments > 4 && strcmp(arguments[numArguments-4], ">") == 0)
                {
                    if (DEBUG)
                    {
                        printf("3) output redirected to %s\n", arguments[numArguments-3]);     
                    }
 
                    redirectOutput = true;
                    //set target for output path
                    outputOffset = 3; 
                }
                else if (numArguments > 2 && strcmp(arguments[numArguments-2], ">") == 0)
                {
                    if (DEBUG)
                    {
                        printf("4) output redirected to %s\n", arguments[numArguments-1]);     
                    }

                    redirectOutput = true;
                    outputOffset = 1; 
                }

                //redirect stdin for background process to dev/null if no path provided
                if (isBackgroundProcess == true && redirectInput == false)
                {
                    fileDescriptor = open("/dev/null", O_RDONLY);

                    checkStatus = true;      
                }
                else if (redirectInput == true)
                {
                    fileDescriptor = open(arguments[numArguments - inputOffset], O_RDONLY);

                    checkStatus = true;  
                }

                if (checkStatus == true)
                {
                    if (fileDescriptor == -1)
                    {
                        printf("smallsh: cannot open %s for input\n", arguments[numArguments - inputOffset]);
                        exit(1); 
                    }

                    fileDescriptor2 = dup2(fileDescriptor, 0);
                    if (fileDescriptor2 == -1)
                    {
                        printf("smallsh: cannot open %s for input\n", arguments[numArguments - inputOffset]);
                        exit(1);
                    }   
                }

                if (redirectOutput == true)
                {
                    fileDescriptor = open(arguments[numArguments - outputOffset], O_WRONLY|O_CREAT|O_TRUNC, 0644);

                    if (fileDescriptor == -1)
                    {
                        printf("smallsh: cannot open %s for output\n", arguments[numArguments - outputOffset]);
                        exit(1); 
                    }

                    fileDescriptor2 = dup2(fileDescriptor, 1);
                    if (fileDescriptor2 == -1)
                    {
                        printf("smallsh: cannot open %s for output\n", arguments[numArguments - outputOffset]);
                        exit(1);
                    }   
                }

                //get the greater of the offsets, if any
                i = 0;
                if (inputOffset > outputOffset)
                {
                    i = inputOffset + 1;
                }
                else if (outputOffset > inputOffset)
                {
                    i = outputOffset + 1;
                }

                //move the pointer to omit the input redirection from array
                int j;
                for (j = i; j > 0; j--)
                {
                    *next--;
                }

                *next = NULL;

                //access built in Linux commands
                execvp(arguments[0], arguments);

                //for incorrect file name
                printf("%s", arguments[0]);
                fflush(NULL);
                perror(" ");  
 
                exit(1); //end child process
            }
            else if (pidCurrent == -1) //parent process
            {   
                //print error if it doesn't fork
                printf("%s", arguments[0]);
                fflush(NULL);                 
                perror(" ");
            } 
            else
            {
                //if command is bg process, print process id
                if (isBackgroundProcess == true)
                {
                    printf("background pid is %d\n", pidCurrent);
                    isBackgroundProcess = false;

                    //add pid to pidBackground array
                    if (current< MAX_PIDS)
                    {  
                        pidBackground[current++] = pidCurrent;
                    }
                } 
                else
                {
                    //reset value of signal number
                    signalNum = 0;                     

                    //assign pidCurrent to global variable
                    //for access in signal handlers  
                    pidForeground = pidCurrent;

                    //set interrupt handler for foreground process 
                    sigaction(SIGINT, &foreground_act, NULL);

                    //wait for foreground child process
                    pidForeground = waitpid(pidForeground, &status, 0);

                    //restore to ignore interrupts
                    sigaction(SIGINT, &restOfTheTime_act, NULL);

                    //reset global variable so signal handlers know
                    //there is no active foreground process
                    pidForeground = INT_MAX;

                    //if process was terminated by signal, print message
                    if (signalNum != 0)
                    {
                        printf("terminated by signal %d\n", signalNum);
                    }   
                }
            }
        }

    }
    while(repeat == true);

    return 0;
}

/******************************************************************************
 ** Function: handlerSigint()
 ** Description: handles the interrupt signals SIGINT and ctrl+C. If 
 **              these signals occur while a foreground child process is running,
 **              handlerSigint kills it. Then sets a flag for a relevant message
 **              to be displayed.
 ******************************************************************************/
void handlerSigint()
{
    //if the interrupt signal occurs while foreground process is 
    //running, kill it. Then change the signalNum
    if (pidForeground != INT_MAX)
    {
        kill(pidForeground, SIGKILL);
        signalNum = 2;  
    }  

    return;
}

/******************************************************************************
 ** Function: handlerBackground()
 ** Description: handles SIGCHLD signals. Stores the PID of background
 **              child processes into an array if the signal is recieved.
 **              This is for cleaning later.                    
 ******************************************************************************/
void handlerBackground(int sig, siginfo_t* info, void* vp)
{
    if (DEBUG)
    {
        printf("In handlerBackground.\n");
    }

    pid_t pidReference = info->si_pid; 

    //if the signal is from a background process, add it to the
    //pidCompleted array
    if (pidReference != pidForeground && completedCurrent < MAX_PIDS)
    {
        
        pidCompleted[completedCurrent++] = pidReference;
    } 

    return;
}
