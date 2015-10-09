#ifndef OPERATING_SYSTEM_H
#define OPERATING_SYSTEM_H
#include <string>
#include <fstream>
#include <vector>
#include <re2/re2.h>
#include <cstdlib>
#include "Logger.h"
#include "ProcessControlBlock.h"
using namespace std;
extern Logger myLog;

struct Process
{
	char component;
	string operation;
	int cycleTime;
};

class OperatingSystem
{
public:
	/**
	 * Default constructor for OperatingSystem
	 * 
	 * Pre: None
	 * Post: All OperatingSystem methods are ready* to use
	 * * Assuming their pre conditions are met
	 */
	OperatingSystem( );

	/**
	 * Reads the configuration file and stores each line in a
	 * ConfigurationSettings variable.
	 * At the end of this method, the ConfigurationSettings variable is told
	 * to check if the input is valid and set its own fields. It will
	 * dynamically figure out whether to use Phase 1.0, 2.0, or 3.0 settings
	 * when storing file data. Currently, only Phase 1.0 is implemented.
	 * The end of this method also tells the Logger whether to print to file,
	 * monitor, or both, and to what file if necessary.
	 *
	 * Pre: None
	 * Post: Settings will have all lines of the configuration file
	 * stored appropriately.
	 * 
	 * @param fileName Name of the configuration file
	 */
	void readConfigurationFile( 
		char * fileName );

	/**
	 * Reads the meta data file and stores operations in a
	 * vector of processes variable. Uses Regular Expressions via Google's RE2
	 * library to find operations. Data is not validated in this method, except
	 * for the first and last lines which must be
	 * "Start Program Meta-Data Code:" and "End Program Meta-Data Code."
	 * respectfully.
	 *
	 * Pre: The configuration file must have been read and stored to tell this
	 * method what meta data file to open. Also, the Google RE2 dynamic library
	 * must be available.
	 * Post: The vector of Processes variable will be stored with each meta
	 * data file instruction.
	 */
	void readMetaDataFile( );

	/**
	 * Iterates through the vector of Processes to process each oepration
	 * correctly. Heavily relies on the processOperation method to function
	 * correctly.
	 *
	 * Pre: The vector of processes is full and ready to be read.
	 * Post: The operations of this meta data file will have been carried out
	 * correctly and completely.
	 */
	void runPhaseOneSimulator( );

	/**
	 * FOR DEBUGGING PURPOSES ONlY
	 * This method is used to print out the configuration settings stored in
	 * the settings variable to ensure that the file was read correctly.
	 *
	 * Pre: readConfigurationSettings was called and ran successfully.
	 * Post: The settings will be output, but nothing will be changed.
	 */
	void outputSettingsToConsole( );
private:
	/**
	 * Looks at the current operation being performed and passes it to the
	 * correct evaluation method. If the component is not recognized, the
	 * program crashes. Accepted components: S, A, P, I, O
	 *
	 * Pre: Simulator must be running, or the operation being passed in
	 * must start the simulator
	 * Post: The correct evaluation method will correctly and completely
	 * handle this operation.
	 * 
	 * @param component  S, A, P, I, or O. Tells whether this operation is
	 * handled by the Operating System (S and A) or the process control block
	 * (P, I, O).
	 * @param operation  Describes the exact operation that this component
	 * is trying to achieve. Is not evaluated here, but is passed on.
	 * @param cycleTime The number of cycles that this operation will run for.
	 * Not evaluated in this method, but is passed on.
	 */
	void processOperation( 
		char component, 
		string operation, 
		int cycleTime );

	/**
	 * Specifically evaluates the operating system operations.
	 * Ways this will succeed:
	 * - operation = start, cycleTime = 0, simulatorRunning = false
	 * - operation = end, cycleTime = 0, simulatorRunning = true
	 * All other possibilities cause the program to crash with an
	 * appropriate error message.
	 *
	 * Pre: None
	 * Post: The simulatorRunning bool will be set to either true or false
	 * depending on whether operation is start or end, respectively.
	 *  
	 * @param operation Must be 'start' or 'end'
	 * @param cycleTime Must be 0
	 */
	void evalOperatingSystem( 
		string operation, 
		int cycleTime );

	/**
	 * Specifically evaluates the application operations.
	 * Ways this will succeed:
	 * - operation = start, cycleTime = 0
	 * - operation = end, cycleTime = 0
	 * All other possibilities cause the program to crash with an 
	 * appropriate error message.
	 *
	 * Pre: None
	 * Post: A new application will be created or destroyed in the 
	 * vector of ProcessControlBlock variable.
	 * @param operation Must be 'start' or 'end'
	 * @param cycleTime Must be 0
	 */
	void evalApplication( 
		string operation, 
		int cycleTime );

	/**
	 * Specifically evaluates the process operations.
	 * Operation must be run.
	 * Passes the operation to current application
	 *
	 * Pre: There must be an application available to run this
	 * process.
	 * Post: This process will be handled correctly and completely
	 * by the Process Control Block.
	 * 
	 * @param operation Must be 'run'
	 * @param cycleTime Must be positive.
	 */
	void evalProcess( 
		string operation, 
		int cycleTime );

	/**
	 * Specifically evaluates the input operations.
	 * Possible operations: hard drive, keyboard
	 * 
	 * Pre: There must be an application available to run this
	 * process.
	 * Post: This process will be handled correctly and completely
	 * by the Process Control Block.
	 * 
	 * @param operation Must be 'hard drive' or 'keyboard'
	 * @param cycleTime Must be positive
	 */
	void evalInput( 
		string operation, 
		int cycleTime );

	/**
	 * Specifically evaluates the input operations.
	 * Possible operations: hard drive, monitor, printer
	 * 
	 * Pre: There must be an application available to run this
	 * process.
	 * Post: This process will be handled correctly and completely
	 * by the Process Control Block.
	 * @param operation Must be 'hard drive', 'monitor', or 'printer'
	 * @param cycleTime Must be positive.
	 */
	void evalOutput( 
		string operation, 
		int cycleTime );
	
	bool simulatorRunning; // True if yes, false if no
	int totalNumberOfProcesses; // Number of unique PCB's from this OS
	int indexOfCurProcess; // Current PCB from this OS
	int numberOfProcesses; // Number of current PCB's from this OS
	vector< Process > operatingSystemInstructions; // Instructions from meta-data
	vector< ProcessControlBlock > processes; // PCB's in use
	ConfigurationSettings settings; // Instructions from config file
	// Future: CPU Scheduling Type
	// Future: Quantum time
};
#endif // OPERATING_SYSTEM_H