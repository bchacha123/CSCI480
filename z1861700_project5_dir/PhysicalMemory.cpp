/***********************************************************************************************
CSCI 480 -  Assignment 05 Virtual Memory Simulation FIFO 

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: November 19, 2021

 File Name: PhysicalMemory.cpp
************************************************************************************************/
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "PhysicalMemory.h" 

using namespace std;
/**************************************************************************************************
* Function Name: void PageTable::reference                                                        *
*     Arguments: int size_, int algorithm_                                                        *            
*   Description: Initializing the physical memory containing the enties of the frames             *
***************************************************************************************************/
PhysicalMemory::PhysicalMemory(int size_, int algorithm_)
{
		size = size_;           // Physical mem size
		algorithm = algorithm_; // FIFO / LRU
		last = size - 1;        // Time accessed 
}

/**************************************************************************************************
* Function Name: void PhysicalMemory::printMemory                                                 *
*     Arguments: none                                                                             *            
*   Description: Printing out information to user rewarding the layout of the page table and the  *
*                physical memory.                                                                 *
***************************************************************************************************/
void PhysicalMemory::printMemory()
{
	
		cout << "Physical Memory Layout:" << endl; 
		int i = 0; // Frame index

		for(std::vector<int>::iterator it = memory.begin(); it != memory.end(); ++it) 
		{
			cout << "Frame: " << i << " contains: " << names[i] << endl; // Output frame ID and content
			i++;
		}
		cout << endl;
}// End PhysicalMemory::printMemory

/**************************************************************************************************
* Function Name: PhysicalMemory::swapIn                                                           *
*     Arguments: PageID, name                                                                     *            
*   Description: Printing out information to user rewarding the physical swap then updaing        *
***************************************************************************************************/
vector<int> PhysicalMemory::swapIn(int pageID, string nameList)
{
    vector<int> swap_return; // Vector that holds the swap elements 
    cout << "Physical: Swap In: " << nameList << endl;
   
        if (memory.size() < size) // Swap with normal location 
        {
            int map = memory.size();  // Size of the memory
            memory.push_back(pageID); // New element is inserted to memory 
            names.push_back(nameList);// New element inserted to physical memory
             
            swap_return.push_back(map + 1); // Element inserted to swap return
            swap_return.push_back(-1);    
            last = size - 1; // Update accessed time
           
            return swap_return;
        }
        else // Swap with prepaging location 
        {

            int map = (last + 1) % size;
            int prev = memory[map];
            memory[map] = pageID;  // Set page in memory
            names[map] = nameList; // Set nameList 
            vector<int> swap_return;
            swap_return.push_back(map + 1);
            swap_return.push_back(prev);
            last = map; // Update accessed time
            
            return swap_return;
        }
}// End PhysicalMemory::swapIn

/**************************************************************************************************
* Function Name: PhysicalMemory::accessMemory                                                     *
*     Arguments: page, nameList                                                                   *            
*   Description: Printing out information to user rewarding the physical accessed frameID and its *
*                content.                                                                         *
***************************************************************************************************/
void PhysicalMemory::accessMemory(int page, string nameList)
{
    cout << "Physical: Accessed frameID: " << page - 1 << " contains: " << nameList << endl;

    if(access.size() > page - 1) access[page - 1]++; // Accessing the page base on the page table
}// End PhysicalMemory::accessMemory