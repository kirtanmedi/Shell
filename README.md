This program is a simple shell which repeatedly takes in user input to execute 
simple and complex commands. 

To compile the file, enter the following command into the UTD linux server:
    gcc -o shell shell.c 
    
After compilation, to run the program, enter the following command:
    ./shell
    
 This will initiate the program.
 
 There are four built in commands. These can be accessed by typing:
    help
    
cd is a built in commands which will allow you to change directories.

Other than the four built in commands, regular shell commands can also be executed, for example:
    grep -c char shell.c
    
In order to pipe two simple commands, enter the command as you would in the linux terminal, for exmaple:
    ls -l | wc -l 
can be entered to pipe the two commands.

To quit the shell program, type any of the following commands: 
    exit
    quit
or press control + d on the keyboard.