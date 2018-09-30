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
	std::string packetToThreat;
	std::vector<std::unique_ptr<Arrow>> bullets;
	std::vector<std::unique_ptr<LetalShoot>> letals;

	int m_bullet_id;
	std::vector<int> m_deadBullets;

	bool m_isAlive;
	bool m_isShooting;
	bool m_isLoading;
	bool m_isStun;

	entityCategory m_playerType;
	entityCategory m_arrowType;

	float m_cadence;

	Timer m_timer_fire;
	Timer m_timer_stun;

	std::string m_name;

	DynamicSprite m_loadingSprite;

	b2Vec2 m_mouseDirection;

	sf::Texture m_TextureAim;
	sf::Sprite m_SpriteAim;

	std::vector<Input> m_input; /* TODO : Supprimer cette variable elle ne nous sert plus */

public:
	Player(b2World& physic_world, std::string name, b2Vec2 position, entityCategory player_type, entityCategory arrow_type);
	void treatInputs(); /* A renommer get et ne renvoie rien (mettre input à la place) */

	std::string getPacketList();
	std::vector<std::unique_ptr<Arrow>>& getArrowList();
	std::vector<int> getArrowDeadList();
	std::vector<std::unique_ptr<LetalShoot>>& getLetalList();
	
	b2Vec2 getMouseDirection();
	bool isLoading();

	void update();
	void draw(sf::RenderTarget &window);
	void bump(b2Vec2 velocity, float force);
	bool isAlive();
	void printInputs();
	void resetDeadList();
	void setPacketToTreat(std::string packet);
	void startContact();

	friend Player& operator >>(std::string& data, Player& character);
}; 
#endif