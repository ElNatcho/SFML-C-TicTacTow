#ifndef GAMEDATA_HPP
#define GAMEDATA_HPP

#pragma once

#include<SFML\Graphics.hpp>

enum class PLAYER { NONE = 0, PLAYER_1 = 1, PLAYER_2 = 0 };

struct Box {
	sf::Sprite *Sprite;
	bool       *isSet;
	PLAYER      Owner;
};


#endif