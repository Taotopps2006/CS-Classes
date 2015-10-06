#include "ConfigurationSettings.h"

ConfigurationSettings::ConfigurationSettings()
{
	version = "1.0"; // Default version unless otherwised specified
	numberOfLines = 9;
}

void ConfigurationSettings::set(int settingLine, string settingValue)
{
	// trim leading spaces
	size_t startpos = settingValue.find_first_not_of(" \t");
	if( string::npos != startpos )
	{
    	settingValue = settingValue.substr( startpos );
	}

	switch(settingLine)
	{
		case 1:
		{
			version = settingValue;
			break;
		}
		default:
		{
			if(version.compare("1.0") == 0)
			{
				setPhaseOne(settingLine, settingValue);
			}
			else if(version.compare("2.0") == 0)
			{
				cout << "Version 2.0 currently not implemented" << endl;
				exit(0);
			}
			else if(version.compare("3.0") == 0)
			{
				cout << "Version 3.0 Currently not implemented" << endl;
			}
			else
			{
				cout << "Incorrect version number" << " " << settingLine << " "<< version << endl;
				exit(0);
			}
		}
	}
}

void ConfigurationSettings::setPhaseOne(int settingLine, string settingValue)
{
	switch(settingLine)
	{
		case 2:
		{
			filePath = settingValue;
			break;
		}
		case 3:
		{
			processorCycleTime = atoi(settingValue.c_str());
			break;
		}
		case 4:
		{
			monitorDisplayTime = atoi(settingValue.c_str());
			break;
		}
		case 5:
		{
			hardDriveCycleTime = atoi(settingValue.c_str());
			break;
		}
		case 6:
		{
			printerCycleTime = atoi(settingValue.c_str());
			break;
		}
		case 7:
		{
			keyboardCycleTime = atoi(settingValue.c_str());
			break;
		}
		case 8:
		{
			if(settingValue.compare("Log to Both") == 0 || settingValue.compare("Log to Monitor") == 0 || settingValue.compare("Log to File") == 0)
			{
				logType = settingValue;
			}
			else
			{
				cout << "Incorrect log type" << " " << settingValue<< endl;
				exit(0);
			}
			break;
		}
		case 9:
		{
			logFilePath = settingValue;
			break;
		}
		default:
		{
			cout << "Error, too many lines in this configuration file" << settingLine << " " << settingValue << endl;
			exit(0);
		}
	}
}

void ConfigurationSettings::outputSettingsToConsole()
{
	cout << "Version: " << version << endl;
	cout << "filePath: " << filePath << endl;
	cout << "processorCycleTime: " << processorCycleTime << endl;
	cout << "monitorDisplayTime: " << monitorDisplayTime << endl;
	cout << "hardDriveCycleTime: " << hardDriveCycleTime << endl;
	cout << "printerCycleTime: " << printerCycleTime << endl;
	cout << "keyboardCycleTime: " << keyboardCycleTime << endl;
	cout << "logType: " << logType << endl;
	cout << "logFilePath: " << logFilePath << endl;
}

string version;
	string filePath;
	int processorCycleTime;
	int monitorDisplayTime;
	int hardDriveCycleTime;
	int printerCycleTime;
	int keyboardCycleTime;
	string logType;
	string logFilePath;