#include "Logger.h"

void Logger::setTypeAndName(string pLogType, string pFileName)
{
	logType = pLogType;
	fileName = pFileName;
}

void Logger::logProcess(string processDescription)
{
	if(logType.empty() || fileName.empty())
	{
		logError("Log Type or Log File Name have not been defined");
	}

	char * timeInText;
	timeInText = new char[15];
	timer.stop();
	timer.getElapsedTime(timeInText);
	timer.start();
	totalTime += atof(timeInText);

	if(logType.compare("File") == 0)
	{
		ofstream logFile;
		logFile.open(fileName.c_str());
		if(logFile.is_open() == false)
		{
			string errorMessage = "Could not open file " + fileName;
			logError(errorMessage);
		}
		logFile << timeInText << " - " << processDescription << endl;
		logFile.close();
	}
	else if(logType.compare("Monitor") == 0)
	{
		printf("%.6f - %s\n", totalTime, processDescription.c_str());
	}
	else if(logType.compare("Both") == 0)
	{
		ofstream logFile;
		logFile.open(fileName.c_str());
		if(logFile.is_open() == false)
		{
			string errorMessage = "Could not open file " + fileName;
			logError(errorMessage);
		}
		logFile << timeInText << " - " << processDescription << endl;
		printf("%.6f - %s\n", totalTime, processDescription.c_str());
	}

}

void Logger::logError(string errorMessage)
{
	cout << ("Error: " + errorMessage) << endl;
	exit(0);
}

//void Logger::outputLog()
//{

//}