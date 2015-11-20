# CS 446: Principles of Operating Systems

Fall 2015
Taught my Dr. Michael Levrington

### PA1: Family Simulation (Graduate student version)

Simulate a family tree by using process creation and deletion system calls. Given an input file with a list of parents and children (of the format <parent 1> <parent 2> <child 1> ... <child n>), output the correct family tree.

Example input:
```
Adam BB Casey Donna X
Donna Yong Mary Emily
Mary Frank
Casey Paul Karen
```

Example output:
```
Adam(0)-BB
	Casey(839)-Paul
			Karen(857)
	Donna(839)-Yong
		Mary(906)-Frank
		Emily(906)
	X(839)
```

Rules:
* Parent process:
 * New process must be created for each of a parent's children
* Child process:
 * Check if the present name is getting married to another person
 * Print the present name and the partner's name as <name>-<parent's name>
 * Must output parent's process ID

 Overall, program was a nice introduction to threads and a good warm up for the programming assignments that were to come. It was also the first program that I wrote entirely in C that wasn't related to embedded systems.
 (This initially gave me enough confidence to write all of the simulators in pure C as well,
  but then I remembered that C doesn't have the same robust STL as CPP)
 
## Simulator Project Overview
Throughout the semester, we completed 3 simulator projects (each one building on the other).
The simulator accepted a configuration file and a meta-data file, and then simulated various CPU
scheduling algorithms. 
 
Sample configuration file (from version 3.0):
```
Start Simulator Configuration File
Version/Phase: 3.0
File Path: Test_3.mdf
CPU Scheduling Code: RR
Quantum Time (cycles): 3
Processor cycle time (msec): 10
Monitor display time (msec): 20
Hard drive cycle time (msec): 15
Printer cycle time (msec): 25
Keyboard cycle time (msec): 50
Log: Log to Both
Log File Path: logfile_1.lgf
End Simulator Configuration File
 ```
 
 Sample meta-data file
 ```
 Start Program Meta-Data Code:
S(start)0; A(start)0; I(keyboard)5; O(hard drive)14; P(run)11;
P(run)11; I(hard drive)14; A(end)0; A(start)0; P(run)8; 
O(monitor)8; I(keyboard)7; P(run)13; O(hard drive)6; A(end)0; 
A(start)0; I(hard drive)5; P(run)5; O(hard drive)5; 
I(keyboard)15; O(hard drive)10; A(end)0; A(start)0; P(run)7; 
P(run)15; I(hard drive)14; O(monitor)5; P(run)13; A(end)0; 
A(start)0; I(keyboard)10; O(monitor)11; I(hard drive)10; 
O(monitor)13; I(hard drive)14; A(end)0; S(end)0.
End Program Meta-Data Code.
 ```
 * S: Operating System
 * A: Application / Process
 * P: Processor operation
 * I: Input operation
 * O: Output operation
 
## PA2: Simulator 1.0
 For the first simulator, only one total Process/Application exists (thus, no CPU scheduling to implement, just create the building blocks for future simulators)
 
 The requirements for this simulator were simple. However, we were given the general requirements for future simulators, so we could plan for future simulators.

## PA3: Simulator 2.0 (Graduate version)
 For the second simulator, there is no maximum to the number of Processes/Applications than can exist. CPU Scheduling will only be nonpreemptive (IE, once a process has started executing its operations, it will not stop until it has finished all of its operations)
 CPU Scheduling methods required:
 * FIFO
  * Iterate through the processes in the order that the came in
 * STF
  * At the beginning of the simulation, sort the processes by their total time. Then iterate through them in that order
 * SRTF
  * When choosing a process to run, choose the process with the shortest remaining time
  * This will end with the same output as STF, but the selection of processes must be done throughout the simulation rather than at the very beginning only

## PA4: Simulator 3.0 (Graduate version)
 For the third simulator, there is a gain no maximum to the number of Processes/Applications that can exist. CPU Scheduling will be preemptive.
 Rules for processes:
 * The configuration file will specify a given quantum
 * For each process, the current operation will be handled in the following ways
  * If the operation is input or output, the operation will be put into its own thread and that process will be put into the blocked queue until the thread has completed. Once it has been completed, it will throw an interrupt
  * If the operation is a processor operation, the operation will be run for the lowest amount of the following cycles:
   * Quantum amount of cycles
   * Number of cycles remaining for this processor operation
   * Number of cycles (potentially 0) until an interrupt occurs
  * If the processor runs for the number of quantum cycles, it should report back that there was a quantum interrupt
  * If the processor finishes its number of cycles, it should be removed from the list of operations left for its process
 * After running the current application, immediately check for interrupts and complete all interrupts before running the next application
 * If there are no applications available to run, and there are no interrupts available, but there are still processes in the blocked queue, report that the CPU is idling

 The following CPU Scheduling methods must be implemented:
 * Round Robin (RR): No sorting or priority of the processes will occur. The next process in the queue will run for its amount, then it will be put in the blocked queue or the back of the ready queue as necessary. Then run the next process
 * FIFO-P: Priority will be given to the processes based on the time they entered the simulator
 * SRTF-P: Priority will be given to the proecesses based on their remaining time left

## Overall Takeaways

 This simulator was a great learning experience, not only from an Operating System CPU Scheduling standpoint, but also in more thoroughly learning various data structures to compare time/space complexities, software design plans, and OOP implementations.

 Features I'm proud of:
 * Used RegEx library for reading in files
  * Gave me a much greater control over the exact requirements of the file structure, which would have required complex if structures otherwise
  * Was a huge pain because the version of gcc we used (4.8) didn't have RegEx fully implmented, so I had to find an outside library and then also learn about static vs dynamic libraries
 * Global logging system
 * Global interrupt system
 * OOP Design
  * OOP Design makes the code rather self-documenting and easy to see what is going on. There is a small main method that shows the overall program structure, with the action occurring behind the scenes
  * Modular design allows for easy method changes / replacements. As long as the interface is satisfied, nothing but the bugged method needs to be changed
 * Robust error reporting
  * Nearly every place that the program can go wrong has an associated error message notifying where and when it went wrong
  * Made debugging a lot easier

 Problems / Lessons for next time:
 * Planned in too small of chunks
  * Planning occurred as a sequence of "What's the next component needed to continue the program? Okay, let's figure that part out and then implement it"
  * Should have focused on an "overall program structure" at the very beginning with a general plan of how the pieces would fit together, or at the very least figure out a "program core" 
 * Unnecessary redundant data
  * While it wasn't that large of a problem, my initial design of a ProcessControlBlock that contained all of its own processes along with all of its needed program settings caused a lot of repeated data in later phases
  * The current system would be useful for a theoretical system in which each PCB can have different settings, but for this simulator it is guaranteed that all PCBs will have the same settings.
  * Overall lesson: Don't plan for theoretical situations unless there is actually a chance of them becoming real
 * Clumsy communication between OperatingSystem and ProcessControlBlock
  * These two systems should have had very tight communication abilities. However, until Sim3, they essentially had no communication except for the operating system telling the PCB what to do. 
 * Trying to do "Too much, too fast"
  * Similar to the planning issue above, I often tried to implement several features at the same time, making it harder to figure out exactly where something went wrong
  * This also created a lot of bad commits that would have been useless to roll back to (They got the code into the system, but they weren't useful from a "Most recent feature broke, let's roll back to the last working version" standpoint