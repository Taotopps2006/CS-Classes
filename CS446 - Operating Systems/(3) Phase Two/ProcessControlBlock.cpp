#include "ProcessControlBlock.h"

static void createThreadThatSleeps( 
	unsigned int sleepTimeInMilliSec )
{
	this_thread::sleep_for(chrono::milliseconds(sleepTimeInMilliSec));;
}

ProcessControlBlock::ProcessControlBlock( )
{
	needToRecalcRT = true; // Used to determine whether the remaining time needs to be recalculated
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
	if(needToRecalcRT == false)
	{
		return remainingTime;
	}

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
	// Check for correct operation type
	if( operation.compare( "run" ) != 0 )
	{
		myLog.logError( "Unknown operation for process: " + operation );
	}
	// Process #: start processing action
	startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start processing action";
	// Calculate the time thread will need to be processed
	processTime = processCycleTime * numberOfCycles;
	endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end processing action";
	// Initialize the new thread
	pcb_thread currentThread;
	currentThread.startLogMessage = startLogMessage;
	currentThread.endLogMessage = endLogMessage;
	currentThread.processTime = processTime;
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}

void ProcessControlBlock::newInputThread( 
	string operation, 
	unsigned int numberOfCycles )
{
	unsigned int processTime;
	string startLogMessage, endLogMessage;
	// Calculate the time thread will need to be processed
	if( operation.compare( "hard drive" ) == 0 )
	{
		processTime = hardDriveCycleTime * numberOfCycles;
	}
	else if( operation.compare( "keyboard" ) == 0 )
	{
		processTime = keyboardCycleTime * numberOfCycles;
	}
	else
	{
		myLog.logError( "Unknown operation for input: " + operation );
	}
	// Process #: start <operation> input
	startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start " + operation + " input";
	// Process #: end <operation> input
	endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end " + operation + " input";
	// Initialize the new thread
	pcb_thread currentThread;
	currentThread.startLogMessage = startLogMessage;
	currentThread.endLogMessage = endLogMessage;
	currentThread.processTime = processTime;
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}

void ProcessControlBlock::newOutputThread( 
	string operation, 
	unsigned int numberOfCycles )
{
	unsigned int processTime;
	string startLogMessage, endLogMessage;
	// Calculate the time thread will need to be processed
	if( operation.compare( "hard drive" ) == 0 )
	{
		processTime = hardDriveCycleTime * numberOfCycles;
	}
	else if( operation.compare( "monitor" ) == 0 )
	{
		processTime = monitorDisplayTime * numberOfCycles;
	}
	else if( operation.compare( "printer" ) == 0 )
	{
		processTime = printerCycleTime * numberOfCycles;
	}
	else
	{
		myLog.logError( "Unknown operation for output: " + operation );
	}
	// Process #: start <operation> output
	startLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start " + operation + " output";
	// Process #: end <operation> output
	endLogMessage =
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end " + 
		operation + 
		" output";
	// Initialize the new thread
	pcb_thread currentThread;
	currentThread.startLogMessage = startLogMessage;
	currentThread.endLogMessage = endLogMessage;
	currentThread.processTime = processTime;
	// Push thread to back of ready queue
	readyProcessThreads.push_back( currentThread );
}