// Created by Ivan Seidel Gomes, March, 2013. Released into the public domain.
// Copyright © 2018 Stanislav Hnatiuk. All rights reserved.

#ifndef THREADLITE_H
#define THREADLITE_H

#include <Arduino.h>



using time_int = uint16_t;
constexpr time_int time_over = (1 << (sizeof (time_int) * 8 - 1));



class Thread{
protected:
	// Desired interval between runs
	time_int interval;

	// Last runned time in Ms
	time_int last_run;

	// Scheduled run in Ms (MUST BE CACHED)	
	time_int _cached_next_run;

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
	Thread(void (*callback)(void), time_int _interval);

	// Set the desired interval for calls, and update _cached_next_run
	void setInterval(time_int _interval);

	// Default is to check whether it should run "now"
	bool shouldRun();

	// Runs Thread if should be it
	void run();

	// Execute Thread
	void execute();
};

#endif // THREADLITE_H
