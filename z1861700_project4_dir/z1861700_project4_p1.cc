/**********************************************************************************
CSCI 480 -  Assignment 04 Part 1 - Fall, 2021

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: October 30, 2021

   Purpose: The purpose of this assignment is to solver th Reader-Writer problem
            Using the PThreads library. This program containts of two parts. Part 1
            and Part two are similiar but Part 2 requires smore strict alerations
            rewarding the writer and the reader.

 Arguments: This program takes two arguments in the command line. The first argu-
            ment is the number of reader threads. Second argument is the writer 
            threads.

   Example: z1861700_project4_p1 <# of readers > <# of writers>
            z1861700_project4_p1 10 3       

 Last Edit: Sat Oct 30 2021 13:57:17 GMT-0500 (Central Daylight Time)
*********************************************************************************/
#include <time.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <bits/stdc++.h>

using namespace std;

// Declaring Semaphores 
sem_t rwsem;     //used by both readers and writers
sem_t cssem;     //used for protecting critical sections of readers

// Global Variables
string stringWord = "All work and no play makes Jack a dull boy."; // Initialize the shared string
int stringWord_length = stringWord.length();                       // Return the length of the share string
int readcounter = 0;                                               // Read counter, to keep track of readers

/*********************************************************************************************
* Function Name: writerfunction                                                              *
*     Arguments: void *param                                                                 *            
*   Description: The writer thread will alternate between writing and sleeping for 1 second. * 
*                During each iteration, it motifies the shared string by removing the last   *
*                character of the given shared string.                                       *
**********************************************************************************************/
void *writerfunction(void *param)
{
    long id = (long)param; // Writer ID

    // This if loop, will loop until the string is empty
    if(stringWord.empty() != true)
    {
        for(int i = 0; i < stringWord_length; i++)      // Will iterate base on the length of the share string
        {
            if(stringWord.empty() == true)
                break;

            sem_wait(&rwsem);                           // Semaphore lock
            fflush(stdout);                             // Flush File Buffer                              
            printf("writer %ld is writing ...\n", id);  // Printing writer ID                             
                                                        
            // Chopping the last string until share string is empty                                       
            if(stringWord.empty() == false)             
            {                                           
                stringWord.pop_back();
            }
            sem_post(&rwsem);                          // Semaphore unlock

            sleep(1);                                  // Sleep for 1 second
        }
    } 
    else // Error checking
    {   
        printf("Error: Check writer ...");
        exit(-1);
    }
    
    // Once share string is empty exit with writers IDs
    if(stringWord.empty())
    {
		printf("writer %ld is exiting ... \n", id);
    }
    pthread_exit(NULL);

}// End writerfunction()


/*********************************************************************************************
* Function Name: readerfunction                                                              *
*     Arguments: void *param                                                                 *            
*   Description: The reader thread is structured in a if loop. It will pring the             * 
*                content of the share string and the current readcount value when the value  *
*                increments or decrements.                                                   *
**********************************************************************************************/ 
void *readerfunction(void *param)
{
    long id = (long) param; // Reader ID

    // This if loop, will loop until the string is empty
    if(stringWord.empty() != true)
    {
        for(int i = 0; i < stringWord_length; i++)  // Will iterate base on the length of the share string
        {
            if(stringWord.empty() == true)
            break;

            sem_wait(&cssem);                                       // Semaphore lock, before entering crital section 
            readcounter++;                                          // Incrementing the counter 
            printf("read_count increments to: %d.\n", readcounter); // print reader increment counter

            if(readcounter == 1)                                    // If this is the first reader, we need to wait for the writer
            {                                    
                sem_wait(&rwsem);                           
            }   

            sem_post(&cssem);                                       // Semaphore unlock
            fflush(stdout);
            printf("reader %ld is reading ... content : %s\n", id, stringWord.c_str()); // Printing content of the given share string

            sem_wait(&cssem);                                                           // Semaphore lock, before entering crital section
            readcounter--;                                                              // Decrementing reader counter
            fflush(stdout);     
            printf("read_count decrements to: %d.\n", readcounter); // print reader decrement counter

            if(readcounter == 0)                                    // When readercount is 0, reader calls sem_post     
            {
                sem_post(&rwsem);
            }
            sem_post(&cssem);                                       // Semaphore unlock
            sleep(1);                                               // Sleep 1 second
        }

    }
    else // Error checking
    {
        printf("Error: Check reader ...\n");
        exit(-1);
    }

    // Once share string is empty exit with writers IDs
    if(stringWord.empty()) 
    { 
        printf("reader %ld is exiting ... \n", id); 
    }

    pthread_exit(NULL);

}// End readerfunction()

/************************************************************************************************
* Function Name: main                                                                           *
*     Arguments: int argc, char *argv[]                                                         *            
*   Description: This is the main function of this program. The main will do the initialization * 
*                of the semaphores and will create the reader writer threads base on            *
*                what user enters. Once it creates the threads it will wait for the threads     *
*                to finish and will do a clean up and exit.                                     *
************************************************************************************************/
int main(int argc, char *argv[]) 
{ 
    // Strictly taking 2 arguments, the number of threads and number of writer threads
    if (argc != 3)
    {
      fprintf(stderr,"usage: z1861700_project4_p1 <Number of reader Thread> <Number of writer Thread>\n");
      return -1;
    }

	// Setting values to designated variables base on users choice of number of writer/reader threads.
	int numreaders = atoi(argv[1]);
	int numwriters = atoi(argv[2]);

    pthread_t readersthread[numreaders]; // Creating reader threads
    pthread_t writersthread[numwriters]; // Creating writer threads

    printf("*** Reader-Writer Problem Simulation ***\n");
    printf("Number of reader threads: %d\n", numreaders);
    printf("Number of writer threads: %d\n", numwriters);

    // Initialize reader/writer semaphore with error check to user
    if(sem_init(&rwsem, 0, 1) == -1)
    {
        printf("Error initizling empty semaphore\n");   // Error message to user
        exit(-1);
    }

    // Initialize critical section semaphore with error check to user
    if(sem_init(&cssem, 0, 1) == -1)
    {
        printf("Error initizling empty semaphore\n");  // Error message to user
        exit(-1);
    }

    // Creating reader threads with error check
    for (long j = 0; j < numreaders; j++)
    {
        if (pthread_create(&readersthread[j], NULL, readerfunction, (void*) j) != 0)
        {
            printf("Error creating reader thread\n"); // Error message to user
            exit(-1);
        }
    }

    // Creating writer threads with error check
    for(long i = 0; i < numwriters; i++)
    {

        if (pthread_create(&writersthread[i], NULL, writerfunction, (void*) i) != 0)
        {
            printf("Error creating writer thread\n"); // Error message to user
            exit(-1);
        }
    }    

    // Join reader threads 
    for (int i = 0; i < numreaders; i++)
    {
        if (pthread_join(readersthread[i], NULL) != 0)
        {
            printf("Error joining reader threads\n"); // Error message to user
            exit(-1);
        }
    }    

    // Join writers threads 
    for (int i = 0; i < numwriters; i++)
    {
        if (pthread_join(writersthread[i], NULL) != 0)
        {
            printf("Error joining writer threads\n"); // Error message to user
            exit(-1);
        }
    }

    printf("All threads are done. \n");
    fflush(stdout);

    // Clean up
    sem_destroy(&rwsem);
    sem_destroy(&cssem);

    printf("Resources clean up. \n");
    fflush(stdout);
    pthread_exit(NULL);

}// End main()