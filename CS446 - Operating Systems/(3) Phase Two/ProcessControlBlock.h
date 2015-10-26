#ifndef PROCESS_CONTROL_BLOCK_H
#define PROCESS_CONTROL_BLOCK_H

#include <time.h>
#include <thread>
#include <unistd.h>
#include "Logger.h"
#include "ConfigurationSettings.h"

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
//	int sleepTimeInMilliSec );

class ProcessControlBlock
{
public:
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
		int pProcessNumber, 
		const ConfigurationSettings &pSettings );

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
	void runProcesses();
	
	/**
	 * Creates a new Processor Thread
	 * Sets sleep time to processCycleTime * numberOfCycles
	 * Creates new thread, then waits for thread to end.
	 *
	 * Pre: processCycleTime has been set appropriately
	 * Post: Program will create and wait for a processor thread
	 * Will also log output for start/end of process
	 * 
	 * @param operation Name of the operation, used for output
	 * @param numberOfCycles Number of cycles to run for
	 */
	void newProcessThread( 
		string operation, 
		int numberOfCycles );

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
		int numberOfCycles );

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
		int numberOfCycles );

	/**
	* Struct os_thread will be used to hold all the
	* threads that will be run throughout this process.
	* The name of the operation will be saved, as will
	* the total time it will take for the process to
	* run.
	*
	* @param operation The name of the operation
	* @param processTime Total process time
		                   (numberOfCycles * cycleTime)
	*/
	struct os_thread
	{
		string startLogMessage;
		string endLogMessage;
		int processTime;
	};

	int processNumber;
	int processCycleTime;
	int monitorDisplayTime;
	int hardDriveCycleTime;
	int printerCycleTime;
	int keyboardCycleTime;
	vector<os_thread> readyProcessThreads;
	vector<os_thread> finishedProcessThreads;
};
#endif // PROCESS_CONTROL_BLOCK_H