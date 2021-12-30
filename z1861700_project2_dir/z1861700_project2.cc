/********************************************************************
CSCI 480 - Assignment 02 - Fall, 2021

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: September 27th, 2021

   Purpose: The purpose of this assignment is to create a shell script 
            and practice the system calls, open, read, write, close 
            fork, exit, wait, pipe. This program run commands on the 
            command line along with redireciton. 

   Example: Command line >> ls 
                         >> ls -la
                         >> ls > filename
                         >> ls >filename
                         >> fcfs 
*********************************************************************/
#include <ctime>
#include <math.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstdlib>
#include <iostream>
#include <iostream> 
#include <signal.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>

// Defining max input characters 
#define MAX_INPUT_CHAR 256

// White space in command line [blank space, new lines]
#define WHITESPACE(x) (x == ' '|| x == '\n')    
#define VALIDCOMMANDS(x) (x == '>')// Redirection I/O

// Microshell status during argument processe
#define STATS_DONE 0            // status: Done
#define STATS_ERROR 1           // status: Error 
#define STATS_REDIRECTION_OUT 2 // status: Output Redireciton 

// Creating an alias -> For: What user enters at command line
typedef char* arguments_t;

// Creating a alias struct that represents status 
typedef struct av_command
{
    arguments_t *arg_list;
    int out_fd;                 
    int stop_fail;             
    int stop_success;     
    int exe_second;    
    struct av_command *next;
} avcommand_t;

// ***************************** INITIALIZATION OF FUNCTIONS **********************************************
int Process_Arguments(const char *input, int *arg_charters, char *arg_buffer, int *arg_count, arguments_t *arg_list, int *pause_reason);
int Process_OutputRedireciton(const char *input, char *arg_buffer, arguments_t *arg_list, avcommand_t *c);
int Process_Execution(const avcommand_t *execute, int *count_commands);
int First_Command_Execution(const avcommand_t *command);
int Second_Command_Execution(const avcommand_t *Left, const avcommand_t *Right);
int func_fcfs();

/*********************************************************************************************************
* Function Name: main                                                                                    *
*     Arguments: none                                                                                    *            
*   Description: This function initlizes the given variables (input, argume-                             * 
*                nets list, buffer, commands). It also goes through a while                              *
*                that will call the necessary functions base on what the user                            *
*                has enter. It will also, diplay a prompts for user input.                               *
*                                                                                                        *
*       Example: Command line -> myshell>> [any LUNIX command] > output redirection (Any file name)      *
*********************************************************************************************************/
int main(void)
{
    // Declaring necessary variables
    int num_commands, count_commands, skip, execute_count, stats_exit, y, initil_start = 1;
    char arg_buffer[MAX_INPUT_CHAR];        // Argument Buffer 
    char input[MAX_INPUT_CHAR];             // Input 
    arguments_t arg_list[MAX_INPUT_CHAR];   // Argument List 
    avcommand_t commands[MAX_INPUT_CHAR];   // Commands 

    // Initial header 
	for(y = 1; y <= initil_start; y++)
	{
        std::cout << "Linux turing 4.19.0-17-amd64 #1 SMP Debian 4.19.194-3 (2021-07-18) x86_64" << std::endl << std::endl;
        std::cout << "The programs included with the Debian GNU/Linux system are free software;" << std::endl;
        std::cout << "the exact distribution terms for each program are described in the"        << std::endl;
        std::cout << "individual files in /usr/share/doc/*/copyright."                           << std::endl << std::endl;
        std::cout << "Debian GNU/Linux comes with ABSOLUTELY NO WARRANTY, to the extent"         << std::endl;
        std::cout << "permitted by applicable law." << std::endl;        
	}// End for()
    
    while(1)
    {
        // Setting ptr to each specific variable
        memset(input, 0, MAX_INPUT_CHAR);
        memset(arg_buffer, 0, MAX_INPUT_CHAR);
        memset(arg_list, 0, MAX_INPUT_CHAR * sizeof(arguments_t));
        memset(commands, 0, MAX_INPUT_CHAR * sizeof(avcommand_t));
        
        // Prompting user
        printf("myshell> ");
        fgets(input, MAX_INPUT_CHAR, stdin);// Storing user input to input variable

        // if loop, that test logic to whatever the user has enter
        if(strlen(input) > 0)
        {
            count_commands = 0;
            num_commands = Process_OutputRedireciton(input, arg_buffer, arg_list, commands);
            for (execute_count = 0; execute_count < num_commands; ++execute_count)
            {
                stats_exit = Process_Execution(commands + count_commands, &skip);
                count_commands += skip;
            }//End for
        }//End if 
    }// End while
    return 0;
}

/*********************************************************************************************************
* Function Name: Process_Arguments                                                                       *
*     Arguments: arg_charters -> Number of arguments                                                     *
*                arg_buffer   -> buff that stores all args                                               *
*                arg_list     -> argument list                                                           *
*                pause_reason -> Why it stop                                                             *
*   Description: This function will clean and will specify the command line args. From the given input   *
*                that the user has enter. It breaks when it finds backslashes, single/double quotes,     *
*                escape charaters, and it specifies logic for spaces found.                              *
*        Return: processing_task                                                                         *   
*********************************************************************************************************/
int Process_Arguments(const char *input, int *arg_charters, char *arg_buffer, int *arg_count, arguments_t *arg_list, int *pause_reason)
{  
    int escape = 0, quote = 0, processing_task = 0, current_argument_length = 0, agument_balance = 0;
    char quote_char = 0;
    const char *x = input;
    char current_arg[MAX_INPUT_CHAR]; // Current arguments 
    
    // Setting the current arguments 
    memset(current_arg, 0, MAX_INPUT_CHAR);
    *arg_count = 0;
    
    while(1)
    {
        if(*x == 0)
        {
            *pause_reason = STATS_DONE; 
            break;
        } // End 1st if()

        // Checking for backslash's 
        if(*x == '\\' && !escape) 
        {
            escape = 1;
        }// End 2nd if()

        // Checking for single/double quotes
        else if((*x == '\'' || *x == '"') && !escape) 
        {
            if(!quote)
            {
                quote_char = *x;
                quote = 1;
            }// End if inside else if()

            else if(*x == quote_char)
            {
                quote = 0;
                quote_char = 0;
            }// End if inside else if()
        }// End else if()

        else if((WHITESPACE(*x) || VALIDCOMMANDS(*x)) && !(escape || quote))
        {
            if(current_argument_length > 0)
            {
                memcpy((arg_buffer + agument_balance), current_arg, current_argument_length);// Copying memory from current arguments 
                arg_list[(*arg_count)++] = (arg_buffer + agument_balance);
                agument_balance += (current_argument_length + 1);                            // AND
                memset(current_arg, 0, current_argument_length);                             // Setting ptr to the current argument
                current_argument_length = 0;

                // Given spaces 
                while (WHITESPACE(*x))
                {
                    ++x;
                    ++processing_task;
                }
                
                // Output Redirection 
                if(*x == '>')
                {
                    *pause_reason = STATS_REDIRECTION_OUT;
                    break;
                } 
                else 
                {
                    --x;
                    --processing_task;
                }
            }
            // Return error to io stream
            else if (VALIDCOMMANDS(*x))
            {
                *pause_reason = STATS_ERROR;
                break;
            }
        }// End else if... 
        else 
        {
            current_arg[current_argument_length++] = *x;
            escape = 0;
        }
        ++x;
        ++processing_task;
    }// End while..  

    ++processing_task; 
    *arg_charters = agument_balance;

    return processing_task;

}// End Process_Arguments 

 /********************************************************************************************************
* Function Name: Process_OutputRedireciton                                                               *
*     Arguments: arg_buffer    -> buff that stores all args                                              *
*                arg_list      -> argument list                                                          *
*                c             -> array of commands                                                      *
*   Description: This function will create the logic of output redirection. If a file doesn't it exist,  *
*                it will be created. It also returns the necessary error messages to user and stream     *
*        Return: execute_count -> count of execution                                                     *
*********************************************************************************************************/
int Process_OutputRedireciton(const char *input, char *arg_buffer, arguments_t *arg_list, avcommand_t *c)
{
    // Initializing necessary variables 
    int arg_count = 0, arg_total = 0, arg_charters = 0, arg_chartersTotal = 0, in_possition = 0, 
        execute_count = 0, count_commands = 0, continueLast = 0, oflags = 0;

    // WIll return reson for pause/error
    int pause_reason;

    // Initilizaing file desriptor 
    int *file_discriptor = 0;

    arguments_t filename;
    avcommand_t *comds, *lastCom = 0;
    
    do 
    {
        // User opens a file for redirection
        if(file_discriptor)
        {
            in_possition += Process_Arguments(input + in_possition, &arg_charters, arg_buffer + 
                                              arg_chartersTotal, &arg_count, arg_list + arg_total, &pause_reason);
            
            // Error message to user 
            if(pause_reason == STATS_ERROR || arg_count != 1)
            {
                fprintf(stderr, "ERROR: Incorrect reading from filename for redirection! Try again.\n");
                continue;

            } //End 2nd if()

            filename = *(arg_list + arg_total);
            arg_chartersTotal += arg_charters;
            arg_total += arg_count+1;
            
            // Opening a file -> system call open(file, oflag, mode)
            // S_IRUSR - Read permission bit for the owner of the file.
            // S_IWUSR - Write permission bit for the owner of the file. 
            // S_IRGRP - Read permission bit for the group owner of the file. 
            // S_IROTH - Read permission bit for other users.
            *file_discriptor = open(filename, oflags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

            // Error message to user
            if(*file_discriptor < 0)
            {
                fprintf(stderr, "Error: Cannot open file '%s' for redirection.\n", filename);
                continue;
            }// End if()

            lseek(*file_discriptor, 0, SEEK_SET);
            file_discriptor = 0;
        }
        else
        {
            in_possition += Process_Arguments(input + in_possition, &arg_charters, arg_buffer + arg_chartersTotal, 
                                            &arg_count, arg_list + arg_total, &pause_reason);
            
            // Error message to user in case of unrecognized command
            if(pause_reason == STATS_ERROR && !continueLast)
            {
                fprintf(stderr, "Unrecognized command input.\n");
                continue;
            }

            if(!continueLast)
            {
                if(arg_count == 0)
                {
                    continue;
                }// End if()

                // Accessing member struct -> av_command
                comds = c+count_commands++;
                comds -> arg_list = arg_list + arg_total; // argument list 
                comds -> stop_fail = 0;                   // fail
                comds -> stop_success = 0;                // success
                comds -> exe_second = 0;                  // execute second command 
                comds -> out_fd = fileno(stdout);

                if(lastCom)
                {
                    lastCom->next = comds;
                }// End if()
            }// End if()

            arg_chartersTotal += arg_charters;
            arg_total += arg_count+1;
            continueLast = 0;

        }// else ...

        // Switch 
        switch(pause_reason)
        {
            // output redirection 
            case STATS_REDIRECTION_OUT:
                file_discriptor = &(comds -> out_fd);
                oflags = O_WRONLY | O_CREAT | O_TRUNC;
                break;

            default:
                ++execute_count;
                lastCom = 0;
        }// End switch()

    } while(pause_reason != STATS_DONE);

    // Return counts of execution
    return execute_count;

}// end Process_OutputRedireciton

/*******************************************************************************************************
* Function Name: Process_Execution                                                                     *
*     Arguments: Pointer to [execute, count_commands]                                                  *            
*   Description: This function will create the logic of executing commands.                            *
*                                                                                                      *
*        Return: total_status  -> returns total number of commands of execution.                       *
*******************************************************************************************************/
 int Process_Execution(const avcommand_t *execute, int *count_commands)
 {
    // Initilizaing variables 
    int stop = 0;
    int rank, total_status;
    
    // Setting command count ptr to 0
    *count_commands = 0;
  
     while(execute)
     {
         if(!stop)
         {
             // If we have a second command
             if(execute -> exe_second)
             {
                 // Calling Second_Command_Execution
                 rank = Second_Command_Execution(execute, execute -> next); 

                 if(!count_commands)
                 {
                     total_status = rank;
                 }// End if()
                 while(execute -> exe_second)
                 {
                     ++(*count_commands);
                     execute = execute -> next;
                 }// End while()

             }// End if() Second execution 
             else // Having a single command 
             {
                 // Calling First_Command_Execution to execute 
                 rank = First_Command_Execution(execute);

                 if(!count_commands)
                 {
                     total_status = rank;
                 }
             }// End single if()
          
             // If execution fails 
             if(execute -> stop_fail)
             {
                 // abs -> zero rank 
                 total_status += abs(rank); 
                 stop = rank;
             }// End if()
             else if(execute -> stop_success) // Exeuciton successed 
             {
                 total_status *= rank; // seting it to zero
                 stop = !rank;
             }// End else if()
             else
             {
                 total_status += rank;
             }// End  else()
         }// End if()

         // Keep executing
         ++(*count_commands);
         execute = execute -> next;
         
     }// End while()

     return total_status;

 }// end Process_Execution

/*******************************************************************************************************
* Function Name: First_Command_Execution                                                               *
*     Arguments: Pointer to [command]                                                                  *            
*   Description: This function will create the logic of executing a single command and will call the   *
*                necessary functions needed to perform the first command of exeuciton.                 *
*        Return: commands -> returns total number of commands of execution.                            *
*******************************************************************************************************/
int First_Command_Execution(const avcommand_t *command) 
{
    pid_t pid;      // Child process 
    int stats_exit; // Reason of exit at a child given a parent process
    
    // If 'q' is at the command line -> exit program 
    if(strcmp( command->arg_list[0], "q" ) == 0)
    {
         exit(0);
         return 0;
    }// End if()

    // If 'quit' is at the command line -> exit program 
    if(strcmp( command->arg_list[0], "quit" ) == 0)
    {
        exit(0);
        return 0;
    }// End if()

    // If user enters 'fcfs' call first come first serve algorithm 
    if(strcmp(command->arg_list[0], "fcfs") == 0)
    {
        func_fcfs(); // Call funciton 
        return 0;
    }// End if()
    
    // Creating 1st child 
    pid = fork();
    
    // Checking child, returning error message to user
    if(pid < 0)
    {
        fprintf(stderr, "ERROR: Can't fork process for command '%s'. Try again!\n", command->arg_list[0]);
        exit(1);
    }// End if ()
    else if(pid == 0) // Executing first command 
    {
        // Duplicating file to stdout
        dup2(command->out_fd, fileno(stdout)); 
        
        // executing command 
        execvp(command->arg_list[0], command->arg_list);
        
        // Display error to user 
        fprintf(stderr, "couldn’t execute: '%s'\n", command->arg_list[0]);
        exit(1);
    }// End else if()
    else
    {
        // Waiting for first child to execute 
        waitpid(pid, &stats_exit, 0);
        
        // Close the end
        if(command->out_fd != fileno(stdout))
        {
            close(command->out_fd);
        }
        
        stats_exit = WEXITSTATUS(stats_exit);
    }// End else ()
    
    return stats_exit;

}// end First_Command_Execution

/*******************************************************************************************************
* Function Name: Second_Command_Execution                                                              *
*     Arguments: Pointer to [Left, Right]                                                              *            
*   Description: This function will create the logic of executing a second command and will call the   *
*                necessary functions needed to perform these commands of execution.                    *
*        Return: commands -> returns stats_exit of the commands                                        *
*******************************************************************************************************/
int Second_Command_Execution(const avcommand_t *Left, const avcommand_t *Right){
    
    int stats_exit, child_exit_status;
    pid_t pid_Right, pid_Left;        // PIDs for child processes 
    int command_pipe[2];              // Initializing pipe 
    
    // Execute left command and right 
    if(!(Left -> exe_second && Right))
    {
        // Last command, execute it
        return First_Command_Execution(Left);
    }// End if()
    
    pipe(command_pipe); // Initializing to pipe
    pid_Right = fork(); // Create fork and set fork() to pid_right
    
    if(pid_Right < 0)
    {
        // Return message to user 
        fprintf(stderr, "ERROR: Can't fork process for command '%s'. Try again!\n", Right -> arg_list[0]);
        exit(1);

    }// End if()
    else if(pid_Right == 0) // Checking fork equaling 0
    {
        close(command_pipe[1]); // Close first command end
        exit(Second_Command_Execution(Right, Right -> next));

    }// End else if()
    else
    {
        pid_Left = fork(); // Create fork and set fork() to pid_left. Execute left command
        
        if(pid_Left < 0)
        {
             // Return message to user 
            fprintf(stderr, "ERROR: Can't fork process for command '%s'. Try again!\n", Left -> arg_list[0]);
            exit(1);

        }// End else()
        else if(pid_Left == 0)
        {
            close(command_pipe[0]);                // Closing first command of execution
            dup2(command_pipe[1], Left -> out_fd); // Execution of left command 
            exit(First_Command_Execution(Left));   // Terminate with first execution command 

        }// End else if()
        else{
            close(command_pipe[0]); // Closing ends
            close(command_pipe[1]); // Closing ends
            
            waitpid(pid_Right, &child_exit_status, 0);          // Waiting on right child process 
            child_exit_status = WEXITSTATUS(child_exit_status); // Return status 
            
            waitpid(pid_Left, &stats_exit, 0);                  // Waiting on left child process 
            stats_exit = WEXITSTATUS(stats_exit);               // Return status
            
            stats_exit += WEXITSTATUS(child_exit_status);

        }// End else()

    }// End else()
    return stats_exit;
}// End Second_Command_Execution()

/*******************************************************************************************************
* Function Name: func_fcfs                                                                             *
*     Arguments: none                                                                                  *            
*   Description: This function will create and genarate random numbers and will do a simulation of a   *
*                first come first serve 'fcfs' algorithm.                                              *
*        Return: return 0;                                                                             *
*******************************************************************************************************/
int func_fcfs()
{
        // Initializing variables 
        int i, b[10];
        srand(time(0));
   
	    //default 
	    int n = 5;
    
	    float total_fcfs, w = 0, sum = 0, sum2 = 0; 
    
	    std::cout << "FCFS CPU scheduling simulation with 5 processes";
    
        // Generating random numbers, only 5 values 
	    for(i = 1;i <= n;i++)
	    {
	    	b[i] = rand() % 100; // Setting array with the random values
	    	total_fcfs = b[1];
	    }// End for()
    
 	    // fcfs algo
 	    for(i = 1;i <= n; i++)
 	    {	
 	    	total_fcfs = b[i] + w;  // adding burst time and waiting 
            sum2 += w;              // wait time
 	    	w += b[i];              // it will add the array {bursting time} to the variable "w"
 	    	sum += total_fcfs;      // total fcfs
 	    }// End for()

	    std::cout << std::endl;
	    std::cout << "CPU burst: " << b[1] << "ms" << std::endl;
	    std::cout << "CPU burst: " << b[2] << "ms" << std::endl;
	    std::cout << "CPU burst: " << b[3] << "ms" << std::endl;
	    std::cout << "CPU burst: " << b[4] << "ms" << std::endl;
	    std::cout << "CPU burst: " << b[5] << "ms" << std::endl;
        std::cout << "Total waiting time in the ready queue: " << sum2 << " ms"<< std::endl;
	    std::cout << "Average waiting time in the ready queue: " << sum2/n << " ms" << std::endl;
        
        return 0;
}// End func_fcfs()