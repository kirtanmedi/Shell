//#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printArray(char **array) {
    int i = 0;
    while (array[i] != NULL) {
        printf("%d: %s ", i, array[i]);
        i++;
    }
    printf("\n");
}

//prints the prompt before every command
void printPrompt() {
    char dir[1024];
    getcwd(dir, sizeof(dir));
    printf("%s>", dir);
}

//checks if "exit" or "quit" is entered
void checkExit(const char *inputLine) {
    char exitInput[] = "exit";
    char quitInput[] = "quit";
    if (strcmp(inputLine, exitInput) == 10 || strcmp(inputLine, quitInput) == 10) {
        exit(0);
    }
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

    //if shell command return 1
    if(argList[1] == NULL){
        return 1;
    }

    //if pipe command return 2
    const char pipe[] = "|";
    if (strcmp(argList[1], pipe) == 0) {
        return 2;
    }
}

void helpSh() {
    printf("This the help menu\n"
           "Built in commands: help, cd, exit, quit\n");
}

void cdSh(char *newDir) {
    if(newDir == NULL){
        printf("please specify a directory\n");
        return;
    }

    if(chdir(newDir) != 0){
        printf("%s is not a valid directory\n", newDir);
    }
}

void exitSh() {
    exit(0);
}

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

void runSh(char **argList) {
    printf("this is shell\n");
}

void runPipe(char **argList) {
    printf("this is pipe\n");
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
        inputLen = strlen(inputLine);
        inputLine[inputLen-1] = 0;
        argList = parseInput(inputLine);
        commandType = processInput(argList);
        if (commandType == 0) {           //to run built in commands
            runBuiltIn(argList);
        } else if (commandType == 1) {    //to run other shell commands
            runSh(argList);
        } else if (commandType == 2) {    //to run piped commands
            runPipe(argList);
        }
        free(inputLine);
        free(argList);
    }
}

//main method
int main(int argc, char **argv) {
    shell();

    return 0;
}