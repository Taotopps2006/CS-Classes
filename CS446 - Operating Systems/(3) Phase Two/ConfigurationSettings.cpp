#include "ConfigurationSettings.h"

ConfigurationSettings::ConfigurationSettings( )
{
	version = "1.0"; // Default version unless otherwised specified
	regexLineKeys = {
		"(Start Simulator Configuration File)",
		"Version/Phase: ([0-9]\\.0)",
		"File Path: (test_\\d\\.mdf)",
		//"CPU Scheduling: (FIFO|SJF|SRTF-N)" (Version 2.0 only; this is at begin()+3)
		"Processor cycle time \\(msec\\): ([1-9]+[0-9]*)",
		"Monitor display time \\(msec\\): ([1-9]+[0-9]*)",
		"Hard drive cycle time \\(msec\\): ([1-9]+[0-9]*)",
		"Printer cycle time \\(msec\\): ([1-9]+[0-9]*)",
		"Keyboard cycle time \\(msec\\): ([1-9]+[0-9]*)",
		"Log: Log to (File|Monitor|Both)",
		"Log File Path: (logfile_\\d\\.lgf)",
		"(End Simulator Configuration File)"
	};
}

void ConfigurationSettings::insertLineOfConfigFile( string line )
{
	configFile.push_back( line );
}

void ConfigurationSettings::processConfigFile( )
{
	for( unsigned int currentLine = 0; currentLine < configFile.size( ); currentLine++ )
	{
		set( currentLine, configFile[currentLine] );
	}
}

void ConfigurationSettings::set( unsigned int settingLine, string settingValue )
{
	if( settingLine >= regexLineKeys.size( ) )
	{
		return;
	}
	RE2 regExpression( regexLineKeys[settingLine] );
    string match;
    re2::StringPiece reString( settingValue );
	if( RE2::FullMatch( reString, regExpression, &match ) )
	{
		switch( settingLine )
		{
			case 0:
			{
				break;
			}
			case 1:
			{
				version = match;
				// Future: updateRegexKeys( version );
				break;
			}
			default:
			{
				if( version.compare( "1.0" ) == 0 )
				{
					setPhaseOne( settingLine , match );
				}
				else if( version.compare( "2.0" ) == 0 )
				{
					regexLineKeys.insert(
						regexLineKeys.begin()+3 ,
						"CPU Scheduling: (FIFO|SJF|SRTF-N)" );
				}
				else if( version.compare( "3.0" ) == 0 )
				{
					myLog.logError( "Version 3.0 Currently not implemented" );
				}
				else
				{
					myLog.logError( "Incorrect version number "+  version );
				}
			}
		}
	}
	else
	{
		myLog.logError( 
			settingValue + " is not valid for line " + to_string( settingLine )
			);
	}

	
}

void ConfigurationSettings::setPhaseOne( int settingLine, string settingValue )
{
	switch( settingLine )
	{
		case 2:
		{
			filePath = settingValue;
			break;
		}
		case 3:
		{
			processCycleTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 4:
		{
			monitorDisplayTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 5:
		{
			hardDriveCycleTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 6:
		{
			printerCycleTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 7:
		{
			keyboardCycleTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 8:
		{
			if( 
				settingValue.compare( "Both" ) == 0 || 
				settingValue.compare( "Monitor" ) == 0 || 
				settingValue.compare( "File" ) == 0 )
			{
				logType = settingValue;
			}
			else
			{
				myLog.logError( "Incorrect myLog type " + settingValue );
			}
			break;
		}
		case 9:
		{
			logFilePath = settingValue;
			break;
		}
		case 10:
		{
			break;
		}
		default:
		{
			myLog.logError( "Error, too many lines in this configuration file" + to_string( settingLine ) );
		}
	}
}

void ConfigurationSettings::setPhaseTwo( int settingLine, string settingValue )
{
	switch( settingLine )
	{
		case 2:
		{
			filePath = settingValue;
			break;
		}
		case 3:
		{
			cpuScheduling = settingValue;
		}
		case 4:
		{
			processCycleTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 5:
		{
			monitorDisplayTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 6:
		{
			hardDriveCycleTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 7:
		{
			printerCycleTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 8:
		{
			keyboardCycleTime = atoi( settingValue.c_str( ) );
			break;
		}
		case 9:
		{
			if( 
				settingValue.compare( "Both" ) == 0 || 
				settingValue.compare( "Monitor" ) == 0 || 
				settingValue.compare( "File" ) == 0 )
			{
				logType = settingValue;
			}
			else
			{
				myLog.logError( "Incorrect myLog type " + settingValue );
			}
			break;
		}
		case 10:
		{
			logFilePath = settingValue;
			break;
		}
		case 11:
		{
			break;
		}
		default:
		{
			myLog.logError( "Error, too many lines in this configuration file" + to_string( settingLine ) );
		}
	}
}

void ConfigurationSettings::outputSettingsToConsole( )
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