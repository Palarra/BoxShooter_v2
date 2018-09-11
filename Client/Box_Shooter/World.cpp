#include "World.h"

World::World(sf::RenderWindow &window) : m_physic_world(b2World(b2Vec2(0.0f, 0.0f))), m_window(window) ,
										 m_player(Player(m_physic_world, "Antony")), 
										 m_ennemy_player(Player(m_physic_world, "Armen"))
{
	bool connected = false;
	std::cout << "Taper votre ip :" << std::endl;
	std::cin >> m_ip_adress;

	initNetwork(m_ip_adress);

	while (!connected)
	{
		enet_host_service(host, &event, 0);
		do
		{
			std::string data;
			switch (event.type)
			{
				std::cout << "Event type" << event.type << std::endl;
			case ENET_EVENT_TYPE_RECEIVE:

				for (int i = 0; i < event.packet->dataLength; i++)
				{
					data += event.packet->data[i];
				}
				std::cout << "[CLIENT] Mon id : " << std::stoi(data) << std::endl;
				m_id = std::stoi(data);
				connected = true;
			}
		}
		while (enet_host_check_events(host, &event) > 0);
	}

	/* On initialise le jeu .... */
	clock.restart();

	velocityIterations = 6;
	positionIterations = 2;
	m_physic_world.SetContactListener(&myContactListenerInstance);
	createEnvironnement();

}


void World::initNetwork(std::string ip) {

	if (enet_initialize() != 0)
	{
		fprintf(stderr, "An error occurred while initializing ENet.\n");
	}
	atexit(enet_deinitialize);
	address.port = 1234;

	std::cout << "Client initialise" << std::endl;
	fflush(stdout);

	host = enet_host_create(NULL, 1, // create a client host
		2, // allow only 1 outgoing connection
		57600, // use 57600 / 8 for 56K modem with 56 Kbps downstream bandwidth
		14400);// use 14400 / 8 for 56K modem with 14 Kbps upstream bandwidth

	if (!host) {
		fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
		exit(EXIT_FAILURE);
	}

	// connect to server:
	enet_address_set_host(&address, ip.c_str());
	server = enet_host_connect(host, &address, 2, 0);
	server->data = 0; // use this as mark that connection is not yet acknowledged
	if (!server) {
		fprintf(stderr, "No available peers for initiating an ENet connection.\n");
		exit(EXIT_FAILURE);
	}

}


void World::sendPacket(int Channel, const std::string data)
{
	/* Create a reliable packet of size 7 containing "packet\0" */
	ENetPacket * packet = enet_packet_create(data.c_str(),
		strlen(data.c_str()) + 1,
		ENET_PACKET_FLAG_RELIABLE);

	//printf("Je send : %s taille : %d \n", data.c_str(), data.c_str() + 1);
		/* Send the packet to the peer over channel id 0. */
	/* One could also broadcast the packet by         */
	/* enet_host_broadcast (host, 0, packet);         */
	enet_peer_send(server, Channel, packet);
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
	if (enet_host_service(host, &event, 0) > 0)
	{
		std::string data;
		do
		{
			data.clear();
			switch (event.type)
			{
				
				case ENET_EVENT_TYPE_RECEIVE:
					for (int i = 0; i < event.packet->dataLength; i++)
					{
						data += event.packet->data[i];
					}
					//std::cout << data << std::endl;
					treatPacket(data);
					enet_packet_destroy(event.packet); // clean up the packet now that we're done using it
					break;
		
				case ENET_EVENT_TYPE_DISCONNECT:
					printf("  host disconnected.\n");
					fflush(stdout);
					free(event.peer->data);
					event.peer->data = 0; // reset the peer's client information.
					server = 0;
				default:
					break;
			}
		}
		while (enet_host_check_events(host, &event) > 0);
	}


	deltaTime = clock.restart().asSeconds();
	m_player.getMovement(m_window);
	m_player.update();
	m_player.draw(m_window);


	m_ennemy_player.update();
	m_ennemy_player.draw(m_window);


	std::string data_to_send;
	data_to_send = std::to_string(m_id); /* On inscrit en tête du packet l'id du client */
	data_to_send += ';';

	data_to_send << m_player; /* On extract toutes les données du player que l'on veut envoyer dans le paquet */
	sendPacket(0, data_to_send);

	m_physic_world.Step(deltaTime, velocityIterations, positionIterations);
	std::string fps;
	fps += "FPS : ";
	fps += std::to_string(1 / deltaTime);
	m_window.setTitle(fps);
}

void World::treatPacket(std::string data)
{
	int id = data[0] - '0';
	if (id == m_id)
	{
		m_player.extractDataPlayer(data);
	}
	else
	{
		m_ennemy_player.extractDataEnnemyPlayer(data);
	}
}