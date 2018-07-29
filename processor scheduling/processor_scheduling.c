/*
  Name: Ray Krishardi Layadi
  Student ID: 26445549
  Start date: 6 September 2017
  End date: 22 September 2017
  High-level description: Refer to the introduction section of the user documentation
*/

#include "processor_scheduling.h"

// Global Variables
int quit = 0; // Represent whether the user wants to quit the program or not
char line[BUFFER_SIZE]; // Represent the user input
FILE *infile; // Represent the input file
char fileName[BUFFER_SIZE]; // Represent the input file name
int schedulingAlgorithm; // Represent the scheduling algorithm to be executed (1->FCFS, 2->RR, 3->SRT)
int processCounter = 0; // Represent the number of processes
Process *processes; // Represent an array of processes

// Function that starts the processor scheduling program
void startProgram() {
  // Keep iterating until the user has chosen to quit the program
  while (!quit) {
    interpretFile(&processCounter); // Prompt the user for a valid file name and scheduling algorithm option and interpret the process table inside the file
    checkAndExecuteSchedulingAlgorithm(schedulingAlgorithm); // Check and execute the appropriate scheduling algorithm
    freeMemory(); // Deallocate memory allocation pointed to by pid variable and processes array
    promptQuitProgram(line, &quit, &processCounter); // Prompt the user to check whether the user wants to continue executing other scheduling algorithms for another file or quit the program
  }
}

// Function that prompts the user for input and get the user input
void promptAndGetLine(char *prompt, char *line) {
  printf("%s", prompt);
  fgets(line, BUFFER_SIZE, stdin);
}

// Function that removes trailing newline character from the user input ("line" variable) as a result of the fgets()
// 1. Replace the newline character with null character if there is a newline character at the end of the "line" variable
void removeTrailingNewlineChar(char *line) {
  size_t lineLength = strlen(line);
  if (line[lineLength - 1] == '\n')
    line[lineLength - 1] = '\0';
}

// Function that prompts the user for a valid file name and get the specified file name
void promptAndGetFileName(char *line, FILE *infile, char *fileName) {
  char *prompt = "Enter file name: "; // Represent the prompt statement
  int validFile = 0; // Represent whether a file exists with the given file name

  // Keep iterating until a file exists with the given file name
  while (!validFile) {
    promptAndGetLine(prompt, line); // Prompt the user for input and get the user input
    removeTrailingNewlineChar(line); // Remove trailing newline character from the user input

    // Check whether a file exists with the given file name
    // If a file does NOT exist with the given file name then print the appropriate error message
    // If a file exists with the given file name then get the file name and exit the loop
    if ((infile = fopen(line, "r")) == NULL)
      printf("%s: No such file\n", line);
    else {
      validFile = 1;
      strcpy(fileName, line);
    }
  }

  // Close the input file
  fclose(infile);
}

// Function that prompts the user for a valid scheduling algorithm option and get the specified scheduling algorithm option
// Valid scheduling algorithm options:
// 1 -> First-Come-First-Served (FCFS)
// 2 -> Round Robin (RR)
// 3 -> Shortest Remaining Time (SRT)
void promptAndGetSchedulingAlgorithm(char *line, int *schedulingAlgorithm) {
  // Represent the prompt statement
  char *prompt = "Select scheduling algorithm:\n1. First-Come-First-Served\n2. Round Robin\n3. Shortest Remaining Time\nSelect [1-3]: ";
  int validSchedulingAlgorithm = 0; // Represent whether the given scheduling algorithm option is valid or not

  // Keep iterating until the user has selected a valid scheduling algorithm option
  while (!validSchedulingAlgorithm) {
    promptAndGetLine(prompt, line); // Prompt the user for input and get the user input
    removeTrailingNewlineChar(line); // Remove trailing newline character from the user input
    *schedulingAlgorithm = atoi(line); // Get the user input as an integer

    // Check whether the user has selected a valid scheduling algorithm option
    // If the user has selected a valid scheduling algorithm option then exit the loop
    // If the user has NOT selected a valid scheduling algorithm option then print the appropriate error message
    if (*schedulingAlgorithm >= 1 && *schedulingAlgorithm <= 3)
      validSchedulingAlgorithm = 1;
    else
      printf("Invalid scheduling algorithm option\n");
  }
}

// Function that performs the following:
// 1. Prompt the user for a valid file name and scheduling algorithm option
// 2. Open the file specified by the input file name and interpret the process table by creating a new process
//    and assigning the appropriate values (i.e. pid, arrival time, and processing time) to each process
void interpretFile(int *processCounter) {
  char fileLine[BUFFER_SIZE]; // Represent each line in the input file
  char *fileLineArray[BUFFER_SIZE]; // Represent the pid, arrival time, and processing time in each line
  int fileLineArrayCounter; // Represent the index of the pid, arrival time, and processing time in each line (i.e. 0 -> pid, 1 -> arrival time, 2 -> processing time)
  char *token; // Represent each substring (i.e. pid, arrival time, and processing time) in each line ("fileLine" variable) delimited/separated by tab ("\t")

  promptAndGetFileName(line, infile, fileName); // Prompt the user for a valid file name and get the specified file name
  promptAndGetSchedulingAlgorithm(line, &schedulingAlgorithm); // Prompt the user for a valid scheduling algorithm option and get the specified scheduling algorithm option

  processes = (Process *) malloc(sizeof(Process)); // Allocate memory in heap to store the processes

  // Open the file specified by the input file name for reading
  infile = fopen(fileName, "r");

  // Keep iterating until there is no more line to be read from the input file
  while (fgets(fileLine, BUFFER_SIZE, infile)) {
    processes = (Process *) realloc(processes, sizeof(Process) * (*processCounter+1)); // Dynamically increase the size of the processes array depending on the number of processes in the file
    removeTrailingNewlineChar(fileLine); // Remove trailing newline character from the file line
    fileLineArrayCounter = 0; // Assign the index to the index of the pid (0 -> pid) to be used in the 1st iteration
    token = strtok(fileLine, "\t"); // Get the pid from the file line by using tab ("\t") as the delimiter

    // Keep iterating until there is no more substring to be retrieved from the file line
    while (token != NULL) {
      fileLineArray[fileLineArrayCounter] = token; // Store the substring (i.e. pid in 1st iteration, arrival time in 2nd iteration, and processing time in 3rd iteration) in the array depending on the current index
      fileLineArrayCounter++; // Increment the index to the index of the arrival time to be used in the 2nd iteration and subsequently increment the index to the index of the processing time to be used in the 3rd iteration
      token = strtok(NULL, "\t"); // Get the next substring (i.e. arrival time to be stored in 2nd iteration and processing time to be stored in 3rd iteration) by using tab ("\t") as the delimiter
    }

    processes[*processCounter].pid = (char *) malloc(sizeof(char)*strlen(fileLineArray[0])+1); // Dynamically allocate memory in heap to store the pid based on the length/size of the pid
    strcpy(processes[*processCounter].pid, fileLineArray[0]); // Copy the content of the pid to the newly created process object's pid in the processes array
    processes[*processCounter].arrivalTime = atof(fileLineArray[1]); // Assign the arrival time to the newly created process object's arrival time in the processes array
    processes[*processCounter].processingTime = atof(fileLineArray[2]); // Assign the processing time to the newly created process object's processing time in the processes array

    *processCounter += 1; // Increment the number of processes accordingly
  }

  // Close the input file
  fclose(infile);
}

// Function that checks and executes the appropriate scheduling algorithm based on the selected scheduling algorithm option
// Valid scheduling algorithm options:
// 1 -> First-Come-First-Served (FCFS)
// 2 -> Round Robin (RR)
// 3 -> Shortest Remaining Time (SRT)
void checkAndExecuteSchedulingAlgorithm(int schedulingAlgorithm) {
  if (schedulingAlgorithm == 1)
    executeFCFS();
  else if (schedulingAlgorithm == 2)
    executeRR();
  else if (schedulingAlgorithm == 3)
    executeSRT();
}

// Function that calculates the turnaround time for each process
// Formula -> turnaround time = finish time - arrival time
void calculateTurnaroundTime(Process *processes) {
  for (int i = 0; i < processCounter; i++)
    processes[i].turnaroundTime = processes[i].finishTime - processes[i].arrivalTime;
}

// Function that calculates the waiting time for each process
// Formula -> waiting time = turnaround time - processing time
void calculateWaitingTime(Process *processes) {
  for (int i = 0; i < processCounter; i++)
    processes[i].waitingTime = processes[i].turnaroundTime - processes[i].processingTime;
}

// Function that calculates and returns the average turnaround time
// Formula -> average turnaround time = total turnaround time / number of processes
float getAverageTurnaroundTime(Process *processes) {
  float totalTurnaroundTime = 0;
  for (int i = 0; i < processCounter; i++)
    totalTurnaroundTime += processes[i].turnaroundTime;
  return (totalTurnaroundTime / processCounter);
}

// Function that calculates and returns the average waiting time
// Formula -> average waiting time = total waiting time / number of processes
float getAverageWaitingTime(Process *processes) {
  float totalWaitingTime = 0;
  for (int i = 0; i < processCounter; i++)
    totalWaitingTime += processes[i].waitingTime;
  return (totalWaitingTime / processCounter);
}

// Function that calculates and returns the total processing time
float getTotalProcessingTime(Process *processes) {
  float totalProcessingTime = 0;
  for (int i = 0; i < processCounter; i++)
    totalProcessingTime += processes[i].processingTime;
  return totalProcessingTime;
}

// Function that calculates and returns the overall throughput of the system (i.e. the number of processes completed per second)
// Formula -> throughput = number of processes / total processing time
float getAverageThroughputTime(Process *processes) {
  float totalProcessingTime = getTotalProcessingTime(processes);
  return (processCounter / totalProcessingTime);
}

// Function that prints the output process table with the appropriate formatting
// Output process table includes the following:
// 1. Turnaround time and waiting time for each process
// 2. Average turnaround time and average waiting time for all the processes
// 3. Overall throughput of the system (i.e. the number of processes completed per second)
// Note: All float values will be rounded to 2 decimal places
void printProcessTable(float averageTurnaroundTime, float averageWaitingTime, float averageThroughputTime) {
  // Print each process's pid with the appropriate formatting
  printf("\n\t\t");
  for (int i = 0; i < processCounter; i++) {
    printf("%s\t", processes[i].pid);
  }
  printf("Mean\n");

  // Print each process's turnaround time and the average turnaround time for all the processes with the appropriate formatting
  printf("Turnaround Time\t");
  for (int i = 0; i < processCounter; i++) {
    printf("%.2f\t", processes[i].turnaroundTime);
  }
  printf("%.2f\n", averageTurnaroundTime);

  // Print each process's waiting time and the average waiting time for all the processes with the appropriate formatting
  printf("Waiting Time\t");
  for (int i = 0; i < processCounter; i++) {
    printf("%.2f\t", processes[i].waitingTime);
  }
  printf("%.2f\n", averageWaitingTime);

  // Print the overall throughput of the system (i.e. the number of processes completed per second) with the appropriate formatting
  printf("Overall throughput of the system: %.2f jobs/second\n\n", averageThroughputTime);
}

// Function that executes the First-Come-First-Served (FCFS) scheduling algorithm
void executeFCFS() {
  // Represent the average turnaround time, average waiting time, and throughput
  float averageTurnaroundTime, averageWaitingTime, averageThroughputTime;

  // Calculate the finish time for each process
  // In the case of FCFS, finish time for each process could be easily computed by iteratively adding the processing time
  // since all processes have been sorted in ascending order by their arrival time
  float processFinishTime = 0;
  for (int i = 0; i < processCounter; i++) {
    processFinishTime += processes[i].processingTime;
    processes[i].finishTime = processFinishTime;
  }

  // Calculate the turnaround time for each process and get the average turnaround time
  calculateTurnaroundTime(processes);
  averageTurnaroundTime = getAverageTurnaroundTime(processes);

  // Calculate the waiting time for each process and get the average waiting time
  calculateWaitingTime(processes);
  averageWaitingTime = getAverageWaitingTime(processes);

  // Calculate and get the overall throughput of the system (i.e. the number of processes completed per second)
  averageThroughputTime = getAverageThroughputTime(processes);

  // Print the output process table
  printProcessTable(averageTurnaroundTime, averageWaitingTime, averageThroughputTime);
}

// Function that executes the Round Robin (RR) scheduling algorithm
void executeRR() {
  // Represent the average turnaround time, average waiting time, throughput, and total processing time
  float averageTurnaroundTime, averageWaitingTime, averageThroughputTime, totalProcessingTime = 0;
  float remainingProcessingTime[processCounter]; // Represent the remaining processing time for each process
  int queueCounter = 0, executionTime = 0; // Represent the number of processes in the queue and the execution time
  int queue[processCounter]; // Represent queue of processes

  // Initialise the remaining processing time of each process and calculate the total processing time
  for (int i = 0; i < processCounter; i++) {
    remainingProcessingTime[i] = processes[i].processingTime;
    totalProcessingTime += processes[i].processingTime;
  }

  // Calculate the finish time for each process by looping through each period of time starting from 0
  // to the last finish time which is equivalent to the total processing time
  for (int currentTime = 0; currentTime <= totalProcessingTime; currentTime++) {
    // Check for new processes that have arrived at current time to be added to the queue
    if (currentTime <= processes[processCounter-1].arrivalTime) {
      // Loop through all processes
      for (int i = 0; i < processCounter; i++) {
        // Check whether there is any new process that has arrived at current time
        // If there is a new process that has arrived at current time (i.e. arrival time is equal to current time)
        // then add the index of the new process to the queue, increment the number of processes in the queue accordingly, and exit the loop
        if (currentTime == processes[i].arrivalTime) {
          queue[queueCounter] = i;
          queueCounter++;
          break;
        }
      }
    }

    // Check the remaining processing time of the next process to be executed and the execution time
    // If the remaining processing time of the next process to be executed is greater than or equal to the time quantum (2) and the execution time is also equal to the time quantum (2)
    if (remainingProcessingTime[queue[0]] >= RR_TIME_QUANTUM && executionTime == RR_TIME_QUANTUM) {
      // If the next process to be executed still require some processing time after its execution
      // then add the process to the back of the queue and increment the number of processes in the queue accordingly
      if ((remainingProcessingTime[queue[0]]-RR_TIME_QUANTUM) > 0) {
        queue[queueCounter] = queue[0];
        queueCounter++;
      }

      // Decrement the process's remaining processing time by the length of the time quantum and update the process's finish time to the current time
      remainingProcessingTime[queue[0]] -= RR_TIME_QUANTUM;
      processes[queue[0]].finishTime = currentTime;

      // Perform left shift on the queue (imitate the behavior of pop at index 0) to update the queue and the next process to be executed
      for (int i = 0; i < queueCounter; i++) {
        queue[i] = queue[i+1];
      }

      queueCounter--; // Decrement the number of processes in the queue accordingly
      executionTime = 0; // Reset the execution time back to 0
    }
    // If the remaining processing time of the next process to be executed is equal to 1 and the execution time is also equal to 1
    else if (remainingProcessingTime[queue[0]] == 1 && executionTime == 1) {
      // Decrement the process's remaining processing time by 1 and update the process's finish time to the current time
      remainingProcessingTime[queue[0]] -= 1;
      processes[queue[0]].finishTime = currentTime;

      // Perform left shift on the queue (imitate the behavior of pop at index 0) to update the queue and the next process to be executed
      // In this case, the process is NOT added back to the queue since the remaining processing time of the process is already 0
      for (int i = 0; i < queueCounter; i++) {
        queue[i] = queue[i+1];
      }

      queueCounter--; // Decrement the number of processes in the queue accordingly
      executionTime = 0; // Reset the execution time back to 0
    }

    // Increment the execution time accordingly
    executionTime++;
  }

  // Calculate the turnaround time for each process and get the average turnaround time
  calculateTurnaroundTime(processes);
  averageTurnaroundTime = getAverageTurnaroundTime(processes);

  // Calculate the waiting time for each process and get the average waiting time
  calculateWaitingTime(processes);
  averageWaitingTime = getAverageWaitingTime(processes);

  // Calculate and get the overall throughput of the system (i.e. the number of processes completed per second)
  averageThroughputTime = getAverageThroughputTime(processes);

  // Print the output process table
  printProcessTable(averageTurnaroundTime, averageWaitingTime, averageThroughputTime);
}

// Function that returns the the index of the process that has the smallest/shortest remaining processing time at a given point of time
// Note: This function is used for the SRT scheduling algorithm
int getSmallestRemainingProcessingTimeIndex(float remainingProcessingTime[], int processCounter, int currentTime, Process *processes) {
  // Represent the index of a process that has the smallest/shortest remaining processing time at a given point of time
  int smallestRemainingProcessingTimeIndex = -1;

  // Loop through all processes
  for (int i = 0; i < processCounter; i++) {
    // If there is a process that has a remaining processing time greater than 0
    // then assume that process index has the smallest remaining processing time and exit the loop
    if (remainingProcessingTime[i] > 0) {
      smallestRemainingProcessingTimeIndex = i;
      break;
    }
  }

  // Loop through all processes starting from the process next to the process that is assumed to have the smallest remaining processing time
  for (int i = smallestRemainingProcessingTimeIndex+1; i < processCounter; i++) {
    // If the process has arrived at current time and has a remaining processing time greater than 0
    if (currentTime >= processes[i].arrivalTime && remainingProcessingTime[i] != 0) {
      // If the remaining processing time of that process is smaller than the process that is assumed to have the smallest remaining processing time
      // then assign the index of that process as the process that is assumed to have the smallest remaining processing time
      if (remainingProcessingTime[i] < remainingProcessingTime[smallestRemainingProcessingTimeIndex]) {
        smallestRemainingProcessingTimeIndex = i;
      }
    }
  }

  // Return the index of the process that has the smallest/shortest remaining processing time at a given point of time
  return smallestRemainingProcessingTimeIndex;
}

// Function that executes the Shortest Remaining Time (SRT) scheduling algorithm
void executeSRT() {
  // Represent the average turnaround time, average waiting time, throughput, and total processing time
  float averageTurnaroundTime, averageWaitingTime, averageThroughputTime, totalProcessingTime = 0;
  float remainingProcessingTime[processCounter]; // Represent the remaining processing time for each process
  int smallestRemainingProcessingTimeIndex; // Represent the index of a process that has the smallest/shortest remaining processing time at a given point of time

  // Initialise the remaining processing time of each process and calculate the total processing time
  for (int i = 0; i < processCounter; i++) {
    remainingProcessingTime[i] = processes[i].processingTime;
    totalProcessingTime += processes[i].processingTime;
  }

  // Calculate the finish time for each process by looping through each period of time starting from 0
  // to the (last finish time - 1) which is equivalent to the (total processing time - 1)
  for (int currentTime = 0; currentTime < totalProcessingTime; currentTime++) {
    // Get the index of the process that has the smallest/shortest remaining processing time at a given point of time
    smallestRemainingProcessingTimeIndex = getSmallestRemainingProcessingTimeIndex(remainingProcessingTime, processCounter, currentTime, processes);

    // Decrement the remaining processing time of that process accordingly
    remainingProcessingTime[smallestRemainingProcessingTimeIndex] -= 1;

    // If the remaining processing time of that process is now equal to 0 then update the process's finish time to (current time + 1)
    if (remainingProcessingTime[smallestRemainingProcessingTimeIndex] == 0) {
      processes[smallestRemainingProcessingTimeIndex].finishTime = currentTime+1;
    }

  }

  // Calculate the turnaround time for each process and get the average turnaround time
  calculateTurnaroundTime(processes);
  averageTurnaroundTime = getAverageTurnaroundTime(processes);

  // Calculate the waiting time for each process and get the average waiting time
  calculateWaitingTime(processes);
  averageWaitingTime = getAverageWaitingTime(processes);

  // Calculate and get the overall throughput of the system (i.e. the number of processes completed per second)
  averageThroughputTime = getAverageThroughputTime(processes);

  // Print the output process table
  printProcessTable(averageTurnaroundTime, averageWaitingTime, averageThroughputTime);
}

// Function that deallocates memory allocation pointed to by pid variable and processes array
void freeMemory() {
  for (int i = 0; i < processCounter; i++)
    free(processes[i].pid);
  free(processes);
}

// Function that prompts the user to check whether the user wants to continue executing other scheduling algorithms for another file or quit the program
void promptQuitProgram(char *line, int *quit, int *processCounter) {
  char *prompt = "Do you wish to quit the program? [y/n]: "; // Represent the prompt statement
  int validResponse = 0; // Represent whether the given response is valid or not (response must be either "y" or "n")

  // Keep iterating until the user has provided a valid response (response must be either "y" or "n")
  while (!validResponse) {
    promptAndGetLine(prompt, line); // Prompt the user for input and get the user input
    removeTrailingNewlineChar(line); // Remove trailing newline character from the user input

    // Check whether the user wants to continue executing other scheduling algorithms for another file or quit the program
    // If the user wants to quit the program then exit the loop and program
    // If the user wants to continue executing other scheduling algorithms for another file then clear the screen, reset the number of processes, and exit the loop
    if (strcmp(line, "y") == 0) {
      *quit = 1;
      validResponse = 1;
    }
    else if (strcmp(line, "n") == 0) {
      printf("\e[1;1H\e[2J");
      *processCounter = 0;
      validResponse = 1;
    }
  }
}