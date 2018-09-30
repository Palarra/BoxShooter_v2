#pragma once

#ifndef H_COLLISION
#define H_COLLISION

enum entityCategory
{
	BOUNDARY = 0x0001,
	PLAYER_1 = 0x0002,
	PLAYER_2 = 0x0004,
	ARROW_1 = 0x0008,
	ARROW_2 = 0x0010,
	ALL = BOUNDARY | PLAYER_1 | PLAYER_2 | ARROW_1 | ARROW_2,
};

#endif