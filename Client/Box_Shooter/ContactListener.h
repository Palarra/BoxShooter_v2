#pragma once

#ifndef H_CONTACTLISTENER
#define H_CONTACTLISTENER

#include "Actor.h"
#include "Arrow.h"

class MyContactListener : public b2ContactListener
{

public:  

	bool getArrowAndPlayer(b2Contact* contact, Actor*& radarEntity, Actor*& aircraftEntity);
	void BeginContact(b2Contact* contact);
};
#endif
