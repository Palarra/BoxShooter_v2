#pragma once

#ifndef H_PLAYER
#define H_PLAYER

#include "Arrow.h"
#include "LetalShoot.h"
#include "DynamicSprite.h"
#include "InputType.h"
#include "CollisionCategorie.h"


class Player: public Actor
{
private:
	int p_id;
	int m_bullet_id;

	std::vector<std::unique_ptr<Arrow>> bullets;
	std::vector<b2Vec2> bulletsToCreate;
	std::vector<std::unique_ptr<LetalShoot>> letals;

	entityCategory m_playerType;
	entityCategory m_arrowType;

	bool m_isAlive;
	bool m_isShooting;
	bool m_isLoading;
	bool m_isStun;
	float m_cadence;

	Timer m_timer_fire;
	Timer m_timer_stun;

	std::string m_name;

	DynamicSprite m_loadingSprite;

	b2Vec2 m_position_server;
	b2Vec2 m_mouseDirection;

	sf::Texture m_TextureAim;
	sf::Sprite m_SpriteAim;

	std::vector<Input> m_input;


public:
	Player(b2World& physic_world, std::string name, entityCategory player_type, entityCategory arrow_type);
	void getMovement(sf::RenderWindow &window); /* A renommer get et ne renvoie rien (mettre input à la place) */
	std::vector<std::unique_ptr<Arrow>>& getArrowList();
	std::vector<std::unique_ptr<LetalShoot>>& getLetalList();
	void update();
	void draw(sf::RenderTarget &window);
	void bump(b2Vec2 velocity, float force);
	bool isAlive();
	std::vector<Input> getInputsList();
	bool hasInputs();

	void extractDataPlayer(std::string data);
	void extractDataEnnemyPlayer(std::string data);

	friend std::string& operator <<(std::string& data, Player& character);
}; 
#endif