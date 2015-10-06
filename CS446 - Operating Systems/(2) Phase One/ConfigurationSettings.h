#ifndef CONFIGURATION_SETTINGS_H
#define CONFIGURATION_SETTINGS_H
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
class ConfigurationSettings
{
public:
	ConfigurationSettings();
	void set(int settingLine, string settingValue);
	void outputSettingsToConsole();

	int numberOfLines; // Expected number of lines in the config file
private:
	void setPhaseOne(int settingLine, string settingValue);
	// future method: void setPhaseTwo(int settingLine, string settingValue);
	// future method: void setPhaseThree(int settingLine, string settingValue);

	string version;
	string filePath;
	int processorCycleTime;
	int monitorDisplayTime;
	int hardDriveCycleTime;
	int printerCycleTime;
	int keyboardCycleTime;
	string logType;
	string logFilePath;
};
#endif // CONFIGURATION_SETTINGS_H