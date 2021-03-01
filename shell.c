#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//built in help command
void helpSh() {
    printf("This the help menu\n"
           "Built in commands: help, cd, exit, quit\n"
           "Other commands can be used by simply typing them and pressing enter\n");
}

//built in cd command
void cdSh(char *newDir) {
    if (newDir == NULL) {
        printf("Please enter a file or directory.\n");
        return;
    }

    if (chdir(newDir) != 0) {
        perror("Error:");
    }
}

//built in exit command
void exitSh() {
    exit(0);
}

//run built in commands
void runBuiltIn(char **argList) {
    const char *builtInCommands[4];
    builtInCommands[0] = "help";
    builtInCommands[1] = "cd";
    builtInCommands[2] = "exit";
    builtInCommands[3] = "quit";

    if (strcmp(argList[0], builtInCommands[0]) == 0) {          //run the help command
        helpSh();
    } else if (strcmp(argList[0], builtInCommands[1]) == 0) {   //run the cd command
        cdSh(argList[1]);
    } else if (strcmp(argList[0], builtInCommands[2]) == 0) {   //exit
        exitSh();
    } else if (strcmp(argList[0], builtInCommands[3]) == 0) {   //exit
        exitSh();
    }
}

//run regular shell commands
void runSh(char **argList) {
    pid_t pid = fork();                                          //spawn new process

    if (pid == -1) {
        perror("Error:");
    } else if (pid == 0) {                                       //execute commands from child process
        if (execvp(argList[0], argList) == -1) {
            printf("%s: command not found...\n", argList[0]);
        }
        exit(0);
    } else {
        wait(NULL);
    }
}

//run piped commands
void runPipe(char **argList) {
    char **command1 = malloc(sizeof(char *) * 2);                //creating first command array
    char **command2 = malloc(sizeof(char *) * 2);                //creating second command array
    const char pipeChar[] = "|";
    int i = 0;
    int j = 0;

    while(strcmp(argList[i], pipeChar) != 0) {
        command1[i] = argList[i];
        i++;
    }

    i++;

    while (argList[i] != NULL) {
        command2[j] = argList[i];
        i++;
        j++;
    }

    //initialize the pipes
    int fd[2];
    pid_t pid;

    //check for piping errors
    if(pipe(fd) == -1){
        perror("Error:");
        exit(1);
    }

    //first pipe from first process
    if (fork() == 0) {
        close(STDOUT_FILENO);
        dup(fd[1]);
        close(fd[0]);
        close(fd[0]);

        if (execvp(command1[0], command1) == -1) {
            printf("%s: command not found...\n", argList[0]);
        }
        exit(1);
    }

    //second pipe from second process
    if (fork() == 0) {
        close(STDIN_FILENO);
        dup(fd[0]);
        close(fd[1]);
        close(fd[0]);

        if (execvp(command2[0], command2) == -1) {
            printf("%s: command not found...\n", argList[0]);
        }
        exit(1);
    }

    //close botch pipes and wait for processes to complete
    close(fd[0]);
    close(fd[1]);
    wait(0);
    wait(0);

    free(command1);
    free(command2);
}

//prints the prompt before every command
void printPrompt() {
    char dir[1024];
    getcwd(dir, sizeof(dir));
    printf("%s>", dir);
}

#define MAX_INPUT_LENGTH 1024

//reads in the line of commands
char *readLine() {
    char *inputLine = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    size_t inputLength = 0;
    getline(&inputLine, &inputLength, stdin);

    if (inputLength > MAX_INPUT_LENGTH) {
        exit(EXIT_FAILURE);
    }

    //checkExit(inputLine);

    return inputLine;
}

#define MAX_NUM_ARGUMENTS 64

//parse the input and figure out which type of execution to run
char **parseInput(char *inputLine) {
    char **argList = malloc(sizeof(char *) * MAX_NUM_ARGUMENTS);
    char *argument;
    int i = 0;
    const char delim[2] = " ";

    argument = strtok(inputLine, delim);

    while (argument != NULL) {
        argList[i] = argument;
        i++;
        argument = strtok(NULL, delim);
    }

    argList[i] = NULL;
    return argList;
}

//process input to figure out which type of execution is needed
int processInput(char **argList) {
    //if built in commands
    const char *builtInCommands[4];
    int i;
    builtInCommands[0] = "help";
    builtInCommands[1] = "cd";
    builtInCommands[2] = "exit";
    builtInCommands[3] = "quit";
    for (i = 0; i < 4; i++) {
        if (strcmp(argList[0], builtInCommands[i]) == 0) {
            return 0;
        }
    }

    //if pipe command return 2
    i = 0;
    const char pipe[] = "|";
    while (argList[i] != NULL) {
        if (strcmp(argList[i], pipe) == 0) {
            return 2;
        }
        i++;
    }

    //if shell command return 1
    return 1;
}

//the main looping logic of the shell
void shell() {
    char *inputLine;
    char **argList;
    int commandType = 1;
    int inputLen;

    while (1) {
        printPrompt();
        inputLine = readLine();

        //removing \n character from input line
        inputLen = strlen(inputLine);
        inputLine[inputLen - 1] = 0;

        argList = parseInput(inputLine);  //getting array of arguments

        //getting the correct command type and running it
        commandType = processInput(argList);
        if (commandType == 0) {           //to run built in commands
            runBuiltIn(argList);
        } else if (commandType == 1) {    //to run other shell commands
            runSh(argList);
        } else if (commandType == 2) {    //to run piped commands
            runPipe(argList);
        }

        //freeing space of inputLine and argList
        free(inputLine);
        free(argList);
    }
}

//main method
int main(int argc, char **argv) {
    shell();

    return 0;
}