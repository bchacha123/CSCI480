/***********************************************************************************************
CSCI 480 -  Assignment 05 Virtual Memory Simulation FIFO 

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: November 19, 2021

 File Name: PageTable.h
************************************************************************************************/
#ifndef PageTable_h
#define PageTable_h

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "PhysicalMemory.h" //contains the class PhysicalMemory
#include "MemoryChunk.h"    //contains the struct for a PageTableEntry

using namespace std;
/************************************************************************************************
* 	 Class Name: PageTable										                    	        *
*     Arguments: PhysicalMemory *physcialMemory_,int size_					                    *            
*   Description: This class initializes the address mapping, which contains one entry for each  *
*				 virtual address.																*
************************************************************************************************/
class PageTable
{	
	public:
		int pgHIts;										 // Page hits for page faults
		int pgTotalFaults;								 // Total page faults
		int size;										 // Size of the page table that will create the entries
		vector<MemoryChunk> table;					     // The page table
		PhysicalMemory* physcialMemory;
		PageTable(PhysicalMemory* pmIn,int tableSize);   // Initialize all entries to invalid
		void reference(int pageID,string ref);		     // Reference a logical page,  if not in memory,  call pageFault() 
		void printTable();							     // Print the layout of page table
};
#endif