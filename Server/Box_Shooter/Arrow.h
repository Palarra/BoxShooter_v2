#pragma once

#ifndef H_ARROW
#define H_ARROW

#include "Actor.h"

class Player;


class Arrow : public Actor
{
private :

	Player &m_owner;
	bool m_contacting;
	bool m_isSend;
	float m_lifeTime;
	sf::Clock m_life_timer;
	
public :

	Arrow(b2Vec2 position, b2Vec2 velocity, b2World& physic_world, uint16 categoryBits, uint16 maskBits, Player &owner);
	void update();
	void startContact();
	void startContact(Actor *player);
	bool isAlive();
	void setisSend(bool send);
	bool isSend();
	int getId();
	void destroy();
};
#endif