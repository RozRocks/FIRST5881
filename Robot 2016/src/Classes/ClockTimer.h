/*
 * ClockTimer.h
 *
 *  Created on: Feb 23, 2016
 *      Author: Developer
 */

#ifndef SRC_CLASSES_CLOCKTIMER_H_
#define SRC_CLASSES_CLOCKTIMER_H_

#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

class ClockTimer {
public:
	ClockTimer(double seconds);
	virtual ~ClockTimer();
	void start();
	void stop();
	bool expired();
	double remaining();

private:
	double timeRemaining;
	bool isRunning;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};

#endif /* SRC_CLASSES_CLOCKTIMER_H_ */
