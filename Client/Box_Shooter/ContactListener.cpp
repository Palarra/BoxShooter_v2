#include "Actor.h"
#include "Arrow.h"
#include "Player.h"
#include "ContactListener.h"


int MyContactListener::getArrowAndPlayer(b2Contact* contact, Actor*& player, Actor*& arrow)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//make sure only one of the fixtures was a sensor
	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	if (sensorA && sensorB)
	{
		arrow = static_cast<Actor*>(fixtureA->GetBody()->GetUserData());
		player = static_cast<Actor*>(fixtureB->GetBody()->GetUserData());
		return 0;
	}
	else if (sensorA ^ sensorB)
	{
		if (sensorA) { //fixtureB must be an enemy aircraft
			arrow = static_cast<Actor*>(fixtureA->GetBody()->GetUserData());
			player = static_cast<Actor*>(fixtureB->GetBody()->GetUserData());
		}
		else { //fixtureA must be an enemy aircraft
			arrow = static_cast<Actor*>(fixtureB->GetBody()->GetUserData());
			player = static_cast<Actor*>(fixtureA->GetBody()->GetUserData());
		}
		return 1;
	}

	return -1;
}


void MyContactListener::BeginContact(b2Contact* contact)
{
	Actor *player;
	Actor *arrow;
	int collision_type = getArrowAndPlayer(contact, player, arrow);

	if (collision_type == 1)
	{
		arrow->startContact(player);
	}
	else if (collision_type == 0)
	{
		arrow->startContact();
		player->startContact();
	}

}
