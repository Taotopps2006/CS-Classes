#ifndef LOGGER_H
#define LOGGER_H
#include "SimpleTimer.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

class Logger
{
public:
	void setTypeAndName(string pLogType, string pFileName = "");
	void logProcess(string processDescription);
	void logError(string errorMessage);
	SimpleTimer timer;
private:
	string logType;
	string fileName;
	double totalTime;
};

#endif // LOGGER_H