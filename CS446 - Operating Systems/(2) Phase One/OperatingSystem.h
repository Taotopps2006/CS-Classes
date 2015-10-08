#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <cstdlib>
#include "Logger.h"
#include "ProcessControlBlock.h"
using namespace std;
extern Logger myLog;

class OperatingSystem
{
public:
	void readConfigurationFile(char * fileName);
	void readMetaDataFile(char * fileName);
	void outputSettingsToConsole();
private:
	void processOperation(char component, string operation, int cycleTime);
	void evalOperatingSystem(string operation, int cycleTime);
	void evalApplication(string operation, int cycleTime);
	void evalProcess(string operation, int cycleTime);
	void evalInput(string operation, int cycleTime);
	void evalOutput(string operation, int cycleTime);
	
	bool simulatorRunning;
	int totalNumberOfProcesses;
	int indexOfCurProcess;
	int numberOfProcesses;
	vector<ProcessControlBlock> processes;
	ConfigurationSettings settings;
	// Future: CPU Scheduling Type
	// Future: Quantum time
};
#endif // OPERATING_SYSTEM_H