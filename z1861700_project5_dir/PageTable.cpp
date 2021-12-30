/***********************************************************************************************
CSCI 480 -  Assignment 05 Virtual Memory Simulation FIFO 

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: November 19, 2021

 File Name: PageTable.cpp
************************************************************************************************/
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

#include "PageTable.h"  	//contains the class PageTable
#include "MemoryChunk.h"  	//contains the struct for a PageTableEntry
#include "PhysicalMemory.h" //contains the class PhysicalMemory

using namespace std;
/************************************************************************************************
* 	 Class Name: PageTable										                    	        *
*     Arguments: PhysicalMemory *physcialmemory_,int size_					                    *            
************************************************************************************************/
PageTable::PageTable(PhysicalMemory *physcialMemory_,int size_)
{
		pgHIts = 0; 	   // Page hits for page faults
		pgTotalFaults = 0; // Total page faults
		size = size_;      // Size of the page table that will create the entries
		physcialMemory = physcialMemory_; // Pointer to calling other methods
}// End PageTable::PageTable

/************************************************************************************************
* Function Name: void PageTable::reference(int pageID, string name)                             *
*     Arguments: int pageID, string name                                                        *            
*   Description: This  function will reference a logical page and will try to acess the page    *
*				 based on the page table. Depending if it already excist in the memory.  		*
************************************************************************************************/
void PageTable::reference(int pageID, string nameList)
{
		pgTotalFaults++;
		int found = 0;   // Flag page found, access entry in Physical Memory
		int remove = -1; // Flag page remove

		for(std::vector<MemoryChunk>::iterator it = table.begin(); it != table.end(); ++it) 
		{
			if(it -> nameList == nameList)
			{
				found = 1;
				
				if(it -> state == 1) // Entry to Physical Memory 
				{
					pgHIts++; // Incrementing page hits
					physcialMemory -> accessMemory(it -> page, it -> nameList); // Pointer to the PhysicalMemory to access the memory
				}
				else // Displaying page fault
				{
					cout << "PageTable: page fault occurred" << endl; 				
					vector<int>map = physcialMemory -> swapIn(pageID, nameList); // Calling swapIn to swap in the page
					
					// Update entires in page table
					it -> page = map[0]; // Mapping of the page is invalid 
					it -> state = 1;	 // Valid state
					remove = map[1];     // remove 
					physcialMemory -> accessMemory(it -> page, it -> nameList); // Accessing the physical memory
				}
			}
		 }// End Flag on(1)
		 if(remove != -1) // If remove is not -1 update state 
		 {
			for(std::vector<MemoryChunk>::iterator it = table.begin(); it != table.end(); ++it)
			{
				if(it -> pageID == remove)
				{
					it -> state = 0; // Set state to invalid
				}
			}		
			 
		 }// End remove != -1
		 if(found == 0) // If flag is 0, page fault 
		 {
			cout << "PageTable: page fault occurred" << endl;  					
			vector<int> map = physcialMemory -> swapIn(pageID,nameList); // Swapping in the page
			
			MemoryChunk x(nameList, pageID, 1, map[0]); 	    // Pass param to MemoryChunk
			this -> table.push_back(x); 					    // Push to page table
			physcialMemory -> accessMemory(map[0], x.nameList); // Accessing physical memory

			for(std::vector<MemoryChunk>::iterator it = table.begin(); it != table.end(); ++it)
			{
				if(it -> pageID == map[1])
				{
					it -> state = 0; // Set state to invalid
				}
			}		
		 }// End Found == 0 
	cout << endl;
}// End void PageTable::reference

/*****************************************************************************************************
* Function Name: void PageTable::printTable                            								 *
*     Arguments: none                                                  								 *            
*   Description: The purpose of this function is to print a snapshot of the physical index, physical *
*				 frame index, and will pring the current number of fault.		   				     *
*****************************************************************************************************/
void PageTable::printTable()
{
	std::sort(table.begin(), table.end()); // Sorting the table enties

	cout << "Page Table Snapshot: " << endl;	

	int i = 0; // Page index 

	for(std::vector<MemoryChunk>::iterator it = table.begin(); it != table.end(); ++it)
	{
		int physical = (it -> state == 1) ? (it -> page) : (0);	  // Physical - initializing the number of faults to 0
		string valid = (it -> state == 1) ? ("true") : ("false"); // If the state of the iteration {true or false}

		cout << "Page Index: " << i << " : Physical Frame Index: " << physical-1 << " : In Use: " << valid << endl;
		i++;
	}

	cout << "PageTable: Current number of page faults: " << pgTotalFaults - pgHIts << endl << endl; // total number of page faults
}// End void PageTable::printTable()