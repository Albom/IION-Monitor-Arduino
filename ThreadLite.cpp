// Created by Ivan Seidel Gomes, March, 2013. Released into the public domain.
// Copyright © 2018 Stanislav Hnatiuk. All rights reserved.

#include "ThreadLite.h"



Thread::Thread(void (*callback)(void), time_int _interval) {
  _onRun = callback;
  _cached_next_run = 0;
  last_run = millis();
  setInterval(_interval);
}



void Thread::runned() {
  // Saves last_run
  last_run = millis();

  // Cache next run
  _cached_next_run = last_run + interval;
}



void Thread::setInterval(time_int _interval) {
  // Save interval
  interval = _interval;

  // Cache the next run based on the last_run
  _cached_next_run = last_run + interval;
}



bool Thread::shouldRun() {
  time_int time = millis();

  // If the "sign" bit is set the signed difference would be negative
  bool time_remaining = (time - _cached_next_run) & time_over;

  // Exceeded the time limit, AND is enabled? Then should run...
  return !time_remaining;
}



void Thread::run() {
  if (shouldRun()) {
    execute();
  }
}



void Thread::execute() {
  _onRun();
  // Update last_run and _cached_next_run
  runned();
}
