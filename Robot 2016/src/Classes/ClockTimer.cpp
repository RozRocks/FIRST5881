/*
 * ClockTimer.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: Developer
 */

#include "ClockTimer.h"


ClockTimer::ClockTimer(double seconds) {
	timeRemaining = seconds;
	isRunning = false;
	startTime = Clock::now();
}

ClockTimer::~ClockTimer() {
}

/**
 * Starts a timer. Has no effect if already running.
 */
void ClockTimer::start() {
	if (!isRunning) {
		startTime = Clock::now();
		isRunning = true;
	}
}

/**
 * Stops the timer, and allow it to be re-started later if not expired at stop time.
 */
void ClockTimer::stop() {
	if (isRunning) {
		std::chrono::time_point<std::chrono::high_resolution_clock> endTime = Clock::now();
		std::chrono::duration<double> diff = endTime - startTime;
		if (diff.count() > timeRemaining) {
			timeRemaining = 0;
		} else {
			timeRemaining = timeRemaining - diff.count();
		}
		isRunning = false;
	}
}

/**
 * Returns true if the requested time has expired. Does *not* stop the clock.
 */
bool ClockTimer::expired() {
	if (timeRemaining == 0) {
		return true;
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = Clock::now();
	std::chrono::duration<double> diff = currentTime - startTime;
	return diff.count() > timeRemaining;
}

/**
 * Returns a double of the number of seconds remaining in the clock.
 */
double ClockTimer::remaining() {
	if (timeRemaining == 0 || !isRunning) {
		return timeRemaining;
	}

	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = Clock::now();
	std::chrono::duration<double> diff = currentTime - startTime;
	if (diff.count() > timeRemaining) {
		return  0;
	} else {
		return timeRemaining - diff.count();
	}
}

