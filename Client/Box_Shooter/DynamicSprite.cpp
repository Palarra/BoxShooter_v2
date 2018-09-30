#include "DynamicSprite.h"

DynamicSprite::DynamicSprite()
{
	m_isActive = false;
	m_complete = false;
	clock.restart();

	centre = sf::Vector2f(0, 0);

	texture.loadFromFile("Assets/circle4.png");
	texture.setSmooth(true);
	texture_size = texture.getSize();

	renderstate.texture = &texture;
	renderstate.transform = transform;

	for (int i = 0; i < 4; i++)
	{
		parties[i] = sf::VertexArray(sf::TrianglesFan, 4);
		for (int j = 0; j < 4; j++)
		{
			parties[i][j].position = sf::Vector2f(0, 0);
			parties[i][j].texCoords = sf::Vector2f(texture_size.x / 2, texture_size.y / 2);
		}
	}
}

void DynamicSprite::update(sf::Vector2f new_position)
{
	if (!m_isActive)
	{
		m_isActive = true;
		m_complete = false;
		clock.restart();
	}

	deltaTime = clock.restart().asSeconds(); 
	position = new_position;
	float pourcentage = fmod(angle, 45) / 45;


	if (!m_complete)
	{
		parties[quartier][1].position = sf::Vector2f(0, -1.0f * texture_size.y / 2);
		parties[quartier][1].texCoords = sf::Vector2f(texture_size.x / 2, 0);


		if (fmod(angle, 90) < 45)
		{
			parties[quartier][2].position = sf::Vector2f(texture_size.y / 2 * tan(fmod(angle, 45) * b2_pi / 180), -1.0f * texture_size.y / 2);
			parties[quartier][2].texCoords = sf::Vector2f(texture_size.x / 2 + texture_size.y / 2 * tan(fmod(angle, 45) * b2_pi / 180), 0);
		}
		else
		{
			parties[quartier][3].position = sf::Vector2f(texture_size.x / 2, -1.0f * texture_size.y / 2 + (texture_size.y / 2 - (tan((45 - fmod(angle, 45)) * b2_pi / 180) * texture_size.x / 2)));
			parties[quartier][3].texCoords = sf::Vector2f(texture_size.x, texture_size.y / 2 - (tan((45 - fmod(angle, 45)) * b2_pi / 180) * texture_size.x / 2));
		}
	}

	//std::cout <<"Pourcentage : " << pourcentage << " / Angle : " << angle <<  std::endl;

	if (angle < 360)
	{
		angle += (360 * deltaTime);

		quartier = angle / 90;
		portion = angle / 45;

		if (angle >= 360)
		{
			m_complete = true;
			quartier = 3; /* Pour ne pas depasser quand on arrive à 360° */
			parties[3][3].position = sf::Vector2f(texture_size.x / 2, 0);
			parties[3][3].texCoords = sf::Vector2f(texture_size.x, texture_size.y / 2);
		}

		if (portion > old_portion)
		{
			if (portion % 2 == 0) /* On a commencé un nouveau quart */
			{
				parties[quartier - 1][3].position = sf::Vector2f(texture_size.x / 2, 0);
				parties[quartier - 1][3].texCoords = sf::Vector2f(texture_size.x, texture_size.y / 2);
				std::cout << "On ajuste à 90° !!" << std::endl;
			}
			else /* On est dans la seconde moitié du quart */
			{
				parties[quartier][2].position = sf::Vector2f(texture_size.x / 2, -1.0f * texture_size.y / 2);
				parties[quartier][2].texCoords = sf::Vector2f(texture_size.x, 0);
				std::cout << "On ajuste à 45° !!" << std::endl;
			}
			old_portion = portion;
		}
	}
}

void DynamicSprite::draw(sf::RenderTarget &window)
{
	for (int i = 0; i <= quartier; i++)
	{
		sf::Transform transform_tmp;

		transform_tmp.translate(position.x, position.y);
		transform_tmp.rotate(i * 90);

		renderstate.transform = transform_tmp;
		window.draw(parties[i], renderstate);
	}
}

bool DynamicSprite::isComplete()
{
	return m_complete;
}

void DynamicSprite::reset()
{
	angle = 0;
	quartier = 0;
	portion = 0;
	old_portion = 0;
	m_isActive = false;

	/* Réinitialisation des vertices à leurs positions d'origine */
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			parties[i][j].position = sf::Vector2f(0, 0);
			parties[i][j].texCoords = sf::Vector2f(texture_size.x / 2, texture_size.y / 2);
		}
	}
	/* --------------------------------------------------------- */
}