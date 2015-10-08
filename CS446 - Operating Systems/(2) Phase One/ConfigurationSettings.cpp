#include "ConfigurationSettings.h"

ConfigurationSettings::ConfigurationSettings()
{
	version = "1.0"; // Default version unless otherwised specified
}

void ConfigurationSettings::checkValidity(int currentLine, string currentString)
{
	string tempString = getSettingFileKey(currentLine);
	if(currentString.compare(tempString) != 0)
	{
		myLog.logError(currentString + " should be " + tempString);
	}
}

string ConfigurationSettings::getSettingFileKey(int currentLine)
{
	string key;
	if(version.compare("1.0"))
	{
		switch(currentLine)
		{
			case 1:
			{
				key = "Start Simulator Configuration File";
				break;
			}
			case 2:
			{
				key = "Version/Phase";
				break;
			}
			case 3:
			{
				key = "File Path";
				break;
			}
			case 4:
			{
				key = "Processor cycle time (msec)";
				break;
			}
			case 5:
			{
				key = "Monitor display time (msec)";
				break;
			}
			case 6:
			{
				key = "Hard drive cycle time (msec)";
				break;
			}
			case 7:
			{
				key = "Printer cycle time (msec)";
				break;
			}
			case 8:
			{
				key = "Keyboard cycle time (msec)";
				break;
			}
			case 9:
			{
				key = "Log";
				break;
			}
			case 10:
			{
				key = "Log File Path";
				break;
			}
			case 11:
			{
				key = "End Simulator Configuration File";
				break;
			}
		}
        
	}
	else if(version.compare("2.0"))
	{
		myLog.logError("Version 2.0 currently not implemented");
	}
	else if(version.compare("3.0"))
	{
		myLog.logError("Version 3.0 currently not implemented");
	}

	return key;
}

void ConfigurationSettings::set(int settingLine, string settingValue)
{
	// trim leading white spaces
	size_t startpos = settingValue.find_first_not_of(" \t");
	if( string::npos != startpos )
	{
    	settingValue = settingValue.substr( startpos );
	}

	switch(settingLine)
	{
		case 2:
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
				myLog.logError("Version 2.0 currently not implemented");
			}
			else if(version.compare("3.0") == 0)
			{
				myLog.logError("Version 3.0 Currently not implemented");
			}
			else
			{
				myLog.logError("Incorrect version number "+  version);
			}
		}
	}
}

void ConfigurationSettings::setPhaseOne(int settingLine, string settingValue)
{
	switch(settingLine)
	{
		case 3:
		{
			filePath = settingValue;
			break;
		}
		case 4:
		{
			processCycleTime = atoi(settingValue.c_str());
			break;
		}
		case 5:
		{
			monitorDisplayTime = atoi(settingValue.c_str());
			break;
		}
		case 6:
		{
			hardDriveCycleTime = atoi(settingValue.c_str());
			break;
		}
		case 7:
		{
			printerCycleTime = atoi(settingValue.c_str());
			break;
		}
		case 8:
		{
			keyboardCycleTime = atoi(settingValue.c_str());
			break;
		}
		case 9:
		{
			if(settingValue.compare("Log to Both") == 0 || settingValue.compare("Log to Monitor") == 0 || settingValue.compare("Log to File") == 0)
			{
				logType = settingValue;
			}
			else
			{
				myLog.logError("Incorrect myLog type " + settingValue);
			}
			break;
		}
		case 10:
		{
			logFilePath = settingValue;
			break;
		}
		default:
		{
			myLog.logError("Error, too many lines in this configuration file");
		}
	}
}

void ConfigurationSettings::outputSettingsToConsole()
{
	cout << "Version: " << version << endl;
	cout << "filePath: " << filePath << endl;
	cout << "processCycleTime: " << processCycleTime << endl;
	cout << "monitorDisplayTime: " << monitorDisplayTime << endl;
	cout << "hardDriveCycleTime: " << hardDriveCycleTime << endl;
	cout << "printerCycleTime: " << printerCycleTime << endl;
	cout << "keyboardCycleTime: " << keyboardCycleTime << endl;
	cout << "logType: " << logType << endl;
	cout << "logFilePath: " << logFilePath << endl;
}