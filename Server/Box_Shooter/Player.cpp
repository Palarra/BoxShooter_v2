#include "Player.h"
#include "CollisionCategorie.h"

Player::Player(b2World& physic_world, std::string name, b2Vec2 position, entityCategory player_type, entityCategory arrow_type) : Actor(physic_world), m_cadence(10.0f), m_timer_fire(1 /m_cadence), m_timer_stun(0.35f)
{
	m_moveSpeed = 15;
	m_isShooting = false;
	m_isLoading = false;
	m_isStun = false;
	m_name = name;
	m_playerType = player_type;
	m_arrowType = arrow_type;

	/* --- Physics settings --- */

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x / 30.0f, position.y / 30.0f);

	body = physic_world.CreateBody(&bodyDef);

	dynamicBox.SetAsBox(50.0f / 60.0f, 50.0f / 60.0f);	/* Box sans friction */

	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.filter.categoryBits = player_type;

	bodyDef.fixedRotation = true;
	body->CreateFixture(&fixtureDef);
	body->SetUserData(this);

	m_velocity = b2Vec2(0.0f, 0.0f);
	m_force = b2Vec2(0.0f, 0.0f);

	/* ------------------------- */

	m_Texture.loadFromFile("Assets/player1.png");
	m_Texture.setSmooth(true);
	m_Sprite = sf::Sprite(m_Texture, sf::IntRect(0, 0, 50, 50));
	m_size = b2Vec2(50, 50);
	m_Sprite.setOrigin(m_size.x / 2, m_size.y / 2);

	m_TextureAim.loadFromFile("Assets/aiming_trail.png");
	m_TextureAim.setSmooth(true);
	m_SpriteAim = sf::Sprite(m_TextureAim, sf::IntRect(0, 0, 1500, 5));
	m_SpriteAim.setOrigin(0, 2.5);
}

void Player::treatInputs()
{
	m_moveSpeed = 15.0f;
	float moveSpeedFire = 0.5f * m_moveSpeed;

	if (!m_isStun)
	{
		
		if (m_isShooting && m_timer_fire.isOver())
		{
			bullets.push_back(std::make_unique<Arrow>(b2Vec2(m_position.x + 60 * m_mouseDirection.x, m_position.y + 60 * m_mouseDirection.y), m_mouseDirection, m_physic_world, m_arrowType, (ALL & ~ (m_playerType | BOUNDARY | m_arrowType)), *this));
		}
		
		if (m_isShooting || m_isLoading)
		{
			m_moveSpeed = moveSpeedFire;
		}

		for (int i = 0; i < m_input.size(); i++)
		{
			switch (m_input[i])
			{
			case TOP:
				m_velocity.y = -1 * m_moveSpeed;
				break;

			case BOT:
				m_velocity.y = 1 * m_moveSpeed;
				break;

			case LEFT:
				m_velocity.x = -1 * m_moveSpeed;
				break;

			case RIGHT:
				m_velocity.x = 1 * m_moveSpeed;
				break;

			default:
				std::cout << "" << std::endl;
			}
		}
		//m_input.clear();
	}
	
	m_input.clear();
	packetToThreat.clear();
}

std::string Player::getPacketList()
{
	return packetToThreat;
}

std::vector<std::unique_ptr<Arrow>>& Player::getArrowList()
{
	return bullets;
}

std::vector<int> Player::getArrowDeadList()
{
	return m_deadBullets;
}

std::vector<std::unique_ptr<LetalShoot>>& Player::getLetalList()
{
	return letals;
}

b2Vec2 Player::getMouseDirection()
{
	return m_mouseDirection;
}

void Player::update()
{
	Actor::update();

	float clamping = 0.75;
	
	b2Vec2 new_velocity = body->GetLinearVelocity(); /* Variable utilisée seulement si le joueur est stun */ 

	m_Sprite.setRotation(90 - atan2(m_mouseDirection.x, m_mouseDirection.y) * 180 / b2_pi);

	if (m_timer_stun.isOver())
	{
		m_isStun = false;
	}

	if (m_isLoading)
	{
		m_loadingSprite.update(sf::Vector2f(m_position.x, m_position.y));
	}
	else
	{
		m_loadingSprite.reset();
	}

	/* Update des projectiles classiques */
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->update();
	}

	/* Update des letals shoots */
	for (int i = 0; i < letals.size(); i++)
	{
		letals[i]->update();
	}


	if (m_isStun) // On laisse un peu plus d'inertie au player si il est stun (pour le bump)
	{
		clamping = 0.85;
	}

	/* Alternative de clamping au lieu de la friction */
	m_velocity.x >= -0.01 && m_velocity.x <= 0.01 ? m_velocity.x = 0 : m_velocity.x = clamping * m_velocity.x; /* Si jamais la velocity est petite */
	m_velocity.y >= -0.01 && m_velocity.y <= 0.01 ? m_velocity.y = 0 : m_velocity.y = clamping * m_velocity.y; /* on la remet à 0				   */
	body->SetLinearVelocity(m_velocity);
}

void Player::draw(sf::RenderTarget &window)
{
	Actor::draw(window);
	
	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->draw(window);
	}

	for (int i = 0; i < letals.size(); i++)
	{
		letals[i]->draw(window);
	}

	if (m_isLoading)
	{
		m_loadingSprite.draw(window);
		m_SpriteAim.setPosition(m_position.x, m_position.y);
		m_SpriteAim.setRotation(90 - atan2(m_mouseDirection.x, m_mouseDirection.y) * 180 / b2_pi);
		window.draw(m_SpriteAim);
	}
}

void Player::bump(b2Vec2 velocity, float force)
{
	m_isStun = true;
	m_timer_stun.refresh();
	//m_force = b2Vec2(velocity.x * force * 250, velocity.y * force * 250);
	m_velocity = b2Vec2(velocity.x * force, velocity.y * force);
}

bool Player::isAlive()
{
	return m_isAlive;
}

bool Player::isLoading()
{
	return m_isLoading;
}

void Player::printInputs()
{
	for (int i = 0; i < m_input.size(); i++)
	{
		//std::cout << m_input[i] << std::endl;
	}
}

void Player::setPacketToTreat(std::string packet)
{
	packetToThreat = packet;
}

void Player::startContact()
{

}

void Player::resetDeadList()
{
	m_deadBullets.clear();
}

Player& operator >>(std::string& data, Player& character)
{
	std::string mouse_position_x, mouse_position_y, input;
	int i = 2;

	while (i < data.size() && data[i] != ';')
	{
		mouse_position_x += data[i];
		i++;
	}
	character.m_mouseDirection.x = std::stof(mouse_position_x);
	i++;

	while (i < data.size() && data[i] != ';')
	{
		mouse_position_y += data[i];
		i++;
	}

	character.m_mouseDirection.y = std::stof(mouse_position_y);
	i++;

	if (data[i] == '1')
	{
		character.m_isShooting = true;
	}
	else
	{
		character.m_isShooting = false;
	}

	i += 2;

	if (data[i] == '1')
	{
		character.m_isLoading = true;
	}
	else
	{
		character.m_isLoading = false;
	}

	i += 2;

	while (i < data.size())
	{
		if (data[i] == ';')
		{
			switch (data[i - 1])
			{
				case '0':
					character.m_input.push_back(TOP);
					break;

				case '1':
					character.m_input.push_back(BOT);
					break;

				case '2':
					character.m_input.push_back(LEFT);
					break;

				case '3':
					character.m_input.push_back(RIGHT);
					break;
			}
			input.clear();
		}
		else
		{
			input += data[i];
		}
		i++;
	}
	return character;
}