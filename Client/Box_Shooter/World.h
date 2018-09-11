#pragma once

#ifndef H_WORLD
#define H_WORLD

#define WIN32_LEAN_AND_MEAN
#define ALLEGRO_NO_MAGIC_MAIN
#define MAX_CLIENTS 3

#include <winsock2.h>
#include <enet/enet.h>
#include <mutex>
#include "Actor.h"
#include "Player.h"
#include "ContactListener.h"

class World
{
private:
	int m_id;

	b2World m_physic_world;
	MyContactListener myContactListenerInstance; 
	/*std::vector<Actor> tiles;*/
	Player m_player;
	Player m_ennemy_player;
	sf::RenderWindow &m_window;
	sf::Clock clock;
	float deltaTime;

	int32 velocityIterations;
	int32 positionIterations;


	ENetEvent event;
	ENetHost * host = 0;
	ENetPeer * server = 0;
	ENetAddress address;

	std::string m_ip_adress;
	sf::TcpSocket socket;

public:
	World(sf::RenderWindow &window);
	void initNetwork(std::string ip);
	void sendPacket(int Channel, const std::string data);
	void createEnvironnement();
	void update();
	void treatPacket(std::string data);
};

#endif