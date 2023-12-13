//
// Created by Ashley on 11/28/2023.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include "TextureManager.h"

using namespace std;

#ifndef MINESWEEPER_SQUARESTORAGE_H
#define MINESWEEPER_SQUARESTORAGE_H

struct SquareSprites {
    sf::Sprite tileSprite;
    sf::Texture& hiddenTexture = TextureManager::getTexture("tile_hidden");
    sf::Texture& revealedTexture = TextureManager::getTexture("tile_revealed");
    sf::Sprite numMineSprite;
    sf::Texture& num1Texture = TextureManager::getTexture("number_1");
    sf::Texture& num2Texture = TextureManager::getTexture("number_2");
    sf::Texture& num3Texture = TextureManager::getTexture("number_3");
    sf::Texture& num4Texture = TextureManager::getTexture("number_4");
    sf::Texture& num5Texture = TextureManager::getTexture("number_5");
    sf::Texture& num6Texture = TextureManager::getTexture("number_6");
    sf::Texture& num7Texture = TextureManager::getTexture("number_7");
    sf::Texture& num8Texture = TextureManager::getTexture("number_8");
    sf::Texture& mineTexture = TextureManager::getTexture("mine");
    sf::Sprite flagSprite;
    sf::Texture& flagTexture = TextureManager::getTexture("flag");
    sf::Sprite pauseSprite;

    void SetPos(int xPos, int yPos) {
        tileSprite.setTexture(hiddenTexture);
        //tileSprite.setOrigin(hiddenTexture.getSize().x/2.0f, hiddenTexture.getSize().y/2.0f);
        tileSprite.setPosition(xPos * 32, yPos * 32);

        numMineSprite.setTexture(num1Texture);
        //numMineSprite.setOrigin(num1Texture.getSize().x/2.0f, num1Texture.getSize().y/2.0f);
        numMineSprite.setPosition(xPos * 32, yPos * 32);

        flagSprite.setTexture(flagTexture);
        //flagSprite.setOrigin(hiddenTexture.getSize().x/2.0f, hiddenTexture.getSize().y/2.0f);
        flagSprite.setPosition(xPos * 32, yPos * 32);

        pauseSprite.setTexture(revealedTexture);
        pauseSprite.setPosition(xPos * 32, yPos * 32);
    }
};

struct Square {
    vector<Square*> _besties = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    int _xPos;
    int _yPos;
    bool _mineHere;
    bool _flagHere = false;
    bool _nothingHere = false;
    bool _clicked = false;
    bool _mineVisible = false;
    int _bestiesMines = -1;

    SquareSprites _squareSprites;

    Square(int xPos, int yPos, bool mineHere) {
        _xPos = xPos;
        _yPos = yPos;
        _mineHere = mineHere;
        _squareSprites.SetPos(xPos, yPos);
    }

    int checkSurroundings() {
        int count = 0;
        for (int i = 0; i < _besties.size(); i++) {
            if (_besties[i] != nullptr && _besties[i]->_mineHere == true) {
                count += 1;
            }
        }
        if (_mineHere) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.mineTexture);
        } else if (count == 0) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.revealedTexture);
            _nothingHere = true;
        } else if (count == 1) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.num1Texture);
        } else if (count == 2) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.num2Texture);
        } else if (count == 3) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.num3Texture);
        } else if (count == 4) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.num4Texture);
        } else if (count == 5) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.num5Texture);
        } else if (count == 6) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.num6Texture);
        } else if (count == 7) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.num7Texture);
        } else if (count == 8) {
            _squareSprites.numMineSprite.setTexture(_squareSprites.num8Texture);
        }
        _bestiesMines = count;
        return count;
    }
};

class SquareStorage {
private:

    int _height;
    int _width;
    int _totalSquares;
    int _mines;
    int _minesAssigned = 0;
    float _minesRatio;


public:
    int _squaresTilVictory;
    bool _minesVis = false;
    vector<vector<Square*>> _rows;
    bool CoinFlip(int ratio);
    SquareStorage(int height, int width, int mines);
    ~SquareStorage();
    //void displayBoard(sf::RenderWindow mainWindow);
    void getSurroundingsForAll();
    void recursiveEmptyClick(Square* board2d);
    void NewTiles(); //copy assignment operator
    void makeMinesVis();
    void makeMinesInvis();
    void onlyFlagMines();

};


#endif //MINESWEEPER_SQUARESTORAGE_H
