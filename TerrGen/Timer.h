//Timer.h - Implementation of a class using SDL's timer functions to create a simple timer.
//Programmed by Ed Lu

#pragma once

#include <Windows.h>
#include <SDL.h>

class Timer
{
public:
	Timer();
	Timer(int interval);

	//Setters
	//Sets the interval
	void SetInterval(int interval);
	//Starts the timer
	void Start();
	//Pauses the timer
	void Pause();
	//Stops the timer
	void Stop();

	//Getters
	//Returns true if the timer is started, false if it is not
	bool GetStarted() const;
	//Returns true if the timer is paused, false if it is not
	bool GetPaused() const;
	//Returns the interval
	int GetInterval() const;

	//If an intervals has passed, this returns true
	bool IntervalPassed();
private:
	int Interval;
	int LastGet;

	bool Started;
	bool Paused;
};