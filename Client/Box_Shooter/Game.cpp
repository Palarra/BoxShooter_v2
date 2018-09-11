#include "World.h"
#include "ContactListener.h"
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Box Shooting Client");
	window.setFramerateLimit(120);
	window.setVerticalSyncEnabled(false);


	sf::Texture image_background;
	sf::Sprite background;
	image_background.loadFromFile("Assets/background_blue.jpg");
	background = sf::Sprite(image_background, sf::IntRect(0, 0, 1920, 1080));

	World world(window);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color(240, 240, 240));
		window.draw(background);
		world.update();
		window.display();
	}
	return 0;
}

//#define WIN32_LEAN_AND_MEAN
//#define ALLEGRO_NO_MAGIC_MAIN
//#define MAX_CLIENTS 3
//
//#include <vector>
//#include <iostream>
//#include <string>
//#include <stdlib.h>
//#include <stdio.h>
//
//#include <winsock2.h>
//#include <enet/enet.h>
//#include <sstream>
//
//
//
//
//void ClientLoop();
//void SendPacket(int Channel, const char pack[11]);
//void InitNetwork();
//
//
//ENetEvent event;
//ENetHost * host = 0;
//ENetPeer * peer = 0;
//ENetAddress address;
//
////--- main
////---------------------------------------------------------
//
//void main() 
//{
//	std::string msg = "Hello";
//	InitNetwork();
//
//	int run = 1;
//	printf("Checking Incoming\n");
//	while (run == 1) {
//		ClientLoop();
//		SendPacket(0, msg.c_str());
//	}
//}
//
//void ClientLoop() {
//	// processing incoming events:
//	if (enet_host_service(host, &event, 0) > 0)
//	{
//		do
//		{
//			switch (event.type)
//			{
//			case ENET_EVENT_TYPE_CONNECT:
//				printf("  A new connected has been established to %u:%u\n",
//					event.peer->address.host, event.peer->address.port);
//				break;
//
//			case ENET_EVENT_TYPE_RECEIVE:
//				printf("%s says %s on channel %u\n",
//					(char*)event.peer->data, event.packet->data, event.channelID);
//				fflush(stdout);
//
//				enet_packet_destroy(event.packet); // clean up the packet now that we're done using it
//				break;
//
//			case ENET_EVENT_TYPE_DISCONNECT:
//				printf("  host disconnected.\n");
//				fflush(stdout);
//				free(event.peer->data);
//				event.peer->data = 0; // reset the peer's client information.
//				peer = 0;
//			default:
//				break;
//			}
//		}
//		while (enet_host_check_events(host, &event) > 0);
//	}
//}
//
//void SendPacket(int Channel, const char pack[11])
//{
//	std::string data = "2.02548;1.025487;45.00;225.1258;true;false";
//	/* Create a reliable packet of size 7 containing "packet\0" */
//	ENetPacket * packet = enet_packet_create(data.c_str(),
//		strlen(data.c_str()) + 1,
//		ENET_PACKET_FLAG_RELIABLE);
//
//	printf("Je send : %s taille : %d \n", data.c_str(), data.c_str() + 1);
//		/* Send the packet to the peer over channel id 0. */
//	/* One could also broadcast the packet by         */
//	/* enet_host_broadcast (host, 0, packet);         */
//	enet_peer_send(peer, Channel, packet);
//	/* One could just use enet_host_service() instead. */
// 	enet_host_flush(host);
//}
//void InitNetwork() {
//
//	if (enet_initialize() != 0)
//	{
//		fprintf(stderr, "An error occurred while initializing ENet.\n");
//	}
//	atexit(enet_deinitialize);
//
//	printf("Initialized.\n");
//
//	address.port = 1234;
//
//	printf("I am client...\n");
//	fflush(stdout);
//
//	host = enet_host_create(NULL, 1, // create a client host
//		2, // allow only 1 outgoing connection
//		57600, // use 57600 / 8 for 56K modem with 56 Kbps downstream bandwidth
//		14400);// use 14400 / 8 for 56K modem with 14 Kbps upstream bandwidth
//
//	if (!host) {
//		fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
//		exit(EXIT_FAILURE);
//	}
//
//	// connect to server:
//	enet_address_set_host(&address, "");
//	peer = enet_host_connect(host, &address, 2, 0);
//	peer->data = 0; // use this as mark that connection is not yet acknowledged
//	if (!peer) {
//		fprintf(stderr, "No available peers for initiating an ENet connection.\n");
//		exit(EXIT_FAILURE);
//	}
//
//}