/***********************************************************************************************
CSCI 480 -  Assignment 05 Virtual Memory Simulation FIFO 

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: November 19, 2021

 File Name: PhysicalMemory.h
************************************************************************************************/
#ifndef PhysicalMemory_h
#define PhysicalMemory_h

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;
/***************************************************************************************
* 	 Class Name: PhysicalMemory										                   *
*     Arguments: PhysicalMemory *physcialMemory_,int size_					           *            
*   Description: Initializing the physical memory containing the enties of the frames  *
*				 virtual address.													   *
***************************************************************************************/
class PhysicalMemory
{
	public:
		int size;      // Physical mem size
		int last;	   // Accessed time
		int algorithm; // Indicate if using FIFO or LRU

		vector<int> memory;   // Physical mem. 
		vector<int> access;   // Page access based on page table
		vector<string> names; // The physical memory
		
		PhysicalMemory(int size_,int algorithm_); 		// Initialize all the data and data structure in the class 
		vector<int> swapIn(int pageID,string nameList); // Returns the frame ID of the item just swapped in. If FIFO update the timestamp and time list
		void printMemory(); 							// Printing the physical memory 
		void accessMemory(int page, string nameList);   // Accesing the frame
};
#endif