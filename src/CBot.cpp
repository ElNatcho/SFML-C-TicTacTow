#include"CBot.hpp"

CBot::CBot(std::vector<std::vector<Box*>*> *fieldMatrix)
{
	_fieldMatrix   = fieldMatrix;
	_tempPosVec    = new sf::Vector2i();
	_winningBoxes  = new std::vector<sf::Vector2i*>();
	_possibleBoxes = new std::vector<sf::Vector2i*>();
	_criticalBoxes = new std::vector<sf::Vector2i*>();
	_count         = new int;
	_tempPos       = new int;
	_tempRandVar   = new int;
	_BoxSet        = new bool;

	*_count   = 0;
	*_tempPos = 0;
	*_BoxSet  = false;
}

void CBot::UpdateField(std::vector<std::vector<Box*>*> *fieldMatrix)
{
	_fieldMatrix = fieldMatrix;
	*_BoxSet     = false;

	_clearVectors();
	_getBoxes(PLAYER::PLAYER_1, _criticalBoxes);
	_getBoxes(PLAYER::PLAYER_2, _winningBoxes);
	_getPossibleBoxes();

	_coutVecInfo(_criticalBoxes, new std::string("CriticalBoxes"));
	_coutVecInfo(_winningBoxes,  new std::string("WinningBoxes"));
	_coutVecInfo(_possibleBoxes, new std::string("PossibleBoxes"));

	if (_winningBoxes->size() > 0) {
		for (int i = 0; i < _winningBoxes->size(); i++) {
			if (_fieldMatrix->at(_winningBoxes->at(i)->y)->at(_winningBoxes->at(i)->x)->Owner == PLAYER::NONE &&
				*_fieldMatrix->at(_winningBoxes->at(i)->y)->at(_winningBoxes->at(i)->x)->isSet == false) {
				_setBox(_fieldMatrix, PLAYER::PLAYER_2, _winningBoxes->at(i));
				std::cout << ">Bot: set winningBox X = " << _winningBoxes->at(i)->x << " Y = " << _winningBoxes->at(i)->y << std::endl;
				*_BoxSet = true;
				break;
			}
		}
	}
	if (_criticalBoxes->size() > 0 && _winningBoxes->size() <= 0) {
		for (int i = 0; i < _criticalBoxes->size(); i++) {
			if (_fieldMatrix->at(_criticalBoxes->at(i)->y)->at(_criticalBoxes->at(i)->x)->Owner == PLAYER::NONE &&
				*_fieldMatrix->at(_criticalBoxes->at(i)->y)->at(_criticalBoxes->at(i)->x)->isSet == false) {
				_setBox(_fieldMatrix, PLAYER::PLAYER_2, _criticalBoxes->at(i));
				std::cout << ">Bot: set criticalBox X = " << _criticalBoxes->at(i)->x << " Y = " << _criticalBoxes->at(i)->y << std::endl;
				*_BoxSet = true;
				break;
			}
		}
	}
	if ((_possibleBoxes->size() > 0 && _winningBoxes->size() <= 0 && _criticalBoxes->size() <= 0) || *_BoxSet == false) {
		if (_possibleBoxes->size() > 0)
			*_tempRandVar = rand() % (_possibleBoxes->size() - 1);
		else
			return;
		_setBox(_fieldMatrix, PLAYER::PLAYER_2, _possibleBoxes->at(*_tempRandVar));
		std::cout << ">Bot: set possibleBox X = " << _possibleBoxes->at(*_tempRandVar)->x << " Y = " << _possibleBoxes->at(*_tempRandVar)->y << std::endl;
	}
}

void CBot::_getBoxes(PLAYER player, std::vector<sf::Vector2i*> *_boxesVec)
{
	for (int i = 0; i < _fieldMatrix->size(); i++) {
		*_count = 0;
		*_fieldMatrix->at(i)->at(0)->isSet && _fieldMatrix->at(i)->at(0)->Owner == player ? *_count += 1 : *_tempPos = 0;
		*_fieldMatrix->at(i)->at(1)->isSet && _fieldMatrix->at(i)->at(1)->Owner == player ? *_count += 1 : *_tempPos = 1;
		*_fieldMatrix->at(i)->at(2)->isSet && _fieldMatrix->at(i)->at(2)->Owner == player ? *_count += 1 : *_tempPos = 2;
		if (*_count == 2)
			_boxesVec->push_back(new sf::Vector2i(*_tempPos, i));
		//std::cout << ">CBot: hor_Count = " << *_count << std::endl;
	}
	for (int i = 0; i < _fieldMatrix->size(); i++) {
		*_count = 0;
		*_fieldMatrix->at(0)->at(i)->isSet && _fieldMatrix->at(0)->at(i)->Owner == player ? *_count += 1 : *_tempPos = 0;
		*_fieldMatrix->at(1)->at(i)->isSet && _fieldMatrix->at(1)->at(i)->Owner == player ? *_count += 1 : *_tempPos = 1;
		*_fieldMatrix->at(2)->at(i)->isSet && _fieldMatrix->at(2)->at(i)->Owner == player ? *_count += 1 : *_tempPos = 2;
		if (*_count == 2)
			_boxesVec->push_back(new sf::Vector2i(i, *_tempPos));
		//std::cout << ">CBot: ver_Count = " << *_count << std::endl;
	}

	*_count = 0;
	*_fieldMatrix->at(0)->at(0)->isSet && _fieldMatrix->at(0)->at(0)->Owner == player ? *_count += 1 : *_tempPos = 0;
	*_fieldMatrix->at(1)->at(1)->isSet && _fieldMatrix->at(1)->at(1)->Owner == player ? *_count += 1 : *_tempPos = 1;
	*_fieldMatrix->at(2)->at(2)->isSet && _fieldMatrix->at(2)->at(2)->Owner == player ? *_count += 1 : *_tempPos = 2;
	if (*_count == 2)
		_boxesVec->push_back(new sf::Vector2i(*_tempPos, *_tempPos));
	//std::cout << ">CBot: lu2rd_Count = " << *_count << std::endl;

	*_count = 0;
	*_fieldMatrix->at(0)->at(2)->isSet && _fieldMatrix->at(0)->at(2)->Owner == player ? *_count += 1 : *_tempPos = 0;
	*_fieldMatrix->at(1)->at(1)->isSet && _fieldMatrix->at(1)->at(1)->Owner == player ? *_count += 1 : *_tempPos = 1;
	*_fieldMatrix->at(2)->at(0)->isSet && _fieldMatrix->at(2)->at(0)->Owner == player ? *_count += 1 : *_tempPos = 2;
	if (*_count == 2) {
		*_tempPos == 0 ? _boxesVec->push_back(new sf::Vector2i(2, *_tempPos)) : *_tempPos;
		*_tempPos == 1 ? _boxesVec->push_back(new sf::Vector2i(1, *_tempPos)) : *_tempPos;
		*_tempPos == 2 ? _boxesVec->push_back(new sf::Vector2i(0, *_tempPos)) : *_tempPos;
	}
	//std::cout << ">CBot: ru2ld_Count = " << *_count << std::endl;

}

void CBot::_getPossibleBoxes() 
{
	for (int y = 0; y < _fieldMatrix->size(); y++) {
		for (int x = 0; x < _fieldMatrix->at(y)->size(); x++) {
			if (!*_fieldMatrix->at(y)->at(x)->isSet && _fieldMatrix->at(y)->at(x)->Owner == PLAYER::NONE) {
				_possibleBoxes->push_back(new sf::Vector2i(x, y));
			}
		}
	}
}

void CBot::_clearVectors()
{
	_possibleBoxes->clear();
	_winningBoxes->clear();
	_criticalBoxes->clear();
}

void CBot::_setBox(std::vector<std::vector<Box*>*> *fieldMatrix, PLAYER player, sf::Vector2i *boxPos) { //SET THE SELECTED BOX TO A X OR O
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
	fieldMatrix->at(boxPos->y)->at(boxPos->x)->Owner = player;
	*fieldMatrix->at(boxPos->y)->at(boxPos->x)->isSet = true;
}

void CBot::_coutVecInfo(std::vector<sf::Vector2i*> *vec, std::string *vecName) 
{
	if (vec->size() == 0)
		std::cout << ">Bot: No data in " << *vecName << std::endl;
	else if (vec->size() < 0)
		std::cout << ">Bot: Error in " << *vecName << ": size() < 0" << std::endl;
	else {
		for (int i = 0; i < vec->size(); i++) {
			std::cout << ">Bot: " << *vecName << "<" << i << ">: X = " << vec->at(i)->x << " Y = " << vec->at(i)->y << std::endl;
		}
	}
}

CBot::~CBot()
{
	SAFE_DELETE(_count);
	SAFE_DELETE(_BoxSet);
	SAFE_DELETE(_tempPos);
	SAFE_DELETE(_tempPosVec);
	SAFE_DELETE(_tempRandVar);
	//SAFE_DELETE(_fieldMatrix);
	SAFE_DELETE(_winningBoxes);
	SAFE_DELETE(_possibleBoxes);
	SAFE_DELETE(_criticalBoxes);
}