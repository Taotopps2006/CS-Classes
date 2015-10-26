#include "OperatingSystem.h"

OperatingSystem::OperatingSystem( )
{
    totalNumberOfProcesses = 0;
    indexOfCurProcess = -1;
    numberOfProcesses = 0;
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
        for( int currentLine = 0; 
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
            int numberOfCycles;

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
                operatingSystemInstructions.push_back( currentProcess );
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
                "Metadata file must end with 'End Program Meta-Data Code:'" );
        }
    }
    else
    {
        myLog.logError( settings.filePath + " not found" );
    }
    metaFile.close( );
}

void OperatingSystem::runPhaseOneSimulator( )
{
    for( vector<Process>::iterator 
        currentProcess = operatingSystemInstructions.begin( );
        currentProcess != operatingSystemInstructions.end( );
        currentProcess++
        )
    {
        processOperation(
            currentProcess->component,
            currentProcess->operation,
            currentProcess->numberOfCycles );
    }
    if( simulatorRunning == true )
    {
        myLog.logError( 
            "Metadata file has no more commands, but simulator is still running" );
    }
}

void OperatingSystem::processOperation( char component, string operation, int numberOfCycles )
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
        case 'P':
        {
            evalProcess( operation, numberOfCycles );
            break;
        }
        case 'I':
        {
            evalInput( operation, numberOfCycles );
            break;
        }
        case 'O':
        {
            evalOutput( operation, numberOfCycles );
            break;
        }
        default:
        {
            myLog.logError( "Unknown component: " + component );
        }
    }
}

void OperatingSystem::evalOperatingSystem( string operation, int numberOfCycles )
{
    if( numberOfCycles != 0 )
    {
        myLog.logError( "Operating System operation must have cycle time of 0" );
    }

    if( operation.compare( "start" ) == 0 )
    {
        // Already established in processOperation that if we got this far
        // Then simulator has indeed already been started
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

void OperatingSystem::evalApplication( string operation, int numberOfCycles )
{
    if( numberOfCycles != 0 )
    {
        myLog.logError( "Program Application operation must have cycle time of 0" );
    }

    if( operation.compare( "start" ) == 0 )
    {
        totalNumberOfProcesses++;
        indexOfCurProcess++;
        numberOfProcesses++;
        myLog.logProcess( "OS: preparing process " + to_string( totalNumberOfProcesses ) );
        ProcessControlBlock pcb( totalNumberOfProcesses, settings );
        processes.push_back( pcb );
        myLog.logProcess( "OS: starting process " + to_string( totalNumberOfProcesses ) );
    }
    else if( operation.compare( "end" ) == 0 )
    {
        myLog.logProcess( "OS: removing process " + to_string( indexOfCurProcess + 1 ) );
        indexOfCurProcess--;
        numberOfProcesses--;
        processes.erase( processes.begin( ) );
    }
    else
    {
        myLog.logError( "Unknown operation for Program Application: " );
    }
}

void OperatingSystem::evalProcess( string operation, int numberOfCycles )
{
    if( numberOfProcesses <= 0 )
    {
        myLog.logError( "There is no process available to perform this operation" );
    }

    if( operation.compare( "run" ) == 0 )
    {
        processes[indexOfCurProcess].newProcessThread( operation, numberOfCycles );
    }
    else
    {
        myLog.logError( "Unknown operation for Process: " );
    }
}

void OperatingSystem::evalInput( string operation, int numberOfCycles )
{
    if( numberOfProcesses <= 0 )
    {
        myLog.logError( "There is no process available to perform this operation" );
    }

    if( operation.compare( "hard drive" ) == 0 ||
                operation.compare( "keyboard" ) == 0 )
    {
        processes[indexOfCurProcess].newInputThread( operation, numberOfCycles );
    }
    else
    {
        myLog.logError( "Unknown operation for Input: " + operation );
    }
}

void OperatingSystem::evalOutput( string operation, int numberOfCycles )
{
    if( numberOfProcesses <= 0 )
    {
        myLog.logError( "There is no process available to perform this operation" );
    }
    
    if( operation.compare( "hard drive" ) == 0 ||
                operation.compare( "monitor" ) == 0 ||
                operation.compare( "printer" ) == 0 )
    {
        processes[indexOfCurProcess].newOutputThread( operation, numberOfCycles );
    }
    else
    {
        myLog.logError( "Unknown operation for Output: " + operation );
    }
}

void OperatingSystem::outputSettingsToConsole( )
{
    settings.outputSettingsToConsole( );
}