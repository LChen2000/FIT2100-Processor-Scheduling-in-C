/*
  Name: Ray Krishardi Layadi
  Student ID: 26445549
  Start date: 6 September 2017
  End date: 22 September 2017
  High-level description: Refer to the introduction section of the user documentation
*/

#ifndef PROCESSOR_SCHEDULING_H
#define PROCESSOR_SCHEDULING_H

// Constants
#define BUFFER_SIZE 100 // Represent the size of the buffer
#define RR_TIME_QUANTUM 2 // Represent the time quantum for the Round Robin (RR) scheduling algorithm

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data structure that represents a process
typedef struct {
  char *pid; // Represent the process's id/name
  float arrivalTime; // Represent the process's arrival time
  float processingTime; // Represent the process's processing time
  float finishTime; // Represent the process's finish/completion time
  float turnaroundTime; // Represent the process's turnaround time
  float waitingTime; // Represent the process's waiting time
} Process;

// Functions
void startProgram(); // Start the processor scheduling program
void promptAndGetLine(char *prompt, char *line); // Prompt the user for input and get the user input
void removeTrailingNewlineChar(char *line); // Remove trailing newline character from the user input
void promptAndGetFileName(char *line, FILE *infile, char *fileName); // Prompt the user for a valid file name and get the specified file name
void promptAndGetSchedulingAlgorithm(char *line, int *schedulingAlgorithm); // Prompt the user for a valid scheduling algorithm option and get the specified scheduling algorithm option
void interpretFile(int *processCounter); // Open file specified by the input file name and interpret the process table by creating a new process and assigning the appropriate values to each process
void checkAndExecuteSchedulingAlgorithm(int schedulingAlgorithm); // Check and execute the appropriate scheduling algorithm
void calculateTurnaroundTime(Process *processes); // Calculate the turnaround time for each process
void calculateWaitingTime(Process *processes); // Calculate the waiting time for each process
float getAverageTurnaroundTime(Process *processes); // Calculate and get the average turnaround time
float getAverageWaitingTime(Process *processes); // Calculate and get the average waiting time
float getTotalProcessingTime(Process *processes); // Calculate and get the total processing time
float getAverageThroughputTime(Process *processes); // Calculate and get the overall throughput of the system (i.e. the number of processes completed per second)
void printProcessTable(float averageTurnaroundTime, float averageWaitingTime, float averageThroughputTime); // Print the output process table with the appropriate formatting
int getSmallestRemainingProcessingTimeIndex(float remainingProcessingTime[], int processCounter, int currentTime, Process *processes); // Get the index of the process that has the smallest/shortest remaining processing time at a given point of time
void executeFCFS(); // Execute First-Come-First-Served (FCFS) scheduling algorithm
void executeRR(); // Execute Round Robin (RR) scheduling algorithm
void executeSRT(); // Execute Shortest Remaining Time (SRT) scheduling algorithm
void freeMemory(); // Deallocate memory allocation pointed to by pid variable and processes array
void promptQuitProgram(char *line, int *quit, int *processCounter); // Prompt the user to check whether the user wants to continue executing other scheduling algorithms for another file or quit the program

#endif