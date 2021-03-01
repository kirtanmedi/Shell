//#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printArray(char **array){
    int i = 0;
    while(array[i] != NULL){
        printf("%d: %s ", i, array[i]);
        i++;
    }
    printf("\n");
}

//prints the prompt before every command
void printPrompt(){
    char dir[1024];
    getcwd(dir, sizeof(dir));
    printf("%s>", dir);
}

//checks if "exit" or "quit" is entered
void checkExit(const char *inputLine){
    char exitInput[] = "exit";
    char quitInput[] = "quit";
    if(strcmp(inputLine, exitInput) == 10 || strcmp(inputLine, quitInput) == 10){
        exit(0);
    }
}

#define MAX_INPUT_LENGTH 1024

//reads in the line of commands
char *readLine(){
    char *inputLine = malloc(sizeof(char) * MAX_INPUT_LENGTH);
    size_t inputLength = 0;
    getline(&inputLine, &inputLength, stdin);

    if (inputLength > MAX_INPUT_LENGTH) {
        exit(EXIT_FAILURE);
    }

    checkExit(inputLine);

    return inputLine;
}

#define MAX_NUM_ARGUMENTS 64

//parse the input and figure out which type of execution to run
char **parseInput(char *inputLine){
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
int processInput(char **argList){
    //if pipe command return 2
    const char pipe[] = "|";
    if (strcmp(argList[1], pipe) == 0) {
        return 2;
    }

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

    return 1;
}

//the main looping logic of the shell
void shell(){
    char *inputLine;
    char **argList;
    int commandType = 1;

    while(1){
        printPrompt();
        inputLine = readLine();
        argList = parseInput(inputLine);
        commandType = processInput(argList);
        printf("%d", commandType);
        free(inputLine);
    }
}

//main method
int main(int argc, char **argv){
    shell();

    return 0;
}