#include "ThreadLite.h"

Thread::Thread(void (*callback)(void), TIME_TYPE _interval) {
  _onRun = callback;
  _cached_next_run = 0;
  last_run = 0;

  setInterval(_interval);
}

void Thread::runned() {
  // Saves last_run
  last_run = millis();

  // Cache next run
  _cached_next_run = last_run + interval;
}

void Thread::setInterval(TIME_TYPE _interval) {
  // Save interval
  interval = _interval;

  // Cache the next run based on the last_run
  _cached_next_run = last_run + interval;
}

bool Thread::shouldRun() {
  TIME_TYPE time = millis();

  // If the "sign" bit is set the signed difference would be negative
  bool time_remaining = (time - _cached_next_run) & TIME_OVERFLOW;

  // Exceeded the time limit, AND is enabled? Then should run...
  return !time_remaining;
}

void Thread::run() {
  _onRun();

  // Update last_run and _cached_next_run
  runned();
}
