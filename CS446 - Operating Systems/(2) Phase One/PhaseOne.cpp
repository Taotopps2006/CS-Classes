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
 #include <iostream>
 #include <fstream>
 #include <string>
 #include "ConfigurationSettings.h"
 #include "Logger.h"
 #include "OperatingSystem.h"
 #include "ProcessControlBlock.h"

 using namespace std;

// Global Constant Definitions ////////////////////////////////////
//
Logger myLog;
//

//
// Class Definitions //////////////////////////////////////////////
//
///// NONE
//

//
// Free Function Prototypes ///////////////////////////////////////
//

// Main Function Implementation ///////////////////////////////////
//
int main( int argc, char * argv[ ] ) 
{
    OperatingSystem os;
    myLog.timer.start();
    if(argc != 2)
    {
        cout << "Incorrect number of command line parameters. Only need name of file" << endl;
        exit(0);
    }
    os.readConfigurationFile(argv[1]);
    os.outputSettingsToConsole();
    os.readMetaDataFile();
    myLog.timer.stop();

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



//
// Class/Data Structure Member Implementation //////////////////////
//


//
// Terminating Precompiler Directives ///////////////////////////////
//
 #endif // SIM01_CPP
//