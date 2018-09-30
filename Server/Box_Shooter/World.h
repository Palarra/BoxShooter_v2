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
	int nbPlayers;
	b2World m_physic_world;
	MyContactListener myContactListenerInstance;
	Player m_player1;
	Player m_player2;
	sf::RenderWindow &m_window;
	sf::Clock clock;
	float deltaTime;

	int32 velocityIterations;
	int32 positionIterations;

	ENetPeer* client1;
	ENetPeer* client2;
	ENetHost* host;

	ENetEvent event;
	ENetAddress address;

	sf::TcpListener listener;



	std::mutex mtx;

public:
	World(sf::RenderWindow &window);
	void initNetwork();
	void sendPacket(int Channel, const std::string data, ENetPeer *client);
	void createEnvironnement();
	void update();
	void receiveClient1();
	void receiveClient2();
	void sendAllClients();
	void sendToClient1();
	void sendToClient2();
};

#endif