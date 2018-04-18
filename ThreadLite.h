#ifndef THREADLITE_H
#define THREADLITE_H

#include <Arduino.h>

#define TIME_TYPE uint16_t
#define TIME_OVERFLOW 0x8000 // uint16_t = 0x8000 or uint32_t = 0x80000000

class Thread{
protected:
	// Desired interval between runs
	TIME_TYPE interval;

	// Last runned time in Ms
	TIME_TYPE last_run;

	// Scheduled run in Ms (MUST BE CACHED)	
	TIME_TYPE _cached_next_run;

	/*
		IMPORTANT! Run after all calls to run()
		Updates last_run and cache next run.
		NOTE: This MUST be called if extending
		this class and implementing run() method
	*/

	// Default is to mark it runned "now"
	void runned();

	// Callback for run() if not implemented
	void (*_onRun)(void);		

public:

	// If the current Thread is enabled or not
	bool enabled;

	// ID of the Thread (initialized from memory adr.)
	
  //int ThreadID;

	Thread(void (*callback)(void), TIME_TYPE _interval);

	// Set the desired interval for calls, and update _cached_next_run
	void setInterval(TIME_TYPE _interval);

	// Default is to check whether it should run "now"
	bool shouldRun();

	// Runs Thread
	void run();
};

#endif // THREADLITE_H
