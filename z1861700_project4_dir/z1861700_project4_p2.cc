/**********************************************************************************
CSCI 480 -  Assignment 04 Part 2 - Fall, 2021

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: October 30, 2021

   Purpose: The purpose of this assignment is to solver th Reader-Writer problem
            Using the PThreads library. This program containts of two parts. Part 1
            and Part two are similiar but Part 2 requires more strict alerations
            rewarding the writer and the reader.

 Arguments: This program takes two arguments in the command line. The first argu-
            ment is the number of reader threads. Second argument is the writer 
            threads.

   Example: z1861700_project4_p2 <# of readers > <# of writers>
            z1861700_project4_p2 1 10     

 Last Edit: Sat Oct 30 2021 14:01:29 GMT-0500 (Central Daylight Time)
*********************************************************************************/
#include <string>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

// Function declaration
void *writerfunction(void *param);
void *readerfunction(void *param);

// Declaring Semaphores 
sem_t rwsem;	//used by both readers and writers	
sem_t cssem;	//used for protecting critical sections of readers

// Global Variables
string stringWord = "All work and no play makes Jack a dull boy.";

// Defining struct for thread ID and writers that are waiting and exiting
struct readargs
{
	long thread_id;
	long writer_queue; 
}; struct readargs rStruct;

/*********************************************************************************************
* Function Name: writerfunction                                                              *
*     Arguments: void *param                                                                 *            
*   Description: The writer thread will alternate between writing and sleeping for 1 second. * 
*                During each iteration, it motifies the shared string by removing the last   *
*                character of the given shared string. This writer funtion is much more      *
*				 strict due to some writers being missed by the readers. This writer function*
*				 will be much more strict alternating betwee writers and reading.			 *
**********************************************************************************************/
void *writerfunction(void *param) 
{	
	while(true)
	{
		// This if loop, will loop until the string is empty
		if(stringWord.empty())
			break;
		
		sem_wait(&rwsem);									// Semaphore lock (reader/writer)
		if(!stringWord.empty())								// loop until share string is empty
		{
			printf("writer %ld is writing ... \n", param);  // Printing writer ID
		}
		// Chopping the last string until share string is empty
		if(!stringWord.empty()) 
		{
			stringWord.pop_back();
		}
		sem_post(&cssem);									// Semaphore unlock (critical section)
		sleep(1);											// Sleep for 1 second
	}
    pthread_exit(NULL);

}// End writerfunction()

/*********************************************************************************************
* Function Name: readerfunction                                                              *
*     Arguments: void *param                                                                 *            
*   Description: The reader thread is structured in a while loop. It will print the          * 
*                content of the share string and once the reader is done writing it will 	 *
*                return how many semaphores were still waiting and the thread ID of exiting  *
*				 threads.																     *
**********************************************************************************************/ 
void *readerfunction(void *param) 
{
	readargs *arg = (readargs *)param;

	// Initializing number of threads reader/writer
	int numreaders = 1;
	int numwriters = 1;

	while(!stringWord.empty()) // Loop until shared string is empty
	{
		sem_wait(&cssem); 	   // Semaphore Lock (critical section)
		printf("reader %ld is reading ... content : %s\n", rStruct.thread_id, stringWord.c_str()); // Printing content of the given share string
		sem_post(&rwsem);	   // Semaphore unlock (reader/writer)
		sleep(1);			   // Sleep for 1 second
	}

	// printing reader IDs that are exiting
	if(stringWord.empty())     
	{
		printf("reader %ld is exiting ... \n", rStruct.thread_id); 
	}

	// printing writer semaphores still waiting on queue
	printf("There are still %ld writers waiting on the semaphore...\n", rStruct.writer_queue);

	for(long i = 0; i < rStruct.writer_queue; i++) // When readercount is 0, reader calls sem_post 
	{
		sem_post(&rwsem);							// Semaphore unlock (reader/writer)
	}

	int waitingWriters = numwriters - numreaders;
	// printing writers IDs that are exiting

	printf("writer %ld is exiting ... \n", waitingWriters);
	if(stringWord.empty())
	{
		for(long i = 0; i < rStruct.writer_queue; i++)
		{
			sem_post(&rwsem);
        	waitingWriters--;
		}
	}

	// int waitingWriters = numwriters - numreaders;
    // printf("30There are still %d writers waiting on the semaphore...\n", waitingWriters);

    // while (waitingWriters)
    // {
    //     sem_post(&rwsem);
    //     waitingWriters--;
	// 	printf("writer %ld is exiting ... \n", waitingWriters);
    // }

	pthread_exit(NULL);
}

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
	// Initializing number of threads reader/writer
	long numreaders = 1;
	long numwriters = 1;

	// Initializing semaphores
	sem_init(&rwsem, 0, 1);
	sem_init(&cssem, 0, 0);	
	
	if(argc > 1)
	{ 
		numreaders = stoi(argv[1]); 
	}
	if(argc > 2)
	{
		numwriters = stoi(argv[2]);
	}
    if (argc != 3)
    {
      fprintf(stderr,"Usage: z1861700_project4_p1 <Number of reader Thread> <Number of writer Thread>\n");
      return -1;
    }
	
	printf("*** Reader-Writer Problem Simulation ***\n");
	printf("Number of reader threads : %d\n", numreaders);
	printf("Number of writer threads : %d\n", numwriters);

	pthread_t readersthread[numreaders]; // Creating reader threads
	pthread_t writersthread[numwriters]; // Creating writer threads
	
	// Creating reader threads with error check
	for(long i = 0; i < numreaders; i++) 
    {
		rStruct = {i, (numwriters - numreaders)}; // Number of total writers minus the number of total readers
		int readthreadcreate = pthread_create(&readersthread[i], NULL, readerfunction, (void *) i);
		
        if(readthreadcreate) 
        {
			printf("Error creating reader thread %d\n", readthreadcreate);
			exit(-1);
		}
		if(readthreadcreate)
		{
			int rStruct2 = numwriters - numreader;; // Number of total writers minus the number of total readers

			sem_post(&rwsem);
			rStruct--;
			printf("writer %ld is exiting ... \n", rStruct2);

		}
	}
	
	// Creating writer threads with error check
	for(long i = 0; i < numwriters; i++) 
    {
		int writerthreadcreate = pthread_create(&writersthread[i], NULL, writerfunction, (void *) i);
		if(writerthreadcreate) 
        {
			printf("Error creating reader thread %d\n", writerthreadcreate);
			exit(-1);
		}
	}
	
	// Join reader threads 
	for(int i = 0; i < numreaders; i++) 
    {
		int readthreadjoin = pthread_join(readersthread[i], NULL);
		if(readthreadjoin) 
        {
			printf("Error joining reader thread %d\n", readthreadjoin);
			exit(-1);
		}
	}

	// Join writer threads 
	for(int i = 0; i < numwriters; i++)
    {
		int writethreadjoin = pthread_join(writersthread[i], NULL);
		if(writethreadjoin) 
        {
			printf("Error joining writer thread %d\n", writethreadjoin);
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