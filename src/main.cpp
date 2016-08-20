#include<SFML\Graphics.hpp>
#include<iostream>
#include<vector>
#include<memory>
#include<string>
#include"CBot.hpp"
#include"GameData.hpp"

#define FIELD_HEIGHT 3
#define FIELD_WIDTH  3

#define SAFE_DELETE(X) {if(X!=NULL){delete(X);}}

std::string playerToString(PLAYER player);

void setupBackground(std::vector<std::vector<sf::RectangleShape*>*> *fieldMatrix);
void setupField     (std::vector<std::vector<Box*>*>                *fieldMatrix);
void clearField     (std::vector<std::vector<Box*>*>                *fieldMatrix);
void setBox         (std::vector<std::vector<Box*>*>                *fieldMatrix, PLAYER player, sf::Vector2i *boxPos);
void checkWinner    (std::vector<std::vector<Box*>*>                *fieldMatrix, bool *done);

int main()
{
	//VARS/////////////////////
	sf::RenderWindow *window    = new sf::RenderWindow(sf::VideoMode(340, 340), "Tic Tac Toe");
	sf::Event        *sfEvent   = new sf::Event();
	sf::FloatRect    *boxBounds = new sf::FloatRect();
	PLAYER currentPlayer;
	bool *done       = new bool;
	bool *againstBot = new bool;
	char *playerMode = new char;

	boxBounds->height = 100;
	boxBounds->width  = 100;

	currentPlayer = PLAYER::PLAYER_1;

	*done = false;

	//BACKGROUND_INIT//////////
	std::vector<std::vector<sf::RectangleShape*>*> *backgroundMatrix = new std::vector<std::vector<sf::RectangleShape*>*>();
	setupBackground(backgroundMatrix);

	//FIELD_INIT///////////////
	std::vector<std::vector<Box*>*> *fieldMatrix = new std::vector<std::vector<Box*>*>();
	setupField(fieldMatrix);

	//BOT_INIT/////////////////
	CBot *bot = new CBot(fieldMatrix);

	std::cout << ">Gegen <B>ot oder realen <S>pieler spielen: ";
	while (!*done) {
		std::cin >> *playerMode;
		if ((*playerMode == 'B') || (*playerMode == 'b')) {
			*done       = true;
			*againstBot = true;
		} else if ((*playerMode == 'S') || (*playerMode == 's')) {
			*done       = true;
			*againstBot = false;
		} else {
			std::cout << ">Ungueltige Eingabe. Bitte erneut versuchen." << std::endl;
		}
	}

	*done = false;
	while (window != NULL && window->isOpen()) {
		if (sfEvent != NULL && done != NULL && againstBot != NULL && bot != NULL && window != NULL && fieldMatrix != NULL && backgroundMatrix != NULL && boxBounds != NULL) {
			while (window->pollEvent(*sfEvent)) {
				if (sfEvent->type == sf::Event::Closed) {
					window->close();
				}
				if (sfEvent->type == sf::Event::MouseButtonReleased && *done == false) {
					if (currentPlayer == PLAYER::PLAYER_1 || (currentPlayer == PLAYER::PLAYER_2 && *againstBot == false)) {
						for (int y = 0; y < fieldMatrix->size(); y++) {
							for (int x = 0; x < fieldMatrix->at(y)->size(); x++) {
								boxBounds->left = x * 110 + 10;
								boxBounds->top = y * 110 + 10;
								if (boxBounds->contains(sf::Mouse::getPosition(*window).x,
									sf::Mouse::getPosition(*window).y) &&
									*fieldMatrix->at(y)->at(x)->isSet != true) {
									setBox(fieldMatrix, currentPlayer, &sf::Vector2i(x, y));
									currentPlayer == PLAYER::PLAYER_1 ? currentPlayer = PLAYER::PLAYER_2 : currentPlayer = PLAYER::PLAYER_1;
									checkWinner(fieldMatrix, done);
									std::cout << ">main: P1 set box at x = " << x << " y = " << y << std::endl;
								}
							}
						}
					}
				}
				if (sfEvent->type == sf::Event::KeyReleased) {
					switch (sfEvent->key.code) {
					case sf::Keyboard::Escape:
						window->close();
						break;
					case sf::Keyboard::C:
						clearField(fieldMatrix);
						currentPlayer = PLAYER::PLAYER_1;
						*done = false;
						break;
					}
				}
			}
			window->clear();

			for (int y = 0; y < backgroundMatrix->size(); y++) {
				for (int x = 0; x < backgroundMatrix->at(y)->size(); x++) {
					window->draw(*backgroundMatrix->at(y)->at(x));
					if (*fieldMatrix->at(y)->at(x)->isSet) {
						window->draw(*fieldMatrix->at(y)->at(x)->Sprite);
					}
				}
			}

			if (*againstBot == true && *done == false && currentPlayer == PLAYER::PLAYER_2) {
				bot->UpdateField(fieldMatrix);
				currentPlayer = PLAYER::PLAYER_1;
				checkWinner(fieldMatrix, done);
			}

			window->display();
		}
	}

	SAFE_DELETE(bot);
	SAFE_DELETE(done);
	SAFE_DELETE(window);
	SAFE_DELETE(sfEvent);
	SAFE_DELETE(boxBounds);
	SAFE_DELETE(againstBot);
	SAFE_DELETE(fieldMatrix);
	SAFE_DELETE(backgroundMatrix);

	return 0;
}

void setupBackground(std::vector<std::vector<sf::RectangleShape*>*> *fieldMatrix) { //SETUP THE BACKGROUND 
	std::vector<sf::RectangleShape*> *tmpFieldVec;
	sf::RectangleShape *tmpRect;
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		tmpFieldVec = new std::vector<sf::RectangleShape*>();
		for (int x = 0; x < FIELD_WIDTH; x++) {
			tmpRect     = new sf::RectangleShape(sf::Vector2f(100.F, 100.F));
			tmpRect->setPosition(x * 110.F + 10.F, y * 110.F + 10.F);
			tmpRect->setFillColor(sf::Color(161, 161, 161));
			tmpFieldVec->push_back(tmpRect);
		}
		fieldMatrix->push_back(tmpFieldVec);
	}
}

void setupField(std::vector<std::vector<Box*>*> *fieldMatrix) { //SETUP THE FIELD INIT VARS ETC
	std::vector<Box*> *tmpBoxVec;
	Box *tmpBox;
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		tmpBoxVec = new std::vector<Box*>();
		for (int x = 0; x < FIELD_WIDTH; x++) {
			tmpBox = new Box();
			tmpBox->isSet   = new bool;
			*tmpBox->isSet  = false;
			tmpBox->Sprite  = new sf::Sprite();
			tmpBox->Owner   = PLAYER::NONE;
			tmpBoxVec->push_back(tmpBox);
		}
		fieldMatrix->push_back(tmpBoxVec);
	}
}

void setBox(std::vector<std::vector<Box*>*> *fieldMatrix, PLAYER player, sf::Vector2i *boxPos){ //SET THE SELECTED BOX TO A X OR O
	sf::Texture *tmpTex = new sf::Texture();
	if (!tmpTex->loadFromFile("assets\\gfx\\Tiles.png"))
		std::cout << ">Unable to load assets\\gfx\\Tiles.png" << std::endl;
	fieldMatrix->at(boxPos->y)->at(boxPos->x)->Sprite->setTexture(*tmpTex);
	if (player == PLAYER::PLAYER_1)
		fieldMatrix->at(boxPos->y)->at(boxPos->x)->Sprite->setTextureRect(sf::IntRect(0, 0, 25, 25));
	if (player == PLAYER::PLAYER_2)
		fieldMatrix->at(boxPos->y)->at(boxPos->x)->Sprite->setTextureRect(sf::IntRect(25, 0, 25, 25));
	fieldMatrix->at(boxPos->y)->at(boxPos->x)->Sprite->setScale(4.f, 4.f);
	fieldMatrix->at(boxPos->y)->at(boxPos->x)->Sprite->setPosition(boxPos->x * 110.F + 10.F, boxPos->y * 110.F + 10.F);
	fieldMatrix->at(boxPos->y)->at(boxPos->x)->Owner  = player;
	*fieldMatrix->at(boxPos->y)->at(boxPos->x)->isSet = true;
}

void clearField(std::vector<std::vector<Box*>*> *fieldMatrix) { //CLEAR THE FIELD MATRIX
	for (int y = 0; y < fieldMatrix->size(); y++) {
		for (int x = 0; x < fieldMatrix->at(y)->size(); x++) {
			*fieldMatrix->at(y)->at(x)->isSet = false;
			fieldMatrix->at(y)->at(x)->Owner = PLAYER::NONE;
		}
	}
}

void checkWinner(std::vector<std::vector<Box*>*> *fieldMatrix, bool *done) { //CHECK IF SOMEBODY WINS IN THIS MOVE
	int *tmpCount = new int;

	for (int y = 0; y < fieldMatrix->size(); y++) //HORIZONTAL CHECK
		if (*fieldMatrix->at(y)->at(0)->isSet && *fieldMatrix->at(y)->at(1)->isSet && *fieldMatrix->at(y)->at(2)->isSet &&
			 fieldMatrix->at(y)->at(0)->Owner ==  fieldMatrix->at(y)->at(1)->Owner &&
			 fieldMatrix->at(y)->at(1)->Owner ==  fieldMatrix->at(y)->at(2)->Owner) {
			std::cout << ">" << playerToString(fieldMatrix->at(y)->at(0)->Owner) << " hat gewonnen." << std::endl;
			*done = true;
		}
	
	for (int x = 0; x < fieldMatrix->size(); x++) //VERTICAL CHECK
		if (*fieldMatrix->at(0)->at(x)->isSet && *fieldMatrix->at(1)->at(x)->isSet && *fieldMatrix->at(2)->at(x)->isSet &&
			 fieldMatrix->at(0)->at(x)->Owner ==  fieldMatrix->at(1)->at(x)->Owner &&
			 fieldMatrix->at(1)->at(x)->Owner ==  fieldMatrix->at(2)->at(x)->Owner) {
			std::cout << ">" << playerToString(fieldMatrix->at(0)->at(x)->Owner) << " hat gewonnen." << std::endl;
			*done = true;
		}
				
	if (*fieldMatrix->at(0)->at(0)->isSet && *fieldMatrix->at(1)->at(1)->isSet && *fieldMatrix->at(2)->at(2)->isSet && //LEFT UP TO RIGHT DOWN
		 fieldMatrix->at(0)->at(0)->Owner ==  fieldMatrix->at(1)->at(1)->Owner &&
		 fieldMatrix->at(1)->at(1)->Owner ==  fieldMatrix->at(2)->at(2)->Owner) {
		std::cout << ">" << playerToString(fieldMatrix->at(1)->at(1)->Owner) << " hat gewonnen." << std::endl;
		*done = true;
	}
			
	if (*fieldMatrix->at(0)->at(2)->isSet && *fieldMatrix->at(1)->at(1)->isSet && *fieldMatrix->at(2)->at(0)->isSet && //RIGHT UP TO LEFT DOWN
		 fieldMatrix->at(0)->at(2)->Owner ==  fieldMatrix->at(1)->at(1)->Owner &&
		 fieldMatrix->at(1)->at(1)->Owner ==  fieldMatrix->at(2)->at(0)->Owner) {
		std::cout << ">" << playerToString(fieldMatrix->at(1)->at(1)->Owner) << " hat gewonnen." << std::endl;
		*done = true;
	}

}

std::string playerToString(PLAYER player) {
	if (player == PLAYER::PLAYER_1)
		return "Spieler 1";
	if (player == PLAYER::PLAYER_2)
		return "Spieler 2";
	if (player == PLAYER::NONE)
		return "NONE";
}
