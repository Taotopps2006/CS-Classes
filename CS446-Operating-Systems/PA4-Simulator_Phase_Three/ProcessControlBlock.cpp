#include "ProcessControlBlock.h"

static void createNonPremptiveThread( 
	unsigned int sleepTimeInMilliSec )
{
	this_thread::sleep_for(chrono::milliseconds(sleepTimeInMilliSec));
}

static void createPremptiveThread(
	unsigned int sleepTimeInMilliSec,
	unsigned int pcbID,
	InterruptSystem &interruptQueue )
{
	this_thread::sleep_for(chrono::milliseconds(sleepTimeInMilliSec));
	interruptQueue.addNewInterrupt(InterruptType::IO, pcbID);
}

ProcessControlBlock::runProcess( PcbThread currentProcess )
{
	bool interruptTriggered = (interruptQueue.size() != 0);
	while(currentProcess.numCyclesRemaining != 0 && !interruptTriggered)
	{
		
	}
}

ProcessControlBlock::ProcessControlBlock( )
{
	needToRecalcRT = true;
	remainingTime = 0;
	processNumber = 0;
	processCycleTime = 0;
	monitorDisplayTime = 0;
	hardDriveCycleTime = 0;
	printerCycleTime = 0;
	keyboardCycleTime = 0;
}

ProcessControlBlock::ProcessControlBlock( 
	unsigned int pProcessNumber, 
	const ConfigurationSettings & pSettings )
{
	needToRecalcRT = true;
	remainingTime = 0;
	processNumber = pProcessNumber;
	quantumCycles = pSettings.quantumCycles;
	processCycleTime = pSettings.processCycleTime;
	monitorDisplayTime = pSettings.monitorDisplayTime;
	hardDriveCycleTime = pSettings.hardDriveCycleTime;
	printerCycleTime = pSettings.printerCycleTime;
	keyboardCycleTime = pSettings.keyboardCycleTime;
}

ProcessControlBlock& ProcessControlBlock::operator=(const ProcessControlBlock& rhs)
{
	needToRecalcRT = rhs.needToRecalcRT;
	remainingTime = rhs.remainingTime;
	processNumber = rhs.processNumber;
	processCycleTime = rhs.processCycleTime;
	monitorDisplayTime = rhs.monitorDisplayTime;
	hardDriveCycleTime = rhs.hardDriveCycleTime;
	printerCycleTime = rhs.printerCycleTime;
	keyboardCycleTime = rhs.keyboardCycleTime;
	readyProcessThreads = vector<pcb_thread>(rhs.readyProcessThreads);
	finishedProcessThreads = vector<pcb_thread>(rhs.finishedProcessThreads);
	return *this;
}

void ProcessControlBlock::runApplication()
{
	needToRecalcRT = true;
	// Check that there are threads to run
	if(readyProcessThreads.size() == 0)
	{
		myLog.logError( "This ProcessControlBlock has no processes, but was asked to run its threads" );
	}
	// Iterate through each thread
	// Process thread, then dump from ready queue to finished queue
	while(readyProcessThreads.size() != 0)
	{
		// Log start process
		myLog.logProcess( readyProcessThreads.front().startLogMessage );
		// Sleep
		thread process( createThreadThatSleeps, readyProcessThreads.front().processTime );
		process.join();
		// Log end process
		myLog.logProcess( readyProcessThreads.front().endLogMessage );
		// Push front of ready to finished, then pop it off of ready
		finishedProcessThreads.push_back( readyProcessThreads.front( ) );
		readyProcessThreads.erase( readyProcessThreads.begin( ) );
	}
}

unsigned int ProcessControlBlock::getRemainingTime()
{
	// If we don't need to recalculate, save time by returning the
	// last calculated remainingTime
	if(needToRecalcRT == false)
	{
		return remainingTime;
	}

	// Otherwise, reset the remaining time and iterate through each
	// process and add the process time to our remaining time
	// Set the need to recalculate flag to false
	needToRecalcRT = false;
	remainingTime = 0;
	vector<pcb_thread>::iterator it;
	for(it = readyProcessThreads.begin();
		it != readyProcessThreads.end();
		it++)
	{
		remainingTime += it->processTime;
	}

	return remainingTime;
}

void ProcessControlBlock::addInstruction(Process newInstruction)
{
	// Set recalc flag to true since we now have a new instruction
	// Then outsource the work to the appropriate thread creation
	// method
	needToRecalcRT = true;
	switch(newInstruction.component)
	{
		case 'P':
		{
			newProcessThread(newInstruction.operation, newInstruction.numberOfCycles);
			break;
		}
		case 'I':
		{
			newInputThread(newInstruction.operation, newInstruction.numberOfCycles);
			break;
		}
		case 'O':
		{
			newOutputThread(newInstruction.operation, newInstruction.numberOfCycles);
			break;
		}
		default:
		{
			myLog.logError( "Unknown component: " + newInstruction.component );
		}
	}
}

void ProcessControlBlock::newProcessThread( 
	string operation, 
	unsigned int numberOfCycles )
{
	unsigned int processTime;
	string startLogMessage, endLogMessage;
	pcb_thread currentThread;

	// Check for correct operation type
	if( operation.compare( "run" ) != 0 )
	{
		myLog.logError( "Unknown operation for process: " + operation );
	}
	// Process #: start processing action
	currentThread.startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start processing action";
	// Process #: end processing action
	currentThread.endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end processing action";
	// Give the thread the means to calculate its wait times
	currentThread.timePerCycle = processCycleTime;
	currentThread.numCyclesRemaining = numberOfCycles;
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}

void ProcessControlBlock::newInputThread( 
	string operation, 
	unsigned int numberOfCycles )
{
	unsigned int processTime;
	string startLogMessage, endLogMessage;
	pcb_thread currentThread;
	// Process #: start <operation> input
	currentThread.startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start " + operation + " input";
	// Process #: end <operation> input
	currentThread.endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end " + operation + " input";
	// Give the thread the means to calculate its wait times
	if( operation.compare( "hard drive" ) == 0 )
	{
		currentThread.timePerCycle = hardDriveCycleTime;
	}
	else if( operation.compare( "keyboard" ) == 0 )
	{
		currentThread.timePerCycle = keyboardCycleTime;
	}
	else
	{
		myLog.logError( "Unknown operation for input: " + operation );
	}
	currentThread.numCyclesRemaining = numberOfCycles;
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}

void ProcessControlBlock::newOutputThread( 
	string operation, 
	unsigned int numberOfCycles )
{
	unsigned int processTime;
	string startLogMessage, endLogMessage;
	pcb_thread currentThread;
	// Process #: start <operation> output
	currentThread.startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start " + operation + " output";
	// Process #: end <operation> output
	currentThread.endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end " + 
		operation + 
		" output";
	// Give the thread the means to calculate its wait times
	if( operation.compare( "hard drive" ) == 0 )
	{
		currentThread.timePerCycle = hardDriveCycleTime;
	}
	else if( operation.compare( "monitor" ) == 0 )
	{
		currentThread.timePerCycle = monitorDisplayTime;
	}
	else if( operation.compare( "printer" ) == 0 )
	{
		currentThread.timePerCycle = printerCycleTime;
	}
	else
	{
		myLog.logError( "Unknown operation for output: " + operation );
	}
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}