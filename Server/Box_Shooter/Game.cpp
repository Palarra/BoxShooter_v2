#include "World.h"
#include "ContactListener.h"
#include <iostream>


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Box Shooting Server");
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);


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
//#include <string.h>
//
//using namespace std;
//
//void ServerLoop();
//void SendPacket(int Channel, const char pack[11]);
//void InitNetwork();
//
//std::vector <ENetPeer*> client_list;
//ENetEvent event;
//ENetHost * host = 0;
//ENetAddress address;
//
//void main() {
//
//	InitNetwork();
//
//	int run = 1;
//	while (run == 1) 
//	{
//		ServerLoop();
//		std::cout << "Wesh alors !!!!!!!!!!!!!" << std::endl;
//	}
//}
//
//void ServerLoop()
//{
//	std::string msg = "";
//	std::string msg_to_send = "[SERVER] Hello I'm the server !!";
//	// processing incoming events:
//	if (enet_host_service(host, &event, 0) > 0)
//	{
//		do
//		{
//			printf("Checking Incoming");
//			switch (event.type)
//			{
//			case ENET_EVENT_TYPE_CONNECT:
//				printf("  A new connected has been established to %u:%u\n",
//					event.peer->address.host, event.peer->address.port);
//				client_list.push_back(event.peer);
//				break;
//
//			case ENET_EVENT_TYPE_RECEIVE:
//				msg.clear();
//				for (int i = 0; i < event.packet->dataLength; i++)
//				{
//					msg += event.packet->data[i];
//				}
//				std::cout << "Message reçu de la part du client : " << msg << std::endl;
//				SendPacket(0, msg_to_send.c_str());
//				enet_packet_destroy(event.packet); // clean up the packet now that we're done using it
//				break;
//
//			case ENET_EVENT_TYPE_DISCONNECT:
//				printf("  host disconnected.\n");
//				event.peer->data = 0; // reset the peer's client information.
//
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
//	/* Create a reliable packet of size 7 containing "packet\0" */
//	ENetPacket * packet = enet_packet_create(pack,
//		strlen(pack) + 1,
//		ENET_PACKET_FLAG_RELIABLE);
//
//
//	/* Send the packet to the peer over channel id 0. */
//	/* One could also broadcast the packet by         */
//	/* enet_host_broadcast (host, 0, packet);         */
//	enet_peer_send(client_list[0], Channel, packet);
//	/* One could just use enet_host_service() instead. */
//	enet_host_flush(host);
//
//}
//void InitNetwork() {
//	if (enet_initialize() != 0)
//	{
//		fprintf(stderr, "An error occurred while initializing ENet.\n");
//	}
//	atexit(enet_deinitialize);
//
//	printf("Initialized.\n");
//
//
//	/* Bind the server to the default localhost.     */
//	/* A specific host address can be specified by   */
//	/* enet_address_set_host (& address, "x.x.x.x"); */
//
//	address.port = 1234;
//
//	printf("I am server...\n");
//	fflush(stdout);
//
//	address.host = ENET_HOST_ANY;
//	host = enet_host_create(&address,
//		32,   
//		2,   
//		0,
//		0);  
//	if (!host) {
//		fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
//		exit(EXIT_FAILURE);
//	}
//	printf("Host Created at %s.\n", address.host);
//
//
//}