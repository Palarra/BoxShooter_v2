#include "World.h"

World::World(sf::RenderWindow &window) : m_physic_world(b2World(b2Vec2(0.0f, 0.0f))), m_window(window) ,
										 m_player1(Player(m_physic_world, "Antony", b2Vec2(200.0f, 100.0f))), 
										 m_player2(Player(m_physic_world, "Armen", b2Vec2(800.0f, 700.0f)))							
{
	nbPlayers = 0;
	initNetwork();

	/* Une fois qu'on a accepté les deux clients on lances les threads */

	clock.restart();
	velocityIterations = 6;
	positionIterations = 2;
	m_physic_world.SetContactListener(&myContactListenerInstance);
	createEnvironnement();
}


void World::initNetwork() {
	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
	}
	atexit(enet_deinitialize);

	printf("Initialized.\n");


	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */

	address.port = 1234;

	printf("I am server...\n");
	fflush(stdout);

	address.host = ENET_HOST_ANY;
	host = enet_host_create(&address,
		32,
		2,
		0,
		0);
	if (!host) {
		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
		exit(EXIT_FAILURE);
	}
	printf("Host Created at %s.\n", address.host);


}

void World::sendPacket(int Channel, const std::string data, ENetPeer *client)
{
	/* Create a reliable packet of size 7 containing "packet\0" */
	ENetPacket * packet = enet_packet_create(data.c_str(), strlen(data.c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);


	/* Send the packet to the peer over channel id 0. */
	/* One could also broadcast the packet by         */
	/* enet_host_broadcast (host, 0, packet);         */
	enet_peer_send(client, Channel, packet);
	/* One could just use enet_host_service() instead. */
	enet_host_flush(host);

}

void World::createEnvironnement()
{
	/* ----------------------------------------------------------------- */

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 805.0f / 30.0f);

	b2Body* groundBody = m_physic_world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(2000 / 60.0f, 10.0f / 60.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);

	/* -------------- Left wall -------------- */

	b2BodyDef leftwallBodyDef;
	leftwallBodyDef.position.Set(-5.0f / 30.0f, 0.0f);

	b2Body* leftwallBody = m_physic_world.CreateBody(&leftwallBodyDef);

	b2PolygonShape leftwallBox;
	leftwallBox.SetAsBox(10.0f / 60.0f, 2000.0f / 60.0f);

	leftwallBody->CreateFixture(&leftwallBox, 0.0f);

	/* -------------- Right wall -------------- */

	b2BodyDef rightwallBodyDef;
	rightwallBodyDef.position.Set(805.0f / 30.0f, 0.0f);

	b2Body* rightwallBody = m_physic_world.CreateBody(&rightwallBodyDef);

	b2PolygonShape rightwallBox;
	rightwallBox.SetAsBox(10.0f / 60.0f, 2000.0f / 60.0f);

	rightwallBody->CreateFixture(&rightwallBox, 0.0f);

	/* -------------- Sky -------------- */

	b2BodyDef skyBodyDef;
	skyBodyDef.position.Set(0.0f, -5.0f / 30.0f);

	b2Body* skyBody = m_physic_world.CreateBody(&skyBodyDef);

	b2PolygonShape skyBox;
	skyBox.SetAsBox(2000 / 60.0f, 10.0f / 60.0f);

	skyBody->CreateFixture(&skyBox, 0.0f);
}

void World::update()
{
	if (enet_host_service(host, &event, 0) > 0) /* On récupère d'éventuel event */
	{
		do
		{
			std::string data;
			data.clear();
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				std::cout << "Nouveau client connecte : " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
				if (nbPlayers == 0)
					client1 = event.peer;
				else
					client2 = event.peer;

				nbPlayers++;
				sendPacket(0, std::to_string(nbPlayers), event.peer);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				for (int i = 0; i < event.packet->dataLength; i++)
				{
					data += event.packet->data[i];
				}
				//std::cout << "Message reçu de la part du client : " << data << std::endl;

				if (event.peer == client1)
				{
					data >> m_player1;
					m_player1.treatInputs();
				}
				else
				{
					data >> m_player2;
					m_player2.treatInputs();
				}

				enet_packet_destroy(event.packet); // clean up the packet now that we're done using it
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("  host disconnected.\n");
				event.peer->data = 0; // reset the peer's client information.

			default:
				break;
			}
		}
		while (enet_host_check_events(host, &event) > 0);
	}


	if (nbPlayers == 2)
	{
		deltaTime = clock.restart().asSeconds();
	
		
		m_player1.update();
		m_player1.draw(m_window);

		m_player2.update();
		m_player2.draw(m_window);
		
		sendAllClients(); /* Cette étape est faite avant pour éviter d'avoir un retard sur les clients du départ */
		m_physic_world.Step(deltaTime, velocityIterations, positionIterations);
	}

}

//void World::receiveClient1()
//{
//	std::vector<sf::Packet> packetReceive;
//	while (true)
//	{
//		/* On recçoit le paquet du client 1 */
//		sf::Packet packet;
//		client1.receive(packet);
//		packetReceive.push_back(packet);
//		if (mtx.try_lock())
//		{
//			for (int i = 0; i < packetReceive.size(); i++)
//			{
//				m_player1.getPacketList().push_back(packetReceive[i]);
//			}
//			packetReceive.clear();
//			mtx.unlock();
//		}
//	}	
//}
//
//void World::receiveClient2()
//{
//	std::vector<sf::Packet> packetReceive;
//	while (true)
//	{
//		sf::Packet packet;
//		client2.receive(packet);
//		packetReceive.push_back(packet);
//		if (mtx.try_lock())
//		{
//			for (int i = 0; i < packetReceive.size(); i++)
//			{
//				m_player2.getPacketList().push_back(packetReceive[i]);
//			}
//			packetReceive.clear();
//			mtx.unlock();
//		}
//	}
//}
//

void World::sendAllClients()
{
	/* ------------------------ CLIENT 1 ------------------------- */
	sendToClient1();
	/* ------------------------ CLIENT 2 ------------------------- */
	sendToClient2();
}

void World::sendToClient1()
{
	/* On envoie les données de la position du client 1 au client 1, serveur autoritaire sur la position */
	std::string c1_packet_to_send1;
	c1_packet_to_send1 += '1';
	c1_packet_to_send1 += ';';

	b2Vec2 client1_position = m_player1.getPosition();
	c1_packet_to_send1 += std::to_string(client1_position.x);
	c1_packet_to_send1 += ';';
	c1_packet_to_send1 += std::to_string(client1_position.y);
	c1_packet_to_send1 += ';';

	sendPacket(0, c1_packet_to_send1, client1);

	/* On envoie les données de la position du client 2 au client 1*/
	std::string c1_packet_to_send2;
	c1_packet_to_send2 += '2';
	c1_packet_to_send2 += ';';

	b2Vec2 client2_position = m_player2.getPosition();
	c1_packet_to_send2 += std::to_string(client2_position.x);
	c1_packet_to_send2 += ';';
	c1_packet_to_send2 += std::to_string(client2_position.y);

	c1_packet_to_send2 += ';';

	b2Vec2 mouseDirection = m_player2.getMouseDirection();
	c1_packet_to_send2 += std::to_string(mouseDirection.x);
	c1_packet_to_send2 += ';';
	c1_packet_to_send2 += std::to_string(mouseDirection.y);

	c1_packet_to_send2 += ';';

	
	std::vector<std::unique_ptr<Arrow>>& bullets = m_player2.getArrowList();
	for (int i = 0; i < bullets.size(); i++)
	{
		if (!bullets[i]->isSend())
		{
			c1_packet_to_send2 += std::to_string(bullets[i]->getPosition().x);
			c1_packet_to_send2 += ';';
			c1_packet_to_send2 += std::to_string(bullets[i]->getPosition().y);
			c1_packet_to_send2 += ';';
			c1_packet_to_send2 += std::to_string(bullets[i]->getVelocity().x);
			c1_packet_to_send2 += ';';
			c1_packet_to_send2 += std::to_string(bullets[i]->getVelocity().y);
			c1_packet_to_send2 += ';';
			bullets[i]->setisSend(true);
		}
	}
	sendPacket(0, c1_packet_to_send2, client1);
}

void World::sendToClient2()
{
	std::string c2_packet_to_send1;
	c2_packet_to_send1 += '2';
	c2_packet_to_send1 += ';';

	b2Vec2 client2_position = m_player2.getPosition();
	c2_packet_to_send1 += std::to_string(client2_position.x);
	c2_packet_to_send1 += ';';
	c2_packet_to_send1 += std::to_string(client2_position.y);
	c2_packet_to_send1 += ';';

	sendPacket(0, c2_packet_to_send1, client2);


	std::string c2_packet_to_send2;
	c2_packet_to_send2 += '1';
	c2_packet_to_send2 += ';';

	b2Vec2 client1_position = m_player1.getPosition();
	c2_packet_to_send2 += std::to_string(client1_position.x);
	c2_packet_to_send2 += ';';
	c2_packet_to_send2 += std::to_string(client1_position.y);
	c2_packet_to_send2 += ';';

	b2Vec2 mouseDirection = m_player1.getMouseDirection();
	c2_packet_to_send2 += std::to_string(mouseDirection.x);
	c2_packet_to_send2 += ';';
	c2_packet_to_send2 += std::to_string(mouseDirection.y);
	c2_packet_to_send2 += ';';


	
	std::vector<std::unique_ptr<Arrow>>& bullets = m_player1.getArrowList();
	for (int i = 0; i < bullets.size(); i++)
	{
		if (!bullets[i]->isSend())
		{
			c2_packet_to_send2 += std::to_string(bullets[i]->getPosition().x);
			c2_packet_to_send2 += ';';
			c2_packet_to_send2 += std::to_string(bullets[i]->getPosition().y);
			c2_packet_to_send2 += ';';
			c2_packet_to_send2 += std::to_string(bullets[i]->getVelocity().x);
			c2_packet_to_send2 += ';';
			c2_packet_to_send2 += std::to_string(bullets[i]->getVelocity().y);
			c2_packet_to_send2 += ';';
			bullets[i]->setisSend(true);
		}
	}
	
	sendPacket(0, c2_packet_to_send2, client2);
}
