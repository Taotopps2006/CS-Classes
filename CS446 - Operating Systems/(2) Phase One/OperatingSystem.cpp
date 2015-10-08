#include "OperatingSystem.h"

OperatingSystem::OperatingSystem()
{
    totalNumberOfProcesses = 0;
    indexOfCurProcess = -1;
    numberOfProcesses = 0;
}

void OperatingSystem::readConfigurationFile(char* fileName)
{
    ifstream configFile;
    configFile.open(fileName, ios::in);
    if(configFile.is_open())
    {
        string tempString; // Used for holding each line

        // This for loop is prepared for versions 1.0, 2.0, and 3.0
        // The settings object will be able to figure out what each line
        // number means when it reads in the current version.
         // This for loop will end in 1 of 2 ways:
         // Versions 1.0 & 2.0: Hit end of file
        // Version 3.0: currentline gets to 13
        for(int currentLine = 0; !configFile.eof() && currentLine < 13; currentLine++)
        {
            // If next line fails, the program will exit
            getline(configFile, tempString);
            settings.set(currentLine, tempString);
        }
    }
    else
    {
        myLog.logError("Could not open configuration file");
    }
    myLog.setTypeAndName(settings.logType, settings.logFilePath);
}

void OperatingSystem::readMetaDataFile()
{
    ifstream metaFile;
    metaFile.open(settings.filePath, ios::in);
    if(metaFile.is_open())
    {
        string tempString;
        getline(metaFile, tempString);
        if(tempString.compare("Start Program Meta-Data Code:") != 0)
        {
            myLog.logError("Metadata file must begin with 'Start Program Meta-Data Code:'");
        }
        while(getline(metaFile, tempString))
        {
        RE2 regExpression("([A-Z])\\(([a-z]+|[a-z]+ [a-z]+)\\)([0-9]+);");
        re2::StringPiece reString(tempString);
        char component;
        string operation;
        int cycleTime;

        while(RE2::FindAndConsume(&reString, regExpression, &component, &operation, &cycleTime))
        {
            processOperation(component, operation, cycleTime);
        }
    }
    }
    else
    {
        myLog.logError(settings.filePath + " not found");
    }
}

void OperatingSystem::processOperation(char component, string operation, int cycleTime)
{
    if(simulatorRunning == false)
    {
        if(component == 'S' && operation.compare("start") == 0)
        {
            if(cycleTime != 0)
            {
                myLog.logError("Operating System operation must have cycle time of 0");
            }

            myLog.logProcess("Simulator program starting");
            simulatorRunning = true;
            return;
        }
        else
        {
            myLog.logError("Simulator is not running");
        }
    }

    switch(component)
    {
        case 'S':
        {
            evalOperatingSystem(operation, cycleTime);
            break;
        }
        case 'A':
        {
            evalApplication(operation, cycleTime);
            break;
        }
        case 'P':
        {
            evalProcess(operation, cycleTime);
            break;
        }
        case 'I':
        {
            evalInput(operation, cycleTime);
            break;
        }
        case 'O':
        {
            evalOutput(operation, cycleTime);
            break;
        }
        default:
        {
            myLog.logError("Unknown component: " + component);
        }
    }
}

void OperatingSystem::evalOperatingSystem(string operation, int cycleTime)
{
    if(cycleTime != 0)
    {
        myLog.logError("Operating System operation must have cycle time of 0");
    }

    if(operation.compare("start") == 0)
    {
        // Already established in processOperation that if we got this far
        // Then simulator has indeed already been started
        myLog.logError("Received S(start) but simulator has already been started");
    }
    else if(operation.compare("end") == 0)
    {
        simulatorRunning = false;
        myLog.logProcess("Simulator program ending");
    }
    else
    {
        myLog.logError("Unknown operation for Operating System: ");
    }
}

void OperatingSystem::evalApplication(string operation, int cycleTime)
{
    if(cycleTime != 0)
    {
        myLog.logError("Program Application operation must have cycle time of 0");
    }

    if(operation.compare("start") == 0)
    {
        totalNumberOfProcesses++;
        indexOfCurProcess++;
        numberOfProcesses++;
        myLog.logProcess("OS: preparing process " + to_string(totalNumberOfProcesses));
        ProcessControlBlock b(totalNumberOfProcesses, settings);
        processes.push_back(b);
    }
    else if(operation.compare("end") == 0)
    {
        myLog.logProcess("OS: removing process " + to_string(indexOfCurProcess));
        indexOfCurProcess--;
        numberOfProcesses--;
        processes.erase(processes.begin());
    }
    else
    {
        myLog.logError("Unknown operation for Program Application: ");
    }
}

void OperatingSystem::evalProcess(string operation, int cycleTime)
{
    if(numberOfProcesses <= 0)
    {
        myLog.logError("There is no process available to perform this operation");
    }

    if(operation.compare("run") == 0)
    {
        processes[indexOfCurProcess].newProcessThread(operation, cycleTime);
    }
    else
    {
        myLog.logError("Unknown operation for Process: ");
    }
}

void OperatingSystem::evalInput(string operation, int cycleTime)
{
    if(numberOfProcesses <= 0)
    {
        myLog.logError("There is no process available to perform this operation");
    }

    if(operation.compare("hard drive") == 0 ||
                operation.compare("keyboard") == 0 )
    {
        processes[indexOfCurProcess].newInputThread(operation, cycleTime);
    }
    else
    {
        myLog.logError("Unknown operation for Input: " + operation);
    }
}

void OperatingSystem::evalOutput(string operation, int cycleTime)
{
    if(numberOfProcesses <= 0)
    {
        myLog.logError("There is no process available to perform this operation");
    }
    
    if(operation.compare("hard drive") == 0 ||
                operation.compare("monitor") == 0 ||
                operation.compare("printer") == 0)
    {
        processes[indexOfCurProcess].newOutputThread(operation, cycleTime);
    }
    else
    {
        myLog.logError("Unkonwn operation for Output: " + operation);
    }
}

void OperatingSystem::outputSettingsToConsole()
{
    settings.outputSettingsToConsole();
}