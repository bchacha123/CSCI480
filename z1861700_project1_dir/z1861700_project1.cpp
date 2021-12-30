/********************************************************************
CSCI 480 - Assignment 01 - Fall, 2021

 Progammer: Brayan Chacha Gonzalez 
       zID: z1861700      
   Section: CSCI 480-01 - Principles Operating Systems
Instructor: Jie Zhou
  Date Due: September 9th, 2021

   Purpose: The purpose of this assignment is to have a better 
            understanding on the information of the operating system
            we are currently working with. We will be using files 
            that are already in the system to print out the 
            information they carry. 
*********************************************************************/
#include <map>
#include <string>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream> 

// Function declarations 
void QuestionA();
void QuestionB();
void QuestionC();
void QuestionD();
void QuestionE();

/*****************************************************************************
* Function Name: main                                                        *
*     Arguments: none                                                        *            
*   Description: This function print an appropiate header along with         *
*                calling the designated funtions that answers questions      *
*                about the operating system.                                 *
******************************************************************************/
int main(int argc, char** argv)
{
    std::cout << "*******************************************************************************************" << std::endl;
    std::cout << "                          _______________    ______________                                " << std::endl;
    std::cout << "                         / /            /   / |      ______|                               " << std::endl;
    std::cout << "                        / /     __     /    | |     |_____/                                " << std::endl;
    std::cout << "                       / /     / /    /     | |     |______                                " << std::endl;
    std::cout << "                      / /     / /    /      | |______      |                               " << std::endl;
    std::cout << "                     / /     /_/    /       |/______/|     |                               " << std::endl;
    std::cout << "                    / /            /  _        _____||     |                               " << std::endl;
    std::cout << "                   / /____________/ /|_|     /|____________|                               " << std::endl;
    std::cout << "                  /_/___________/   |_/     |_/___________/                                " << std::endl;     
    std::cout << "                                                                                           " << std::endl;   
    std::cout << "                  ______  ______________ _____________       _____________                 " << std::endl;      
    std::cout << "                /|      ||              ||            |    //            /                 " <<std::endl;    
    std::cout << "               | |      ||              ||     _______|   //     __     /                  " << std::endl;    
    std::cout << "               | |      ||     ____     ||    |______/   //     / /    /                   " << std::endl;    
    std::cout << "               | |      ||    |___/|    ||    |_______  //     / /    /                    " << std::endl;  
    std::cout << "               | |      ||    |   ||    ||     _______|//     /_/    /                     " << std::endl;   
    std::cout << "               | |      ||    |   ||    ||    |_____/ //            /                      " << std::endl; 
    std::cout << "               | |______||____|   ||____||____|      //____________/                       " << std::endl; 
    std::cout << "               |/______/|/___/    |___/ /___ /      //___________/                         " << std::endl; 
    std::cout << "                                                                                           " << std::endl;
    std::cout << "*******************************************************************************************" << std::endl;

    // Calling Question A. 
    QuestionA();

    // Calling Question B. 
    QuestionB();

    // Calling Question C. 
    QuestionC();

    // Calling Question D. 
    QuestionD();

    // Calling Question E.
    QuestionE();

	return 0;

}// End Main()

/*****************************************************************************
* Function Name: QuestionA                                                   *
*     Arguments: none                                                        *            
*   Description: This function returns information rewarding the OS          *
*                like ostype, host, osrelease, and version in a              *
*                readiable manner.                                           *
*                                                                            *
* Question A - 1. ostype                                                     *
*              2. hostname                                                   *
*              3. osrelease                                                  *
*              4. version                                                    *
******************************************************************************/
void QuestionA()
{

    //Declartion of variables for Q1
    std::string sq1_ostype;
    std::string sq1_host;
    std::string sq1_osrelease;
    std::string sq1_version;

    // Input files necessary for Q1
    std::ifstream in_ostype ("/proc/sys/kernel/ostype");       // Will input the ostype        -> Lunux 
    std::ifstream in_host ("/proc/sys/kernel/hostname");       // Will input the host name     -> turing 
    std::ifstream in_osrelease ("/proc/sys/kernel/osrelease"); // Will input the host os lease -> 4.19.0-17-amd64
    std::ifstream in_version ("/proc/sys/kernel/version");     // Will input the version       -> #1 SMP Debian 4.19.194-3 (2021-07-18)

    // Extracing content from input files and setting them to their appropiate strings
    std::getline(in_ostype, sq1_ostype);
    std::getline(in_host, sq1_host);
    std::getline(in_osrelease, sq1_osrelease);
    std::getline(in_version, sq1_version);

    // Output OS information 
    std::cout << "A. Questions about turing’s OS:" << std::endl;
    std::cout << "   1. ostype    - " << sq1_ostype << std::endl;
    std::cout << "   2. hostname  - " << sq1_host << std::endl;
    std::cout << "   3. osrelease - " << sq1_osrelease << std::endl;
    std::cout << "   4. version   - " << sq1_version << std::endl << std::endl;

    // Closing necessary files
    in_ostype.close();
    in_host.close();
    in_osrelease.close();
    in_version.close();

}// End Question A

/*****************************************************************************
* Function Name: QuestionB                                                   *
*     Arguments: none                                                        *
*   Description: This function carries the necessary information needed to   *
*                answer the following questions about the OS.                *
*                                                                            *
* Question B - 1. Number of processors                                       *
*              2. Number of physical multicore chips                         *
*              3. Amount of turing has been up                               *
*              4. Amount of turing time in days, hours, minutes and seconds. *
*****************************************************************************/
void QuestionB()
{

    // String declarations for Q2
    std::string sq2_cpuinfo;

    // Variable holding total processors 
    int num_processors = 0; 

    // Variable holding total physical milti-core chips
    int num_multichips; 
    int q2_chipid;

    std::map<int,int> q2_multichips;

    // Input files necessary for Q1
    std::ifstream in_cpuinfo ("/proc/cpuinfo");       // Will input information rewarding OS

    while (getline(in_cpuinfo, sq2_cpuinfo))
    {
        if (sq2_cpuinfo.find("processor") != std::string::npos) 
        { 
            num_processors++; 

        } else if (sq2_cpuinfo.find("physical id") != std::string::npos)
        {          
            int position = sq2_cpuinfo.find( ":" );

            sscanf( sq2_cpuinfo.substr(position + 2).c_str(), "%d", &q2_chipid);

            q2_multichips[q2_chipid] = true;
        }

    }// End while loop

    num_multichips = q2_multichips.size();
    
    // Input file for number of seconds turing has been up
    std::ifstream in_uptime ("/proc/uptime");

    // Declaring string to hold content
    std::string sq2_uptime;
    float uptime;

    // Extracing content from input file and setting it a string
    std::getline(in_uptime, sq2_uptime);

    // Reading the formatted data
    std::sscanf(sq2_uptime.c_str(), "%f", &uptime);

    // Declaring necessary variables
    float qb2_secods; 
    int qb2_minutes; 
    int qb2_hrs;
    int qb2_days;
    
    // Conversion to output seconds, minutes, hours, days
    qb2_secods = fmod(uptime, 60.0f);
    qb2_minutes = (int)(uptime - qb2_secods) / 60 % 60;
    qb2_hrs = (int)(uptime - qb2_secods) / (60 * 60) % 24;
    qb2_days = (int)(uptime - qb2_secods) / (24 * 60 * 60);

    // Output OS information 
    std::cout << "B. Questions about turing’s processors:" << std::endl;
    std::cout << "   1. How many processors does turing have?    -  " << num_processors << " processor" << ((num_processors > 1) ? "s" : "" ) << std::endl;
    std::cout << "   2. How many physical multi-core do we have? -  " << num_multichips << " multicore-core chip" << ((num_multichips > 1) ? "s" : "" ) << std::endl;
    std::cout << "   3. How long has turing been up in seconds?  -  " << uptime << std::endl;
    std::cout << "   4. Express that time in days, hours, minutes and seconds? " << qb2_days << " day" << ((qb2_days > 1 ) ? "s" : "") << 
                        ", " << qb2_hrs << " hour" << ((qb2_hrs > 1 ) ? "s" : "" ) << ", " << qb2_minutes << " minutes" 
                        << ((qb2_minutes > 1) ? "s" : "" ) << " and " << qb2_secods << " seconds" << std::endl << std::endl;

    // Closing necessary files
    in_cpuinfo.close();
    in_uptime.close();

}// End Question B

/*****************************************************************************
* Function Name: Question3                                                   *
*     Arguments: none                                                        *
*   Description: This function carries the necessary information to answer   *
*                information rewarding processor 0.                          *
*                                                                            *
* Question 3 - 1. Who is the vendor                                          *
*              2. What is the model name?                                    *
*              3. What is the physical address size?                         *
*              4. What is the virtual address size?                          *
*****************************************************************************/
void QuestionC()
{

    // Declaring string holding OS information 
    std::string strq3_cpuinfo;

    // Delaration of strings that will hold the given answers
    std::string strq3_vendor; 
    std::string strq3_modelname;

    // Declation of variables that will hold the physical and virtual values 
    int q3physical_num;
    int q3virtual_num;

    // Input file necessary for Q3
    std::ifstream inq3_cpuinfo ("/proc/cpuinfo");       // Will input information rewarding OS

    bool onVendor = false;
    bool onModelName = false;
    bool onAddresses = false; 

    // This while loop will go through the cpuinfo file an extract the given fields needed
    while (getline(inq3_cpuinfo, strq3_cpuinfo))
    {
        if (!onVendor && strq3_cpuinfo.find("vendor_id") != std::string::npos) 
        { 
            int position = strq3_cpuinfo.find( ":" );
            strq3_vendor = strq3_cpuinfo.substr(position + 2);
            onVendor = true;

        } else if(!onModelName && strq3_cpuinfo.find("model name") != std::string::npos)
        {
            int position = strq3_cpuinfo.find( ":" );
            strq3_modelname = strq3_cpuinfo.substr(position + 2);
            onModelName = true;

        } else if(!onAddresses && strq3_cpuinfo.find("address sizes") != std::string::npos)
        {
            int position = strq3_cpuinfo.find( ":" );
            sscanf(strq3_cpuinfo.substr(position + 2).c_str(), "%d", &q3physical_num);

            int position2 = strq3_cpuinfo.find( "," );
            sscanf(strq3_cpuinfo.substr(position2 + 2).c_str(), "%d", &q3virtual_num);
            onAddresses = true;
        }
         
    }// End while loop

    // Output OS information 
    std::cout << "C. For processor 0, answer these questions:" << std::endl;
    std::cout << "   1. Who is the vendor?                  - " << strq3_vendor << std::endl;
    std::cout << "   2. What is the model name?             - " << strq3_modelname << std::endl;
    std::cout << "   3. What is its physical address size?  - " << q3physical_num << std::endl;
    std::cout << "   4. What is its virtual address size?   - " << q3virtual_num << std::endl << std::endl;

    inq3_cpuinfo.close();
}// End Question C

/*****************************************************************************
* Function Name: QuestionD                                                   *
*     Arguments: none                                                        *
*   Description: This function carries the necessary information to answer   *
*                information rewarding processor 5 using the stat file       *
*                foung in proc/stat                                          *
*                                                                            *
* Question 3 - 1. How long has it spent time in user mode in seconds?        *
*              2. How long has it spent time in system mode in seconds?      *
*              3. How long has it been idle in seconds?                      *
*              4. How long has it been idle in days,hours,minutes & seconds? *
*****************************************************************************/
void QuestionD()
{
    char q4_cpunum[1080];

    std::string strq4_stat;
    std::string strq4_uptime;
    
    // Variables for time conversion
    float uptime;
    float qd4_secods; 
    int qd4_minutes; 
    int qd4_hrs;
    int qd4_days;
    
    // Variables for user/nice/system/idle
    int q4_usermode;
    int q4_nicemode;
    int q4_systemmode;
    int q4_idle;

    // Input file for stat
    std::ifstream inq4_stat("/proc/stat");

    // Input file for number of seconds coming from the kernel
    std::ifstream inq4_uptime ("/proc/uptime");

    // Extracting CPU uptime for final conversion 
    getline(inq4_uptime, strq4_uptime);
    sscanf(strq4_uptime.c_str(), "%f", &uptime);


    // Extracing content from input file and setting it a string
    std::getline(inq4_stat, strq4_stat); // Total CPU 
    std::getline(inq4_stat, strq4_stat); // CPU #0
    std::getline(inq4_stat, strq4_stat); // CPU #1
    std::getline(inq4_stat, strq4_stat); // CPU #2 
    std::getline(inq4_stat, strq4_stat); // CPU #3
    std::getline(inq4_stat, strq4_stat); // CPU #4 
    std::getline(inq4_stat, strq4_stat); // CPU #5

    // Placing the specific fields CPU # {0,1,2,3,4,5,6,7}, user mode, nice mode, system mode, idle 
    //    CPU: CPU # {0,1,2,3,4,5,6,7}
    //   user: normal processes executing in user mode
    //   nice: niced processes executing in user mode
    // system: processes executing in kernel mode
    //   idle: twiddling thumbs
    sscanf(strq4_stat.c_str(), "%s%d%d%d%d", q4_cpunum, &q4_usermode, &q4_nicemode, &q4_systemmode, &q4_idle);  

    // Conversion output seconds, minutes, hours, days
    qd4_days = (int)(q4_idle - qd4_secods) / (24 * 60 * 60);
    qd4_hrs = (int)(q4_idle - qd4_secods) / (60 * 60 ) % 24;
    qd4_minutes = (int)(q4_idle - qd4_secods) / 60 % 60;
    qd4_secods = fmod(q4_idle, 60.0f);

    // Output OS information 
    std::cout << "D. For processor 5, answer these questions:     CURRENT CPU - " << "{ " << q4_cpunum << " } " << std::endl;
    std::cout << "   1. How long has it spent time in user mode in seconds?   - " << q4_usermode << " seconds" << std::endl; 
    std::cout << "   2. How long has it spent time in system mode in seconds? - " << q4_systemmode << " seconds" << std::endl;
    std::cout << "   3. How long has it been idle in seconds?                 - " << q4_idle << " seconds" << std::endl;
    std::cout << "   4. How long has it been idle in days,hrs,min & seconds?  - " << qd4_days << " day" << ((qd4_days > 1 ) ? "s" : "") << 
         ", " << qd4_hrs << " hr" << ((qd4_hrs > 1 ) ? "s" : "" ) << ", " << qd4_minutes << " min"  << ((qd4_minutes > 1) ? "s" : "" ) << " and " 
              << qd4_secods << " secs " << std::endl << std::endl;

    // Closing file 
    inq4_stat.close();

}// End Question D

/*****************************************************************************
* Function Name: QuestionE                                                   *
*     Arguments: none                                                        *
*   Description: This function carries the necessary information to answer   *
*                information rewarding turing swap device                    *
*                                                                            *
* Question 4 = 1. What is the size of turing’s swap device in MB?            *
*****************************************************************************/
void QuestionE()
{
    char q5_buff[1080];
    std::string strq5_swap;
    int swapsize;

    // Input file for swap devices
    std::ifstream inq5_swap("/proc/swaps");

    // Extracting swap device (headers)
    getline(inq5_swap, strq5_swap);

    // Extracting the given swap size 
    getline(inq5_swap, strq5_swap);

    // Setting values to designated variables
    sscanf(strq5_swap.c_str(), "%s%s%d", q5_buff, q5_buff, &swapsize);
    
    std::cout << "E. What is the size of turing’s swap device in MB?          - " << (swapsize / 1024.0f) <<  "MB (in binary)" << std::endl << std::endl;

}// End Question E