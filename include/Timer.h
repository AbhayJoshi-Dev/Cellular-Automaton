#pragma once

#include <SDL.h>

class Timer
{
public:
	Timer();

	void Start();
	void Stop();
	void Pause();
	void UnPause();

	Uint32 GetTicks();

	bool IsStarted();
	bool IsPaused();

private:
	Uint32 m_startTicks;
	Uint32 m_pausedTicks;

	bool m_paused;
	bool m_started;
};