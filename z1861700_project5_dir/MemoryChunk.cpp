/***********************************************************************************************
CSCI 480 -  Assignment 05 Virtual Memory Simulation FIFO 

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: November 19, 2021

 File Name: MemoryChunk.cpp 
************************************************************************************************/
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "MemoryChunk.h"  

using namespace std;
/************************************************************************************************
* 	Struct Name: MemoryChunk										                            *
*     Arguments: string nameList_, int pageID_, int state_, int page_                           *            
************************************************************************************************/
MemoryChunk::MemoryChunk(string nameList_, int pageID_, int state_, int page_)
{
	nameList = nameList_;   // RunList - main, loan, holding etc.
	pageID = pageID_; 		// The page ID
	state  = state_; 		// State indicates if there is physical mem. present or virtual pg. if the frame is present 
	page   = page_;			// Indicating the page reference()

}// End MemoryChunk::MemoryChunk

bool MemoryChunk::operator < (const MemoryChunk& val) const 
{ 
	return pageID < val.pageID; 
	
}// End bool MemoryChunk::operator