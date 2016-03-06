package org.techvalleyhigh.frc5881.util;

import java.util.Date;

public class ClockTimer {
	/**
	 * Time remaining in milliseconds.
	 */
	private long timeRemaining;
	
	/**
	 * TRUE if the "clock" is currently running, or FALSE if paused or never started.
	 */
	private boolean isRunning = false;
	
	/**
	 * Time (in milliseconds) clock started or 0 if never started.
	 */
	private long startTime = 0;
	
	/**
	 * Initialize a timer of a given length in seconds.
	 * 
	 * @param seconds Seconds to set the timer for. Only accurate to 1ms, fractions truncated
	 */
	public ClockTimer(double seconds) {
		timeRemaining = (long)(seconds * 1000);
	}
	
	/**
	 * Starts or re-starts the current timer if not started. If already started, has no effect.
	 */
	public void start() {
		if (!isRunning) {
			startTime = new Date().getTime();
			isRunning = true;
		}
		
		// If already running we can ignore - makes command calls easier
	}
	
	/**
	 * Stops the current timer if started, saving the time remaining in the timer. If not started,
	 * has no effect.
	 */
	public void stop() {
		if (isRunning) {
			final long diff = new Date().getTime() - startTime;
			if (diff > timeRemaining) {
				// More time has passed then requested
				System.out.println("ClockTimer: Stopping " + (diff - timeRemaining) + "ms late");
			} else {
				timeRemaining = timeRemaining - diff;
			}
			
			isRunning = false;
		}
	}
	
	/**
	 * Indicates if the timer is expired due to time elapsed.
	 * 
	 * @return TRUE if the timer has elapsed it's time, FALSE if time remains
	 */
	public boolean isExpired() {
		if (timeRemaining == 0) {
			return true;
		}
		
		if (!isRunning) {
			return timeRemaining == 0;
		}
		
		final long diff = new Date().getTime() - startTime;
		return diff > timeRemaining;
	}
	
	/**
	 * Gives the amount of time remaining in the timer.
	 * 
	 * @return Amount of time remaining in milliseconds
	 */
	public long remaining() {
		if (timeRemaining == 0 || !isRunning) {
			return timeRemaining;
		}
		
		final long diff = new Date().getTime() - startTime;
		if (diff > timeRemaining) {
			return 0;
		} else {
			return timeRemaining - diff;
		}
	}
}
