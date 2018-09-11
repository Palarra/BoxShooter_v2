#pragma once

#ifndef H_DYNAMICSPRITE
#define H_DYNAMICSPRITE

#include "Timer.h"
#include "Box2D\Box2D.h"

class DynamicSprite
{
private:

	sf::Clock clock;

	sf::Vector2f centre;
	sf::VertexArray parties[4];

	sf::Texture texture;
	sf::Vector2u texture_size;

	sf::RenderStates renderstate;
	sf::Transform transform;

	sf::Vector2f position;

	float angle;
	float deltaTime;
	int quartier;
	int portion;
	int old_portion;
	bool m_isActive;
	bool m_complete;

public:
	DynamicSprite();
	void update(sf::Vector2f new_position);
	void draw(sf::RenderTarget &physic_world);
	bool isComplete();
	void reset();
	
};
#endif
