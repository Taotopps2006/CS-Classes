//////////////////////////////////////////////////////////////////////
/* 
 FileName: PhaseThree.cpp

 Description: 
 Simulates a multi-program Operating System. It will accept the
 meta-data for one or more programs with potentially unlimited number of
 meta-data operations, run each program, and end the simulation. Each unique
 I/O operation must be conducted with its own unique thread.
 Additional Operations for this Phase:
 - CPU Scheduling now utilizes Round Robin with a quantum
 - FIFO: The order of the next process is determined by the order in which the processes came into the system
 - SRTF: The order of the next process is determined by which process has the shortest remaining time left

 */
// Version/Revision Information ///////////////////////////////////
/* 
 0.5 ( 11/16/2015 ) - Tim Kwist
 Copied Phase Two code to new folder
 */
 // Program Description/Support /////////////////////////////////////
/*
This program relies on building Google's re2 library to be able to utilize
their dynamic library file. The make file is set up to automatically do 
this.
*/
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
 #include "ConfigurationSettings.h"
 #include "Logger.h"
 #include "ProcessControlBlock.h"
 #include "OperatingSystem.h"

 using namespace std;

// Global Constant Definitions ////////////////////////////////////
//
// Defined my Logger as global because it is used in 3 different
// classes and I didn't want to have to redundantly pass it around,
// store it in a class that didn't need it, or have every class
// create a version of its methods.
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
///// NONE
//

// Main Function Implementation ///////////////////////////////////
//
int main( int argc, char * argv[ ] ) 
{
    OperatingSystem os;
    myLog.timer.start( );
    if( argc != 2 )
    {
        cout << 
        "Incorrect number of command line parameters. Only need name of file" 
        << endl;
        exit( 0 );
    }
    os.readConfigurationFile( argv[1] );
    os.readMetaDataFile( );
    os.runSimulator( );
    myLog.timer.stop( );
    myLog.outputLogFile( );

    return 0;
}

//
// Free Function Implementation ///////////////////////////////////
//
///// NONE
//



//
// Class/Data Structure Member Implementation //////////////////////
//
///// NONE
//

//
// Terminating Precompiler Directives ///////////////////////////////
//
 #endif // SIM01_CPP
//