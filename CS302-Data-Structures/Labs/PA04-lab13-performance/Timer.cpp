/**
  *@file Timer.cpp
  *@author Tim Kwist
  *@version 1.0
  *@date Wednesday, September 23nd, 2014
  *@brief This program implements a stopwatch. It starts, stops and stores the time elapsed in between
  *@details The program uses system time to determine elapsed time. Starting stores the current system time. Stopping gets the current system time and subtracts the beginning time to get the elapsed time.
  */

/********************************************//**
 *  Header Files
 ***********************************************/
#include <sys/time.h>
#include <stdexcept>
#include <iostream>
#include "Timer.h"

using namespace std;
 /********************************************//**
 *  Method implementation
 ***********************************************/
 
 /**
 * Initialize Timer object
 * 
 * @param none
 * @return none
 * @pre None
 * @post Initialize the internal timer valuesso that the timer is ready to measure time
 */
Timer::Timer (  )
{
	timerWasStarted = false;
	beginTime.tv_sec = 0;
	beginTime.tv_usec = 0;
	duration.tv_sec = 0;
	duration.tv_usec = 0;;
}

/**
 * Set timer started to true and record the beginning time
 * 
 * @param none
 * @return none
 * @pre Timer has not started yet
 * @post Mark the beginning of a time interval; start the timer
 */
void Timer::start (  ) throw ( runtime_error )
{
	if (timerWasStarted)
	{
		throw runtime_error ("start() while running");
	}
	
	gettimeofday ( &beginTime, NULL );
	timerWasStarted = true;
}

/**
 * Set timer started to false and record end time of timer
 * 
 * @param none
 * @return none
 * @pre Timer has started
 * @post Marks the end of a time interval; stop the timer
 */
void Timer::stop (  ) throw ( logic_error )
{
	if (!timerWasStarted)
	{
		throw logic_error ("stop() while not running");
	}
	
	timeval end;
	gettimeofday ( &end, NULL );
	duration.tv_sec = ( (end.tv_sec - beginTime.tv_sec) * 1000000 );
	duration.tv_usec = ( end.tv_usec - beginTime.tv_usec );
	timerWasStarted = false;
}

/**
 * Find how much time has elapsed between start and end of timer
 * 
 * @param none
 * @return double Length of time interval in seconds
 * @pre Timer has been started and ended
 * @post Returns the length of the time interval in seconds
 */
double Timer::getElapsedTime (  ) const throw (logic_error)
{
	if (timerWasStarted)
	{
		throw logic_error ("getElapsedTime() while running");
	}
		
	double temp = duration.tv_sec + duration.tv_usec;
	return ( temp / 1000000.0 );
}
















