#include "Actor.h"

Actor::Actor(b2World& physic_world) : m_physic_world(physic_world)
{
	m_isAlive = true;
}

b2Vec2 Actor::getPosition()
{
	return body->GetPosition();
}

b2Vec2 Actor::getVelocity()
{
	return body->GetLinearVelocity();
}

void Actor::update()
{
	b2Vec2 new_position;
	new_position.x = body->GetPosition().x * 30;
	new_position.y = body->GetPosition().y * 30;

	m_position = new_position;
	m_Sprite.setPosition(m_position.x, m_position.y);
}

void Actor::startContact()
{
}

void Actor::startContact(Actor * actor)
{
}

void Actor::draw(sf::RenderTarget& window)
{
	window.draw(m_Sprite);
}