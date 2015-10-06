//////////////////////////////////////////////////////////////////////
/*
 FileName: Sim01.cpp

 Description: 

*/
// Version/Revision Information ///////////////////////////////////
/*
 1.00 ( 10/09/2015 ) - Tim Kwist
 Original Code
*/
 // Program Description/Support /////////////////////////////////////
/*

*/
 // Precompiler Directives //////////////////////////////////////////
//
 #ifndef SIMO1_CPP
 #define SIM01_CPP
//
// Header Files ///////////////////////////////////////////////////
//
 #include "ConfigurationSettings.h"
 #include <iostream>
 #include <fstream>
 #include <string>

 using namespace std;

// Global Constant Definitions ////////////////////////////////////
//
///// NONE
//

//
// Class Definitions //////////////////////////////////////////////
//
///// NONE
//

//
// Free Function Prototypes ///////////////////////////////////////
//
void ReadConfig(ConfigurationSettings &settings, char* fileName);

// Main Function Implementation ///////////////////////////////////
//
int main( int argc, char * argv[ ] ) 
{
    ConfigurationSettings settings;
    if(argc != 2)
    {
        cout << "Incorrect number of command line parameters. Only need name of file" << endl;
        exit(0);
    }

    ReadConfig(settings, argv[1]);
    settings.outputSettingsToConsole();


    return 0;
    //
}

//
// Free Function Implementation ///////////////////////////////////
//

/***********************************************************
Function Specification: StoreFamily
============================================================
Preconditions:
 - 
Postconditions:
 - 
Algorithm:
 - 
============================================================
***********************************************************/
/* Parameters:

 fileName -
*/
void ReadConfig(ConfigurationSettings &settings, char* fileName)
{
    string settingKeys[] = {
        "Start Simulator Configuration File",
        "Version/Phase",
        "File Path",
        "Processor cycle time (msec)",
        "Monitor display time (msec)",
        "Hard drive cycle time (msec)",
        "Printer cycle time (msec)",
        "Keyboard cycle time (msec)",
        "Log",
        "Log File Path",
        "End Simulator Configuration File"
    };
    //try
    //{
        ifstream configFile;
        configFile.open(fileName, ios::in);
        if(configFile.is_open())
        {
            string tempString; // Used for holding each line
            getline(configFile, tempString);
            // First line must be Start ... File
            if(tempString.compare("Start Simulator Configuration File") != 0)
            {
                cout << "Config file must start with Start Simulator Configuration File" << endl;
                exit(0);
                // throw error("Config file must start with Start Simulator Configuration File");
            }
            // Default value of settings.numberOfLines is 9
            // If the phase changes to 2.0 or 3.0, it will update to 10 or 11
            // Respectively.
            // If any inputs are invalid, the program will terminate from the
            // set method in settings
            for(int currentLine = 1; getline(configFile, tempString, ':') && currentLine <= settings.numberOfLines; currentLine++)
            {
                if(tempString.compare(settingKeys[currentLine]) != 0)
                {
                    // throw error("Error at " + settingKeys[currentLine]);
                }
                getline(configFile, tempString);
                settings.set(currentLine, tempString);
            }

            getline(configFile, tempString, '\n');
            if(tempString.compare("End Simulator Configuration File\n") != 0)
            {
                cout << "Config File must end with End Simulator Configuration File" << endl;
                exit(0);
                // throw error("Config File must end with End Simulator Configuration File")
            }

        }
   // }
    //catch()
    //{

    //}
}
//
// Class/Data Structure Member Implementation //////////////////////
//


//
// Terminating Precompiler Directives ///////////////////////////////
//
 #endif // SIM01_CPP
//