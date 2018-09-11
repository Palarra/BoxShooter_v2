#include "Actor.h"
#include "Arrow.h"
#include "Player.h"
#include "ContactListener.h"


bool MyContactListener::getArrowAndPlayer(b2Contact* contact, Actor*& player, Actor*& arrow)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	//make sure only one of the fixtures was a sensor
	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();
	if (!(sensorA ^ sensorB))
		return false;

	if (sensorA) { //fixtureB must be an enemy aircraft
		player = static_cast<Actor*>(fixtureA->GetBody()->GetUserData());
		arrow = static_cast<Actor*>(fixtureB->GetBody()->GetUserData());
	}
	else { //fixtureA must be an enemy aircraft
		player = static_cast<Actor*>(fixtureB->GetBody()->GetUserData());
		arrow = static_cast<Actor*>(fixtureA->GetBody()->GetUserData());
	}
	return true;
}

void MyContactListener::BeginContact(b2Contact* contact)
{
	Actor *player;
	Actor *arrow;

	if (getArrowAndPlayer(contact, arrow, player))
	{
		arrow->startContact(player);
	}

}
