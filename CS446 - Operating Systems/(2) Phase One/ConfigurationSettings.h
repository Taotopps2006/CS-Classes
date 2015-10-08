#ifndef CONFIGURATION_SETTINGS_H
#define CONFIGURATION_SETTINGS_H
#include <iostream>
#include <string>
#include <cstdlib>
#include "Logger.h"
using namespace std;

extern Logger myLog;
class ConfigurationSettings
{
public:
	ConfigurationSettings();
	void checkValidity(int currentLine, string currentString);
	void set(int settingLine, string settingValue);
	void outputSettingsToConsole();
	string getSettingFileKey(int currentLine);
	
	int processCycleTime;
	int monitorDisplayTime;
	int hardDriveCycleTime;
	int printerCycleTime;
	int keyboardCycleTime;
private:
	void setPhaseOne(int settingLine, string settingValue);
	// future method: void setPhaseTwo(int settingLine, string settingValue);
	// future method: void setPhaseThree(int settingLine, string settingValue);
	string version;
	string filePath;
	string logType;
	string logFilePath;
};
#endif // CONFIGURATION_SETTINGS_H