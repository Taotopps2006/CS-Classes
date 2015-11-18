#include "OperatingSystem.h"

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
            // If next line fails, the program will exit
            getline( configFile, tempString );
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
                switch(currentProcess.component)
                {
                	case 'S':
                	{
                		operatingSystemInstructions.push_back( currentProcess );
                		break;
                	}
                	case 'A':
                	{
                		operatingSystemInstructions.push_back( currentProcess );
                		if(currentProcess.operation.compare("start") == 0)
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
                		if(readyProcesses.size() == 0)
                		{
                			myLog.logError("Operation asked for before starting a process");
                		}
                		readyProcesses[indexOfCurProcess].addInstructionNonPreemptive(currentProcess);
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
                break; // End the loop, we found the end of the os meta file
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
	if(settings.version.compare("1.0") == 0)
	{
		runPhaseOneSimulator( );
	}
	else if(settings.version.compare("2.0") == 0)
	{
		runPhaseTwoSimulator( );
	}
}

void OperatingSystem::runPhaseOneSimulator( )
{
    runFIFO();
}

void OperatingSystem::runPhaseTwoSimulator( )
{
	if(settings.cpuScheduling.compare("FIFO") == 0)
	{
		runFIFO();
	}
	else if(settings.cpuScheduling.compare("SJF") == 0)
	{
		runSJF();
	}
	else if(settings.cpuScheduling.compare("SRTF-N") == 0)
	{
		runSRTFN();
	}
}

void OperatingSystem::runFIFO()
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

void OperatingSystem::runSJF()
{
	// Sort readyProcesses, then use the FIFO method to run through them
	sortSJF(0, readyProcesses.size( ) - 1);
	runFIFO();
}

void OperatingSystem::sortSJF(unsigned int originalLeft, unsigned int originalRight)
{
	// Use quicksort to sort
	// Assume that the getRemainingTime() method on each process will not
	// take a significant amount of time overall. (getRemainingTime will be
	// programmed to 'cache' the last calculated remaining time and only
	// recalculate if a new item is added or removed
	unsigned int currentLeft = originalLeft;
	unsigned int currentRight = originalRight;
	ProcessControlBlock temp;
	unsigned int pivot = readyProcesses[(currentLeft + currentRight) / 2].getRemainingTime();

	while(currentLeft <= currentRight)
	{
		while(readyProcesses[currentLeft].getRemainingTime() < pivot)
		{
			currentLeft++;
		}

		while(readyProcesses[currentRight].getRemainingTime() > pivot)
		{
			currentRight--;
		}

		if(currentLeft <= currentRight)
		{
			temp = readyProcesses[currentLeft];
			readyProcesses[currentLeft] = readyProcesses[currentRight];
			readyProcesses[currentRight] = temp;
			currentLeft++;
			currentRight--;
		}
	}

	if(originalLeft < currentRight)
	{
		sortSJF(originalLeft, currentLeft - 1);
	}

	if(currentLeft < originalRight)
	{
		sortSJF(currentLeft, originalRight);
	}
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

            myLog.logProcess( "Simulator program starting" );
            simulatorRunning = true;
            return;
        }
        else
        {
            myLog.logError( "Simulator is not running" );
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
    	if(applicationStarted == true)
    	{
    		myLog.logError("Application already started, cannot start again");
    	}
    	applicationStarted = true;
    	if(settings.version.compare("1.0") == 0)
    	{
    		myLog.logProcess("OS: preparing process 1");
    		myLog.logProcess("OS: starting process 1");
    	}
    	else if(settings.version.compare("2.0") == 0)
    	{
    		// If this is the first process being started, report that 
    		// we are preparing all readyProcesses
    		if(numberOfProcesses == totalNumberOfProcesses)
    		{
    			myLog.logProcess("OS: preparing all readyProcesses");
    		}

    		// If we are not dealing with SRTF-N (ie, FIFO or SJF),
    		// then this is where we will report that we are selecting next
    		// process and starting said process
    		if(settings.cpuScheduling.compare("SRTF-N") != 0)
    		{
    			myLog.logProcess("OS: selecting next process");
    			myLog.logProcess("OS: starting process " + to_string(readyProcesses[indexOfCurProcess].processNumber));
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

void OperatingSystem::outputSettingsToConsole( )
{
    settings.outputSettingsToConsole( );
}