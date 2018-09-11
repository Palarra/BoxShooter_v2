#pragma once

#ifndef H_TIMER
#define H_TIMER

#include "SFML\Graphics.hpp"
#include <iostream>

class Timer
{
private :

	sf::Clock clock;
	float deltaTime;
	float timer;
	float old_timer;
	float m_duree;

public :

	Timer();
	Timer(float duree);
	void refresh();
	bool isOver();
};
#endif