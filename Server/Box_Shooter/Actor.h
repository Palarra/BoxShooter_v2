#pragma once

#ifndef H_ACTOR
#define H_ACTOR

#include "SFML\Graphics.hpp"
#include "SFML\Network.hpp"
#include "Box2D\Box2D.h"
#include <iostream>
#include <set>

class Actor 
{
protected :

	int m_id;

	float m_moveSpeed;
	float deltaTime;

	bool m_isAlive;

	b2World& m_physic_world;
	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape dynamicBox;
	b2FixtureDef fixtureDef;

	b2Vec2 m_position;
	b2Vec2 m_velocity;
	b2Vec2 m_force;
	b2Vec2 m_size;

	sf::Texture m_Texture;
	sf::Sprite m_Sprite;


public :

	Actor(b2World& physic_world);
	virtual void update();
	virtual void startContact();
	virtual void startContact(Actor *actor);
	b2Vec2 getPosition();
	b2Vec2 getVelocity();
	virtual void draw(sf::RenderTarget &window);
};
#endif