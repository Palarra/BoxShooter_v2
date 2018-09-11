#include "Timer.h"

Timer::Timer()
{

}

Timer::Timer(float duree)
{
	clock.restart();
	m_duree = duree;
	old_timer = -3600000.0f;
}

void Timer::refresh()
{
	clock.restart();
	old_timer = 0;
}

bool Timer::isOver()
{
	timer = clock.getElapsedTime().asMilliseconds();
	if (timer - old_timer >= m_duree * 1000)
	{
		old_timer = 0;
		clock.restart();
		return true;
	}
	else
	{
		return false;
	}
}