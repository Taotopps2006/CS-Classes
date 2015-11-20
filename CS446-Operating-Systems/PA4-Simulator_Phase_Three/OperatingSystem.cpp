#include "OperatingSystem.h"

/**
 * Sort method to be used with std::sort from algorithm
 * Sorts based on process number, as that is how FIFO
 * determines when a process came into the queue
 *
 * Pre: Processes have been assigned process numbers
 * Post: Neither parameter will be changed
 * 
 * @param  left  ProcessControlBlock currently on left side of
 *               container
 * @param  right ProcessControlBlock currently on right side of
 *               container
 * @return       left.processNumber < right.processNumber
 */
bool sortFIFO( ProcessControlBlock left, ProcessControlBlock right )
{
    return ( left.processNumber < right.processNumber );
}

/**
 * Sort method to be used with std::sort from algorithm
 * Sorts based on process' remaining time
 *
 * Pre: Processes have been assigned process numbers
 * Post: Only the remaining time variable of each
 *  ProcessControlBlock will be potentially updated
 * 
 * @param  left  ProcessControlBlock currently on left side of
 *               container
 * @param  right ProcessControlBlock currently on right side of
 *               container
 * @return       left.remainingTime < right.remainingTime
 */
bool sortSJF( ProcessControlBlock left, ProcessControlBlock right )
{
    return ( left.getRemainingTime( ) < right.getRemainingTime( ) );
}

OperatingSystem::OperatingSystem( )
{
    totalNumberOfProcesses = 0;
    indexOfCurProcess = 0;
    numberOfProcesses = 0;
    applicationStarted = false;
}

void OperatingSystem::readConfigurationFile( char * fileName )
{
    ifstream configFile;
    configFile.open( fileName, ios::in );
    if( configFile.is_open( ) )
    {
        string tempString; // Used for holding each line

        // This for loop is prepared for versions 1.0, 2.0, and 3.0
        // The settings object will be able to figure out what each line
        // number means when it reads in the current version.
         // This for loop will end in 1 of 2 ways:
         // Versions 1.0 & 2.0: Hit end of file
        // Version 3.0: currentline gets to 13
        for( unsigned int currentLine = 0; 
            !configFile.eof( ) && currentLine < 13; 
            currentLine++ )
        {
            getline( configFile, tempString );
            // If next line fails, the program will exit
            settings.insertLineOfConfigFile( tempString );
        }
    }
    else
    {
        configFile.close( );
        myLog.logError( "Could not open configuration file" );
    }
    configFile.close( );
    settings.processConfigFile( );
    myLog.setTypeAndName( settings.logType, settings.logFilePath );
}

void OperatingSystem::readMetaDataFile( )
{
    ifstream metaFile;
    metaFile.open( settings.filePath, ios::in );
    if( metaFile.is_open( ) )
    {
        string tempString;
        getline( metaFile, tempString );
        if( tempString.compare( "Start Program Meta-Data Code:" ) != 0 )
        {
            myLog.logError( 
                "Metadata file must begin with 'Start Program Meta-Data Code:'" );
        }

        while( getline( metaFile, tempString ) )
        {
            RE2 regExpression( 
                "([A-Z])\\(([a-z]+|[a-z]+ [a-z]+)\\)([0-9]+);" );
            RE2 endOSRegEx( "(S)\\((end)\\)(0)." );
            re2::StringPiece reString( tempString );
            char component;
            string operation;
            unsigned int numberOfCycles;
            bool preemptive = ( settings.version.compare( "3.0" ) == 0 );

            while( 
                RE2::FindAndConsume( 
                    &reString, 
                    regExpression, 
                    &component, 
                    &operation, 
                    &numberOfCycles ) )
            {
                Process currentProcess;
                currentProcess.component = component;
                currentProcess.operation = operation;
                currentProcess.numberOfCycles = numberOfCycles;
                switch( currentProcess.component )
                {
                	case 'S':
                	{
                		operatingSystemInstructions.push_back( currentProcess );
                		break;
                	}
                	case 'A':
                	{
                		operatingSystemInstructions.push_back( currentProcess );
                		if( currentProcess.operation.compare( "start" ) == 0 )
                		{
                			totalNumberOfProcesses++;
        					if(totalNumberOfProcesses != 1)
        					{
        						indexOfCurProcess++;
        					}
        					ProcessControlBlock pcb( totalNumberOfProcesses, settings );
        					readyProcesses.push_back(pcb);
                		}
                		break;
                	}
                	default:
                	{
                		if( readyProcesses.size( ) == 0 )
                		{
                			myLog.logError( "Operation asked for before starting a process" );
                		}

                        if( preemptive )
                        {
                            readyProcesses[ indexOfCurProcess ].addInstructionPreemptive( currentProcess );
                        }
                        else
                        {
                            readyProcesses[ indexOfCurProcess ].addInstructionNonPreemptive( currentProcess );
                        }
                		break;
                	}
                }
            }

            if( 
                RE2::FindAndConsume( 
                    &reString, 
                    endOSRegEx, 
                    &component, 
                    &operation, 
                    &numberOfCycles ) )
            {
                Process currentProcess;
                currentProcess.component = component;
                currentProcess.operation = operation;
                currentProcess.numberOfCycles = numberOfCycles;
                operatingSystemInstructions.push_back( currentProcess );
                break; // End the loop, we found the end of theOS meta file
            }
        }
        getline( metaFile, tempString );
        re2::StringPiece reString( tempString );
        // Use regex here because getline would get the \n char on my system, which made
        // compare inconsistent. RE2 will find if this exists and that is all we're
        // looking for
        if( RE2::FindAndConsume( 
            &reString, 
            "End Program Meta-Data Code." ) == false
            )
        {
            metaFile.close( );
            myLog.logError( 
                "Metadata file must end with 'End Program Meta-Data Code.'" );
        }
    }
    else
    {
        myLog.logError( settings.filePath + " not found" );
    }
    metaFile.close( );
}

void OperatingSystem::runSimulator( )
{
	if( settings.version.compare( "1.0" ) == 0 )
	{
		runPhaseOneSimulator( );
	}
	else if( settings.version.compare( "2.0" ) == 0 )
	{
		runPhaseTwoSimulator( );
	}
    else /*if( settings.version.compare("3.0") == 0 ); not needed, but implicit*/
    {
        runPhaseThreeSimulator( );
    }
}

void OperatingSystem::runPhaseOneSimulator( )
{
    runFIFO();
}

void OperatingSystem::runFIFO( )
{
    // Set currentIndex back to starting point
    indexOfCurProcess = 0;
    // Set number of remaining readyProcesses
    numberOfProcesses = totalNumberOfProcesses;

    //Iterate through all operating system instructions
    for( vector<Process>::iterator 
        currentProcess = operatingSystemInstructions.begin( );
        currentProcess != operatingSystemInstructions.end( );
        currentProcess++
        )
    {
        // Process current instruction in a different method
        processOperation(
            currentProcess->component,
            currentProcess->operation,
            currentProcess->numberOfCycles );
        // If an application was started by current instruction
        if(applicationStarted == true)
        {
            // Check that current index is valid
            if(indexOfCurProcess < readyProcesses.size( ) )
            {
                // Run the application, then decrement number of
                // remaining readyProcesses
                readyProcesses[indexOfCurProcess].runApplicationNonPreemptive();
                numberOfProcesses--;
                indexOfCurProcess++;
            }
            else
            {
                // If it is out of bounds, end the program and say why
                myLog.logError("Attempted application index out of bounds");
            }
        }
    }
    // If the simulator is still running by the time we have run out of instructions
    // then end the program and say why
    if( simulatorRunning == true )
    {
        myLog.logError( 
            "Metadata file has no more commands, but simulator is still running" );
    }
}

void OperatingSystem::processOperation( char component, string operation, unsigned int numberOfCycles )
{
    if( simulatorRunning == false )
    {
        if( component == 'S' && operation.compare( "start" ) == 0 )
        {
            if( numberOfCycles != 0 )
            {
                myLog.logError( 
                    "Operating System operation must have cycle time of 0" );
            }
            simulatorRunning = true;
            return;
        }
        else
        {
            myLog.logError( "Operating system end command issued before start" );
        }
    }

    switch( component )
    {
        case 'S':
        {
            evalOperatingSystem( operation, numberOfCycles );
            break;
        }
        case 'A':
        {
            evalApplication( operation, numberOfCycles );
            break;
        }
        default:
        {
            myLog.logError( "Unknown component: " + component );
        }
    }
}

void OperatingSystem::evalOperatingSystem( string operation, unsigned int numberOfCycles )
{
    if( numberOfCycles != 0 )
    {
        myLog.logError( "Operating System operation must have cycle time of 0" );
    }

    if( operation.compare( "start" ) == 0 )
    {
        // Already established in processOperation that if we got this far
        // Then simulator has indeed already been started
        // Assume that the simulator cannot be stopped then restarted
        myLog.logError( "Received S( start ) but simulator has already been started" );
    }
    else if( operation.compare( "end" ) == 0 )
    {
        simulatorRunning = false;
        myLog.logProcess( "Simulator program ending" );
    }
    else
    {
        myLog.logError( "Unknown operation for Operating System: " );
    }
}

void OperatingSystem::evalApplication( string operation, unsigned int numberOfCycles )
{
    if( numberOfCycles != 0 )
    {
        myLog.logError( "Program Application operation must have cycle time of 0" );
    }

    if( operation.compare( "start" ) == 0 )
    {
        if( applicationStarted == true )
        {
            myLog.logError( "Application already started, cannot start again" );
        }
        applicationStarted = true;
        if( settings.version.compare( "1.0" ) == 0 )
        {
            myLog.logProcess( "OS: preparing process 1" );
            myLog.logProcess( "OS: starting process 1" );
        }
        else if( settings.version.compare( "2.0" ) == 0 )
        {
            // If this is the first process being started, report that 
            // we are preparing all readyProcesses
            if( numberOfProcesses == totalNumberOfProcesses )
            {
                myLog.logProcess("OS: preparing all readyProcesses");
            }

            // If we are not dealing with SRTF-N (ie, FIFO or SJF),
            // then this is where we will report that we are selecting next
            // process and starting said process
            if( settings.cpuScheduling.compare( "SRTF-N" ) != 0 )
            {
                myLog.logProcess( "OS: selecting next process" );
                myLog.logProcess( "OS: starting process " + to_string( readyProcesses[indexOfCurProcess].processNumber ) );
            }
        }
    }
    else if( operation.compare( "end" ) == 0 )
    {
        if( applicationStarted == false)
        {
            myLog.logError("Application end called before application start");
        }

        applicationStarted = false;
    }
    else
    {
        myLog.logError( "Unknown operation for Program Application: " );
    }
}

void OperatingSystem::runPhaseTwoSimulator( )
{
	if( settings.cpuScheduling.compare( "FIFO" ) == 0 )
	{
		runFIFO( );
	}
	else if( settings.cpuScheduling.compare( "SJF" ) == 0)
	{
		runSJF( );
	}
	else if( settings.cpuScheduling.compare( "SRTF-N" ) == 0 )
	{
		runSRTFN( );
	}
}

void OperatingSystem::runSJF()
{
    // Sort readyProcesses, then use the FIFO method to run through them
    sort( readyProcesses.begin( ), readyProcesses.end( ), sortSJF );
    runFIFO();
}

void OperatingSystem::runSRTFN()
{
    // Use the same core algorithm as fifo, but instead of
    // simply incrementing the currentIndex, we will have a method
    // find the shortest remaining time index
    
    // Set currentIndex back to starting point
    indexOfCurProcess = 0;
    // Set number of remaining readyProcesses
    numberOfProcesses = totalNumberOfProcesses;

    //Iterate through all operating system instructions
    for( vector<Process>::iterator 
        currentProcess = operatingSystemInstructions.begin( );
        currentProcess != operatingSystemInstructions.end( );
        currentProcess++
        )
    {
        // Process current instruction in a different method
        processOperation(
            currentProcess->component,
            currentProcess->operation,
            currentProcess->numberOfCycles );
        // If an application was started by current instruction
        if(applicationStarted == true)
        {
            // Try to find the index of the next shortest time remaining
            // process
            indexOfCurProcess = findSRTFN( );
            if(indexOfCurProcess < readyProcesses.size( ) )
            {
                readyProcesses[indexOfCurProcess].runApplicationNonPreemptive( );
            }
            else
            {
                // If it is out of bounds, end the program and say why
                myLog.logError("Attempted application index out of bounds");
            }
        }
    }
    // If the simulator is still running by the time we have run out of instructions
    // then end the program and say why
    if( simulatorRunning == true )
    {
        myLog.logError( 
            "Metadata file has no more commands, but simulator is still running" );
    }
}

unsigned int OperatingSystem::findSRTFN( )
{
    // Iterate over all readyProcesses to find out the next shortest
    // Assume that the getRemainingTime() method on each process will not
    // take a significant amount of time overall. (getRemainingTime will be
    // programmed to 'cache' the last calculated remaining time and only
    // recalculate if a new item is added or removed
    unsigned int SRTFNIndex = 0;
    unsigned int SRTFNAmount = 0;
    myLog.logProcess("OS: selecting next process");
    for(unsigned int indexOfCurProcess = 0; indexOfCurProcess < readyProcesses.size(); indexOfCurProcess++)
    {
        unsigned int curTimeAmount = readyProcesses[indexOfCurProcess].getRemainingTime();
        if(curTimeAmount != 0 && (curTimeAmount < SRTFNAmount || SRTFNAmount == 0))
        {
            SRTFNIndex = indexOfCurProcess;
            SRTFNAmount = curTimeAmount;
        }
    }

    // Check that we have found a shortest remaining time
    if(SRTFNAmount == 0)
    {
        myLog.logError("No remaining readyProcesses to run");
    }

    myLog.logProcess("OS: starting process " + to_string(readyProcesses[SRTFNIndex].processNumber));
    return SRTFNIndex;
}

void OperatingSystem::runPhaseThreeSimulator( )
{
    prepareProcesses( );
    if( settings.cpuScheduling.compare( "RR" ) == 0 )
    {
    	myLog.logProcess( "OS: Using Round Robin CPU Scheduling" );
    	runRR( );
    }
    else if( settings.cpuScheduling.compare( "FIFO-P" ) == 0 )
    {
    	myLog.logProcess( "OS: Using FIFO-P CPU Scheduling" );
        runFIFOP( );
    }
    else
    {
    	myLog.logProcess( "OS: Using SRTF-P CPU Scheduling" );
    	runSRTFP( );
    }

    myLog.logProcess( "Simulator program ending" );
}

void OperatingSystem::prepareProcesses( )
{
    myLog.logProcess( "OS: preparing all processes" );
    vector<Process>::iterator it;
    bool operatingSystemStarted = false;
    bool applicationStarted = false;
    for( it = operatingSystemInstructions.begin( );
        ( it != operatingSystemInstructions.end( ) );
        it++ )
    {
        if( it->component == 'S' )
        {
            if( it->operation.compare( "start" ) == 0 )
            {
                if( operatingSystemStarted == true )
                {
                    myLog.logError( "Operating System start command issued twice" );
                }
                operatingSystemStarted = true;
            }
            else if( it->operation.compare( "end" ) == 0 )
            {
                if( operatingSystemStarted == false )
                {
                    myLog.logError( "Operating system end command issued before start" );
                }
                operatingSystemStarted = false;
            }
            else
            {
                myLog.logError( "Operating System issued unknown command" );
            }

            if( it->numberOfCycles != 0)
            {
                myLog.logError( "Operating System operation must have cycle time of 0" );
            }
        }
        else if( it->component == 'A' )
        {
            if( it->operation.compare( "start" ) == 0)
            {
                if( applicationStarted == true )
                {
                    myLog.logError( "Multiple application start commands issued before an end" );
                }
                applicationStarted = true;
            }
            else if( it->operation.compare( "end" ) == 0)
            {
                if( applicationStarted == false )
                {
                    myLog.logError( "Application end command issued before start" );
                }
                applicationStarted = false;
            }
            else
            {
                myLog.logError( "Application issued unknown command" );
            }

            if( it->numberOfCycles != 0 )
            {
                myLog.logError( "Application operation must have cycle time of 0" );
            }
        }
        else
        {
            myLog.logError( "Unknown system command issued" );
        }

    }
}

void OperatingSystem::runRR( )
{
    // Continue through processes until there is nothing left in our
    // ready queue or our blocked queue
	while( readyProcesses.size() != 0 || blockedProcesses.size() != 0 )
    {
        myLog.logProcess( "OS: selecting next process" );
        // Notice: No sorting is occuring in this method
        // If the first process is a finished one, we remove it from the queue
        // and continue removing processes from the queue until we find one that
        // isn't finished
        bool processIsFinished = ( readyProcesses[0].getRemainingTime( ) == 0 );
        while( processIsFinished == true && readyProcesses.size() != 0 )
        {
            myLog.logProcess( 
                "OS: Process " + 
                to_string( readyProcesses[0].processNumber ) +
                " has been completed" );
            readyProcesses.erase( readyProcesses.begin( ) );
            processIsFinished = ( readyProcesses[0].getRemainingTime( ) == 0 );
        }
        if( readyProcesses.size() != 0 )
        {
            // Find out if the process that just ran was an IO process that
            // needs to be put into blocked while its thread finishes
            bool putIntoBlocked = readyProcesses[0].runApplicationPreemptive( );
            if( putIntoBlocked == true )
            {
                // If it needs to be put into blocked, then remove it from
                // here, put it into blocked, and then resolve its interrupt
                // directly from the interrupt system (instead of our own
                // interrupt resolve system) so that we don't accidentally
                // remove it from the blocked queue immediately after
                blockedProcesses.insert( pair< int, ProcessControlBlock >( 
                    readyProcesses[0].processNumber, readyProcesses[0] ) );
                readyProcesses.erase( readyProcesses.begin( ) );
                interrupts.resolveInterrupt( );
            }
            else
            {
                // If it wasn't put into blocked, then we send it
                // to the back of the ready queue
            	readyProcesses.push_back( readyProcesses[0] );
            	readyProcesses.erase( readyProcesses.begin( ) );
            }
            // Regardless of whether the last process was blocked,
            // we need to check if any interrupts after occurred (if it
            // wasn't blocked, then there will be a quantum or complete
            // interrupt; if it was IO, there is possibly still an interrupt
            // from another thread reporting back)
            resolveInterrupts( );
        }
        else
        {
            // If there are no processes in our ready queue, then we need
            // to continue checking our blocked queue to unblock some
            // processes so that we can check ready again
            bool idle = false;
            bool notifiedIdle = false;
            while( readyProcesses.size() == 0 && blockedProcesses.size() > 0)
            {
                // Find out if we resolved an interrupt, or if we were idle
                idle = resolveInterrupts( );
                // We don't want to be obnoxcious with our reporting of
                // idle, so we will only report that we are idling
                // the first time
                if( idle == true && notifiedIdle == false )
                {
                    myLog.logProcess( 
                        "OS: No processes or interrupts available, idling" );
                    notifiedIdle = true;
                }
            }
            // Somewhat redundant, as the log will show that we resolved an
            // interrupt, but just a heads up that we are definitely done being
            // idle, if we had sent out a notification that we were idle
            if( notifiedIdle == true )
            {
                myLog.logProcess( "OS: No longer idling " );
            }
        }
    }
}

void OperatingSystem::runFIFOP( )
{
    // Continue through processes until there is nothing left in our
    // ready queue or our blocked queue
    while( readyProcesses.size() != 0 || blockedProcesses.size() != 0 )
    {
        myLog.logProcess( "OS: selecting next process" );
        // SORT BY PROCESS ID
        // We could use a "find" method, but then we would have to use
        // it multiple times if it turned out that a process finished
        // Sorting, instead, will guarantee that the first process will
        // always be the one we want, even if we remove other first processes
        sort( readyProcesses.begin( ), readyProcesses.end( ), sortFIFO);

        // If the first process is a finished one, we remove it from the queue
        // and continue removing processes from the queue until we find one that
        // isn't finished
        bool processIsFinished = ( readyProcesses[0].getRemainingTime( ) == 0 );
        while( processIsFinished == true && readyProcesses.size() != 0 )
        {
            myLog.logProcess( 
                "OS: Process " + 
                to_string( readyProcesses[0].processNumber ) +
                " has been completed" );
            readyProcesses.erase( readyProcesses.begin( ) );
            processIsFinished = ( readyProcesses[0].getRemainingTime( ) == 0 );
        }
        if( readyProcesses.size() != 0 )
        {
            // Find out if the process that just ran was an IO process that
            // needs to be put into blocked while its thread finishes
            bool putIntoBlocked = readyProcesses[0].runApplicationPreemptive( );
            if( putIntoBlocked == true )
            {
                // If it needs to be put into blocked, then remove it from
                // here, put it into blocked, and then resolve its interrupt
                // directly from the interrupt system (instead of our own
                // interrupt resolve system) so that we don't accidentally
                // remove it from the blocked queue immediately after
                blockedProcesses.insert( pair< int, ProcessControlBlock >( 
                    readyProcesses[0].processNumber, readyProcesses[0] ) );
                readyProcesses.erase( readyProcesses.begin( ) );
                interrupts.resolveInterrupt( );
            }
            // Regardless of whether the last process was blocked,
            // we need to check if any interrupts after occurred (if it
            // wasn't blocked, then there will be a quantum or complete
            // interrupt; if it was IO, there is possibly still an interrupt
            // from another thread reporting back)
            resolveInterrupts( );
        }
        else
        {
            // If there are no processes in our ready queue, then we need
            // to continue checking our blocked queue to unblock some
            // processes so that we can check ready again
            bool idle = false;
            bool notifiedIdle = false;
            while( readyProcesses.size() == 0 && blockedProcesses.size() > 0)
            {
                // Find out if we resolved an interrupt, or if we were idle
                idle = resolveInterrupts( );
                // We don't want to be obnoxcious with our reporting of
                // idle, so we will only report that we are idling
                // the first time
                if( idle == true && notifiedIdle == false )
                {
                    myLog.logProcess( 
                        "OS: No processes or interrupts available, idling" );
                    notifiedIdle = true;
                }
            }
            // Somewhat redundant, as the log will show that we resolved an
            // interrupt, but just a heads up that we are definitely done being
            // idle, if we had sent out a notification that we were idle
            if( notifiedIdle == true )
            {
                myLog.logProcess( "OS: No longer idling " );
            }
        }
    }
}

void OperatingSystem::runSRTFP( )
{
	// Continue through processes until there is nothing left in our
	// ready queue or our blocked queue
    while( readyProcesses.size() != 0 || blockedProcesses.size() != 0 )
    {
        myLog.logProcess( "OS: selecting next process" );
        // SORT BY REMAINING TIME
        // We could use a "find" method, but then we would have to use
        // it multiple times if it turned out that a process finished
        // Sorting, instead, will guarantee that the first process will
        // always be the one we want, even if we remove other first processes
        sort( readyProcesses.begin( ), readyProcesses.end( ), sortSJF);

        // If the first process is a finished one, we remove it from the queue
        // and continue removing processes from the queue until we find one that
        // isn't finished
        bool processIsFinished = ( readyProcesses[0].getRemainingTime( ) == 0 );
        while( processIsFinished == true && readyProcesses.size() != 0 )
        {
            myLog.logProcess( 
                "OS: Process " + 
                to_string( readyProcesses[0].processNumber ) +
                " has been completed" );
            readyProcesses.erase( readyProcesses.begin( ) );
            processIsFinished = ( readyProcesses[0].getRemainingTime( ) == 0 );
        }

        if( readyProcesses.size() != 0 )
        {
        	// Find out if the process that just ran was an IO process that
        	// needs to be put into blocked while its thread finishes
            bool putIntoBlocked = readyProcesses[0].runApplicationPreemptive( );
            if( putIntoBlocked == true )
            {
            	// If it needs to be put into blocked, then remove it from
            	// here, put it into blocked, and then resolve its interrupt
            	// directly from the interrupt system (instead of our own
            	// interrupt resolve system) so that we don't accidentally
            	// remove it from the blocked queue immediately after
                blockedProcesses.insert( pair< int, ProcessControlBlock >( 
                    readyProcesses[0].processNumber, readyProcesses[0] ) );
                readyProcesses.erase( readyProcesses.begin( ) );
                interrupts.resolveInterrupt( );
            }
            // Regardless of whether the last process was blocked,
            // we need to check if any interrupts after occurred (if it
            // wasn't blocked, then there will be a quantum or complete
            // interrupt; if it was IO, there is possibly still an interrupt
            // from another thread reporting back)
            resolveInterrupts( );
        }
        else
        {
        	// If there are no processes in our ready queue, then we need
        	// to continue checking our blocked queue to unblock some
        	// processes so that we can check ready again
            bool idle = false;
            bool notifiedIdle = false;
            while( readyProcesses.size() == 0 && blockedProcesses.size() > 0)
            {
            	// Find out if we resolved an interrupt, or if we were idle
                idle = resolveInterrupts( );
                // We don't want to be obnoxcious with our reporting of
                // idle, so we will only report that we are idling
                // the first time
                if( idle == true && notifiedIdle == false )
                {
                    myLog.logProcess( 
                        "OS: No processes or interrupts available, idling" );
                    notifiedIdle = true;
                }
            }
            // Somewhat redundant, as the log will show that we resolved an
            // interrupt, but just a heads up that we are definitely done being
            // idle, if we had sent out a notification that we were idle
            if( notifiedIdle == true )
            {
                myLog.logProcess( "OS: No longer idling " );
            }
        }
    }
}

bool OperatingSystem::resolveInterrupts( )
{
	// To report back whether or not idle, keep a flag
	// that is initially set to true. It can only be
	// changed once at least one interrupt has been
	// resolved
    bool idle = true;

    // Continue resolving interrupts until there are
    // none left
    while( interrupts.numberOfInterrupts > 0 )
    {
        idle = false; // if we got here, there's at least one interrupt

        // Get the process ID of the interrupt most recently resolved
        int currentInterrupt = interrupts.resolveInterrupt( );

        // Attempt to find that ID in the blocked queue
        unordered_map< int, ProcessControlBlock >::iterator currentPCB =
            blockedProcesses.find(currentInterrupt);
        // If it isn't there, currentPCB will point to the end
        if( currentPCB != blockedProcesses.end( ) )
        {
        	// If it is there, we remove the first instruction
        	// (because we didn't remove the interrupt instruction
        	// inside the PCB itself)
            currentPCB->second.removeFirstInstruction();
            // If the remaining time is 0, remove it now
            // to save us a bit of work in the future
            if(currentPCB->second.getRemainingTime( ) != 0 )
            {
                readyProcesses.push_back( currentPCB->second );
            }
            else
            {
                myLog.logProcess( 
                "OS: Process " + 
                to_string( currentPCB->second.processNumber ) +
                " has been completed" );
            }
            blockedProcesses.erase( currentPCB );
        }
    }

    return idle;
}

void OperatingSystem::outputSettingsToConsole( )
{
    settings.outputSettingsToConsole( );
}