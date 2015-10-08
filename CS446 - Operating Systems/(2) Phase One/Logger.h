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
	void logProcess(string processDescription);
	void logError(string errorMessage);
private:
	SimpleTimer timer;
	string logType;
	char* fileName;
};

#endif // LOGGER_H