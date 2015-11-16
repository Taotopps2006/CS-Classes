#ifndef MODIFIED_STL_QUEUE_H
#define MODIFIED_STL_QUEUE_H

#include <queue>
#include <string>

using namespace std;

struct Process
{
	string startMessage;
	string endMessage;
	int totalTimeToProcess;
	int timeLeftToProcess; // Not used in Phase 1.0
	int positionInQueue;
};

class MyComparison
{
	bool operator( )  ( const Process& lhs, const Process& rhs ) const
	{
		return lhs.positionInQueue < rhs.positionInQueue;
	}
};


class MyQueue : public priority_queue<Process, vector<Process>, MyComparison>
{
public:
	Process pop( );
};

Process MyQueue::pop( )
{
	Process val = top( );
	pop( );
	return val;
}
#endif // MODIFIED_STL_QUEUE_H


void ProcessControlBlock::newProcessThread( string operation, int cycleTime )
{
	Process currentProcess;
	currentProcess.startMessage = string( "Process " ) + to_string( processNumber ) + ": start processing action";
	currentProcess.totalTimeToProcess = processCycleTime * cycleTime;
	currentProcess.endMessage = string( "Process " ) + to_string( processNumber ) + ": end processing action";
	currentProcess.positionInQueue = processQueue.size( ) + 1;
	processQueue.push( currentProcess );
}

void ProcessControlBlock::newInputThread( string operation, int cycleTime )
{
	Process currentProcess;
	currentProcess.startMessage = string( "Process " ) + to_string( processNumber ) + ": start " + operation + " input";
	currentProcess.endMessage = string( "Process " ) + to_string( processNumber ) + ": end " + operation + " input";
	currentProcess.positionInQueue = processQueue.size( ) + 1;

	if( operation.compare( "hard drive" ) == 0 )
	{
		currentProcess.totalTimeToProcess = hardDriveCycleTime * cycleTime;
	}
	else if( operation.compare( "keyboard" ) == 0 )
	{
		currentProcess.totalTimeToProcess = keyboardCycleTime * cycleTime;
	}

	processQueue.push( currentProcess );
}

void ProcessControlBlock::newOutputThread( string operation, int cycleTime )
{
	Process currentProcess;
	currentProcess.startMessage = string( "Process " ) + to_string( processNumber ) + ": start " + operation + " output";
	currentProcess.endMessage = string( "Process " ) + to_string( processNumber ) + ": end " + operation + " output";
	currentProcess.positionInQueue = processQueue.size( ) + 1;

	if( operation.compare( "hard drive" ) == 0 )
	{
		currentProcess.totalTimeToProcess = hardDriveCycleTime * cycleTime;
	}
	else if( operation.compare( "monitor" ) == 0 )
	{
		currentProcess.totalTimeToProcess = monitorDisplayTime * cycleTime;
	}
	else if( operation.compare( "printer" ) == 0 )
	{
		currentProcess.totalTimeToProcess = printerCycleTime * cycleTime;
	}

	processQueue.push( currentProcess );
}

void createThreadsPhaseOne( )
{
	while( processQueue.size( ) > 0 )
	{
		Process currentProcess = processQueue.pop( );
		myLog.logProcess( currentProcess.startMessage );
		thread currentThread( createThreadThatSleeps, currentProcess.totalTimeToProcess );
		currentThread.join( );
		myLog.logProcess( currentProcess.endMessage );
	}
}