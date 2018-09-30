#include "Arrow.h"
#include "Player.h"

Arrow::Arrow (b2Vec2 position, b2Vec2 velocity, b2World& physic_world, uint16 categoryBits, uint16 maskBits, Player& owner) : Actor(physic_world), m_owner(owner)
{
	m_moveSpeed = 30.0f;
	m_position = position;
	m_velocity = velocity;
	m_contacting = false;
	m_lifeTime = 5.0;
	m_life_timer.restart();

	/* --- Texture settings --- */

	m_Texture.loadFromFile("Assets/blue_arrow.png");
	m_Texture.setSmooth(true);
	sf::Vector2u texture_size = m_Texture.getSize();
	m_size = b2Vec2(texture_size.x, texture_size.y);
	m_Sprite = sf::Sprite(m_Texture, sf::IntRect(0, 0, texture_size.x, texture_size.y));
	m_Sprite.setOrigin(texture_size.x / 2, texture_size.y / 2);
	m_Sprite.setPosition(position.x * 30, position.y * 30);
	/* --- Physics settings --- */

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x / 30, position.y / 30);

	body = physic_world.CreateBody(&bodyDef);
	m_Sprite.rotate(90 - atan2(velocity.x, velocity.y) * 180 / b2_pi);

	dynamicBox.SetAsBox(texture_size.x / 60.0f, texture_size.y / 60.0f);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	bodyDef.fixedRotation = true;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = maskBits;
	body->CreateFixture(&fixtureDef);
	body->SetAngularVelocity(90 - atan2(velocity.x, velocity.y) * 180 / b2_pi);
	body->SetUserData(this); // On dit au body que c'est cette instance le owner pour les collisions après par ex ... */

	/* ------------------------- */

	body->SetLinearVelocity(b2Vec2(velocity.x * m_moveSpeed, velocity.y * m_moveSpeed));
}

void Arrow::update()
{
	Actor::update();

	/* On check si il est toujours en vie, sinon on le delete */
	if (!m_isAlive || m_life_timer.getElapsedTime().asSeconds() >= m_lifeTime)
	{
		destroy();
	}
}

void Arrow::startContact()
{
	m_isAlive = false;
}

void Arrow::startContact(Actor *player)
{
	Player *player_to_bump = static_cast <Player*>(player);
	player_to_bump->bump(m_velocity, 30.0f);
	m_isAlive = false;	
} 

bool Arrow::isAlive()
{
	return m_isAlive;
}

void Arrow::setisAlive(bool alive)
{
	m_isAlive = alive;
}

int Arrow::getId()
{
	return m_id;
}

void Arrow::destroy()
{
	auto it = std::find_if(m_owner.getArrowList().begin()
		, m_owner.getArrowList().end()
		, [this](auto & lookup)
	{
		return lookup.get() == this;
	});

	if (it != m_owner.getArrowList().end())
	{
		if (!m_physic_world.IsLocked())
		{
			m_physic_world.DestroyBody(body);
			body->SetUserData(nullptr);
			body = nullptr;
			m_owner.getArrowList().erase(it);
		}
	}
}