#include "ProcessControlBlock.h"

static void createThreadThatSleeps( 
	int sleepTimeInMilliSec )
{
	int sleepTimeInMicroSec = sleepTimeInMilliSec * 1000;
	usleep( sleepTimeInMicroSec );
}

ProcessControlBlock::ProcessControlBlock( 
	int pProcessNumber, 
	const ConfigurationSettings & pSettings )
{
	processNumber = pProcessNumber;
	processCycleTime = pSettings.processCycleTime;
	monitorDisplayTime = pSettings.monitorDisplayTime;
	hardDriveCycleTime = pSettings.hardDriveCycleTime;
	printerCycleTime = pSettings.printerCycleTime;
	keyboardCycleTime = pSettings.keyboardCycleTime;
}
void ProcessControlBlock::newProcessThread( 
	string operation, 
	int cycleTime )
{
	myLog.logProcess( 
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start processing action" );
	int sleepTime;
	sleepTime = processCycleTime * cycleTime;
	thread process( createThreadThatSleeps, sleepTime );
	process.join( ); // Pause until thread is done
	myLog.logProcess( 
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end processing action" );
}

void ProcessControlBlock::newInputThread( 
	string operation, 
	int cycleTime )
{
	myLog.logProcess( 
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start " + operation + " input" );
	int sleepTime;

	if( operation.compare( "hard drive" ) == 0 )
	{
		sleepTime = hardDriveCycleTime * cycleTime;
	}
	else if( operation.compare( "keyboard" ) == 0 )
	{
		sleepTime = keyboardCycleTime * cycleTime;
	}

	thread process( createThreadThatSleeps, sleepTime );
	process.join( ); // Pause until thread is done
	myLog.logProcess( 
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end " + operation + " input" );
}

void ProcessControlBlock::newOutputThread( 
	string operation, 
	int cycleTime )
{
	myLog.logProcess( 
		string( "Process " ) + 
		to_string( processNumber ) + 
		": start " + operation + " output" );
	int sleepTime;

	if( operation.compare( "hard drive" ) == 0 )
	{
		sleepTime = hardDriveCycleTime * cycleTime;
	}
	else if( operation.compare( "monitor" ) == 0 )
	{
		sleepTime = monitorDisplayTime * cycleTime;
	}
	else if( operation.compare( "printer" ) == 0 )
	{
		sleepTime = printerCycleTime * cycleTime;
	}

	thread process( createThreadThatSleeps, sleepTime );
	process.join( ); // Pause until thread is done
	myLog.logProcess( 
		string( "Process " ) + 
		to_string( processNumber ) + 
		": end " + 
		operation + 
		" output" );
}