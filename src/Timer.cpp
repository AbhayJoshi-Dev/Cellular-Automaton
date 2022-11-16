#include"Timer.h"

Timer::Timer()
{
	m_startTicks = 0;
	m_pausedTicks = 0;

	m_paused = false;
	m_started = false;
}

void Timer::Start()
{
	m_started = true;
	m_paused = false;

	m_startTicks = SDL_GetTicks();
	m_pausedTicks = 0;
}

void Timer::Stop()
{
	m_started = false;
	m_paused = false;

	m_startTicks = 0;
	m_pausedTicks = 0;
}

void Timer::Pause()
{
	if (m_started && !m_paused)
	{
		m_paused = true;

		m_pausedTicks = SDL_GetTicks() - m_startTicks;
		m_startTicks = 0;
	}
}

void Timer::UnPause()
{
	if (m_started && m_paused)
	{
		m_paused = false;
		m_startTicks = SDL_GetTicks() - m_pausedTicks;
		m_pausedTicks = 0;
	}
}

Uint32 Timer::GetTicks()
{
	Uint32 time = 0;

	if (m_started)
	{
		if (m_paused)
		{
			time = m_pausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - m_startTicks;
		}
	}

	return time;
}

bool Timer::IsStarted()
{
	return m_started;
}

bool Timer::IsPaused()
{
	return m_paused;
}