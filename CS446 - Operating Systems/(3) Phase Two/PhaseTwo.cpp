//////////////////////////////////////////////////////////////////////
/* 
 FileName: PhaseTwo.cpp

 Description: 
 Simulates a multi-program Operating System. It will accept the
 meta-data for one or more programs with potentially unlimited number of
 meta-data operations, run each program, and end the simulation. Each unique
 I/O operation must be conducted with its own unique thread.
 Additional Operations for this Phase:
 - CPU Scheduling can be one of the following
 --FIFO: First process in is the first process out
 --SJF: Once the operating system has been loaded, it will analyze each
 program and sort their order by least to greatest in terms of how long it will
 take for each program to run.
 --SRTFN: After each program is run, the operating system will analyze the
 remaining programs and chose the shortest program to run as the next program.

 */
// Version/Revision Information ///////////////////////////////////
/* 
 0.50 ( 10/26/2015 ) - Tim Kwist
 Copied Phase One code to new folder
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