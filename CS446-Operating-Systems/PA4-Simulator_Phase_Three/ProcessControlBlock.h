#ifndef PROCESS_CONTROL_BLOCK_H
#define PROCESS_CONTROL_BLOCK_H

#include <chrono>
#include <time.h>
#include <thread>
#include "ConfigurationSettings.h"
#include "InterruptSystem.h"
#include "Logger.h"
#include "Structs.h"

using namespace std;

extern Logger myLog;

/**
 * Used for creating new threads.
 * Converts given parameter to microseconds
 * Then calls usleep
 *
 * Pre: None
 * Post: Program will wait sleepTime milliseconds
 * 
 * @param sleepTimeInMilliSec Sleep time given in milliseconds
 */
//static void createThreadThatSleeps(
//	unsigned int sleepTimeInMilliSec );

class ProcessControlBlock
{
public:
	/**
	 * Default constructor
	 * Should only be used for temporary PCB's
	 */
	ProcessControlBlock( );
	/**
	 * ProcessControlBlock constructor
	 * Updates this PCB's processNumber and various numberOfCycles settings
	 * - ProcessCycleTime
	 * - MonitorDisplayTime
	 * - HardDriveCycleTime
	 * - PrinterCycleTime
	 * - KeyboardCyelTime
	 *
	 * Pre: None
	 * Post: Will update all settings necessary for this object to function
	 * correctly.
	 */
	ProcessControlBlock( 
		unsigned int pProcessNumber, 
		const ConfigurationSettings &pSettings );
	/**
	 * = Operator overload
	 */
	ProcessControlBlock& operator=(const ProcessControlBlock& rhs);

	/**
	* Iterates through each process in the PCB,
	* logs the process, waits, then logs the end of the
	* process.
	*
	* Pre: Processes will have been fully loaded into the
	*      readyProcessThreads vector
	* Post: readyProcessThreads will be empty and finishedProcessThreads
	*       will be filled with all the processes from readyProcessThreads
	*/
	void runApplication();

	/**
	* Add a new instruction to this PCB
	*
	* Pre: The new instruction was initialized correctly
	* Post: This PCB will have one more instruction in it
	*
	* @param newInstruction Either a Processor, Input, or Output action
	* 			with a number of cycles and a specific operation
	*/
	void addInstruction(Process newInstruction);

	/**
	* Calculates the remaining time of the active processes in this PCB
	* If the remaining time has already been calculated and no new process
	* has been run / added, will use the last calculated time instead of
	* recalculating
	*
	* @return 0 if there are no more processes, or >0 dependent on the
	*  number of instructions and their run times
	*/
	unsigned int getRemainingTime();
	
	unsigned int processNumber;
private:
	/**
	 * Starts a new process
	 * Sets sleep time to processCycleTime * numberOfCycles
	 * Version 3.0: Checks if there are any interrupts in queue
	 * before waiting.
	 * If completes its own quantum, will throw a quantum interrupt
	 * into the queue.
	 *
	 * Pre: processCycleTime has been set appropriately
	 * Post: Program will create and wait for a processor thread
	 * Will also log output for start/end of process
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newProcess( 
		string operation, 
		unsigned int &numberOfCycles );

	/**
	 * Creates a new Processor Thread
	 * Sets sleep time to 
	 *  hardDriveCycleTime * numberOfCycles
	 *  keyboardCycleTime * numberOfCycles
	 * based on the given operation
	 * Creates new thread, then waits for thread to end.
	 *
	 * Pre: hardDriveCycleTime and keyboardCycleTime
	 * have been set appropriately
	 * Post: Program will create and wait for a input thread
	 * Will also log output for start/end of process
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newInputThread( 
		string operation, 
		unsigned int numberOfCycles );

	/**
	 * Creates a new Output Thread
	 * Sets sleep time to 
	 *  hardDriveCycleTime * numberOfCycles
	 *  monitorDisplayTime * numberOfCycles
	 *  printerCycleTime * numberOfCycles
	 * based on the given operation
	 * Creates new thread, then waits for thread to end.
	 *
	 * Pre: hardDriveCycleTime, monitorDisplayTime, printerCycleTime
	 * have been set appropriately
	 * Post: Program will create and wait for an output thread
	 * Will also log output for start/end of process
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newOutputThread( 
		string operation, 
		unsigned int numberOfCycles );

	void runIO();

	void runProcess();

	
	bool needToRecalcRT; // Used to determine whether the remaining time needs to be recalculated
	unsigned int remainingTime;
	unsigned int quantumCycles;
	unsigned int processCycleTime;
	unsigned int monitorDisplayTime;
	unsigned int hardDriveCycleTime;
	unsigned int printerCycleTime;
	unsigned int keyboardCycleTime;
	vector<pcb_thread> readyProcessThreads;
};
#endif // PROCESS_CONTROL_BLOCK_H