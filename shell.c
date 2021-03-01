//#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void printPrompt(){
    char dir[1024];
    getcwd(dir, sizeof(dir));
    printf("%s>", dir);
}

void checkExit(const char *inputLine){
    char exitInput[] = "exit";
    char quitInput[] = "quit";
    if(strcmp(inputLine, exitInput) == 10 || strcmp(inputLine, quitInput) == 10){
        exit(0);
    }
}

#define MAX_INPUT_LENGTH 1024

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

void shell(){
    char *inputLine;
    char **argList;

    while(1){
        printPrompt();
        inputLine = readLine();
    }
}

int main(int argc, char **argv){
    shell();

    return 0;
}