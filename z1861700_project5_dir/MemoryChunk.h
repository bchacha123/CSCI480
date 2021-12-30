/***********************************************************************************************
CSCI 480 -  Assignment 05 Virtual Memory Simulation FIFO 

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: November 19, 2021

 File Name: MemoryChunk.h
************************************************************************************************/
#ifndef MemoryChunk_h
#define MemoryChunk_h

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;
/************************************************************************************************
* 	Struct Name: MemoryChunk										                            *
*     Arguments: none					                                                        *            
*   Description: This struct is used for the PageTableEntry							  		    *
************************************************************************************************/
struct MemoryChunk
{
		string nameList; // RunList - main, loan, holding etc.
		int pageID;  	 // The page ID
		int state;	 	 // State indicates if there is physical mem. present or virtual pg. if the frame is present. 
		int page;	 	 // Indicating the page reference()

		MemoryChunk(string nameList, int pageID, int state, int page); // Passing Run list, pageID, the state, and page regerence
		bool operator < (const MemoryChunk& val) const; 
};
#endif