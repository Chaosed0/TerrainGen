//Timer.h - Implementation of a class using SDL's timer functions to create a simple timer.
//Programmed by Ed Lu

#include "Timer.h"

Timer::Timer()
{
	Interval = LastGet = Started = Paused = 0;
}

Timer::Timer(int interval)
{
	Interval = interval;
	LastGet = Started = Paused = 0;
}

void Timer::Start()
{
	Started = true;
	LastGet = SDL_GetTicks();
}

void Timer::Pause()
{
	Paused = true;
}

void Timer::Stop()
{
	Started = false;
	Paused = false;
}

bool Timer::GetStarted() const
{
	return Started;
}

bool Timer::GetPaused() const
{
	return Paused;
}

int Timer::GetInterval() const
{
	return Interval;
}

bool Timer::IntervalPassed()
{
	int timeNow = SDL_GetTicks();
	if(Started && timeNow + Interval >= LastGet)
	{
		LastGet = SDL_GetTicks();
		return true;
	}

	return false;
}