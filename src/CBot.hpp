#ifndef CBOT_HPP
#define CBOT_HPP

#pragma once

#include<SFML\Graphics.hpp>
#include<iostream>
#include<vector>
#include"GameData.hpp"

//enum class PLAYER { NONE = 0, PLAYER_1 = 1, PLAYER_2 = 0 };
//
//struct Box {
//	sf::Sprite *Sprite;
//	bool       *isSet;
//	PLAYER      Owner;
//};

#define SAFE_DELETE(X) {if(X!=NULL){delete(X);}}

class CBot {
public:
	CBot(std::vector<std::vector<Box*>*> *fieldMatrix);  //Konstruktor
	~CBot(); //Destruktor
	
	void UpdateField(std::vector<std::vector<Box*>*> *fieldMatrix);

private:
	std::vector<sf::Vector2i*> *_criticalBoxes;
	std::vector<sf::Vector2i*> *_possibleBoxes;
	std::vector<sf::Vector2i*> *_winningBoxes;
	sf::Vector2i *_tempPosVec;

	std::vector<std::vector<Box*>*> *_fieldMatrix;

	int *_count;
	int *_tempPos;
	int *_tempRandVar;

	bool *_BoxSet;

	void _getBoxes(PLAYER player, std::vector<sf::Vector2i*> *_boxesVec);
	void _getPossibleBoxes();
	void _clearVectors();
	void _coutVecInfo(std::vector<sf::Vector2i*> *vec, std::string *vecName);
	void _setBox(std::vector<std::vector<Box*>*> *fieldMatrix, PLAYER player, sf::Vector2i *boxPos);
};

#endif