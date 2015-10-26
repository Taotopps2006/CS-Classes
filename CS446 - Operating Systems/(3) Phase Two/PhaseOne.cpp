//////////////////////////////////////////////////////////////////////
/* 
 FileName: Sim01.cpp

 Description: 
 Simulates a one-program Operating System. It will accept the
 meta-data for one program with a potentially unlimited number of
 meta-data operations, run it, and end the simulation. Each unique
 I/O operation must be conducted with its own unique thread.

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
    os.runPhaseOneSimulator( );
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