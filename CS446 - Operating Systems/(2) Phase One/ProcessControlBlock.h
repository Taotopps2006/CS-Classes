#ifndef PROCESS_CONTROL_BLOCK_H
#define PROCESS_CONTROL_BLOCK_H

#include <time.h>
#include <thread>
#include <unistd.h>
#include "Logger.h"
#include "ConfigurationSettings.h"

using namespace std;

extern Logger myLog;

class ProcessControlBlock
{
public:
	ProcessControlBlock(int pProcessNumber, const ConfigurationSettings &pSettings);
	void newProcessThread(string operation, int cycleTime);
	void newInputThread(string operation, int cycleTime);
	void newOutputThread(string operation, int cycleTime);
private:
	int processNumber;
	int processCycleTime;
	int monitorDisplayTime;
	int hardDriveCycleTime;
	int printerCycleTime;
	int keyboardCycleTime;
	// Future: Queue for CPU scheduling
};
#endif // PROCESS_CONTROL_BLOCK_H