#include "Logger.h"

void Logger::logProcess(string processDescription)
{
	if(logType.empty() || fileName == 0)
	{
		logError("Log Type or Log File Name have not been defined");
	}

	char * timeInText;
	timer.getElapsedTime(timeInText);
	if(logType.compare("Log to File") == 0)
	{
		ofstream logFile;
		logFile.open(fileName);
		if(logFile.is_open() == false)
		{
			string errorMessage = "Could not open file ";
			errorMessage += fileName;
			logError(errorMessage);
		}
		logFile << timeInText << " - " << processDescription << endl;
		logFile.close();
	}
	else if(logType.compare("Log to Monitor") == 0)
	{
		cout << timeInText << " - " << processDescription << endl;
	}
	else if(logType.compare("Log to Both") == 0)
	{
		ofstream logFile;
		logFile.open(fileName);
		if(logFile.is_open() == false)
		{
			string errorMessage = "Could not open file ";
			errorMessage += fileName;
			logError(errorMessage);
		}
		logFile << timeInText << " - " << processDescription << endl;
		cout << timeInText << " - " << processDescription << endl;
	}

}

void Logger::logError(string errorMessage)
{
	cout << ("Error: " + errorMessage) << endl;
	exit(0);
}