#pragma once

#ifndef H_LETALSHOOT
#define H_LETALSHOOT

#include "Actor.h"

class Player;

class LetalShoot : public Actor
{
public :
	LetalShoot(b2Vec2 position, b2Vec2 velocity, b2World& physic_world, uint16 categoryBits, uint16 maskBits, Player &owner);
	void update();
	void startContact(Actor *player);
	void destroy();

private :
	Player &m_owner;
	float m_lifeTime;
	sf::Clock m_life_timer;
	bool m_isLoading;
	bool m_contacting;
};




#endif