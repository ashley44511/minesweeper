//
// Created by Ashley on 11/28/2023.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include "TextureManager.h"
#include "SquareStorage.h"

using namespace std;


bool SquareStorage::CoinFlip(int ratio) {
    //Random Number generation using <random> and <chrono> - generates random numbers using current time
    //sources: https://cplusplus.com/reference/random/ and https://cplusplus.com/reference/random/uniform_int_distribution/operator()/
    // construct a trivial random generator engine from a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    std::uniform_int_distribution<int> distribution(0, 400);

    int coin_flip = distribution(generator);  // generates number in the range 0..1
    if (coin_flip == 1) {
        if (_minesAssigned < _mines) {
            _minesAssigned++;
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

SquareStorage::SquareStorage(int height, int width, int mines) {
    _height = height;
    _width = width;
    _mines = mines;
    _totalSquares = _height * _width;
    _squaresTilVictory = _totalSquares - _mines;

    Square* currentSquare;
    for (int rowY = 0; rowY < _height; rowY++) {
        vector<Square*> tempAcross;
        Square* farLeft = new Square(0, rowY, CoinFlip(_totalSquares / _mines));
        tempAcross.push_back(farLeft);

        currentSquare = farLeft;
        for (int x = 0; x < _width - 1; x++) {
            Square* addRight = new Square(x + 1, rowY, CoinFlip(_totalSquares / _mines));
            addRight->_besties[3] = currentSquare;
            currentSquare->_besties[4] = addRight;
            if (rowY > 0) { //check top
                _rows[rowY - 1][x]->_besties[6] = currentSquare;
                currentSquare->_besties[1] = _rows[rowY -1][x];
            }
            if (x > 0 && rowY > 0) { // check top left diagonal
                _rows[rowY - 1][x - 1]->_besties[7] = currentSquare;
                currentSquare->_besties[0] = _rows[rowY - 1][x - 1];
            }
            if (x < _width - 1 && rowY > 0) { // check top right diagonal
                _rows[rowY - 1][x + 1]->_besties[5] = currentSquare;
                currentSquare->_besties[2] = _rows[rowY - 1][x + 1];
            }

            currentSquare = addRight;
            tempAcross.push_back(currentSquare);
        }
        //assign pointers to last in row
        if (rowY > 0) { //check top
            _rows[rowY - 1][_width - 1]->_besties[6] = currentSquare;
            currentSquare->_besties[1] = _rows[rowY -1][_width - 1];
            //check top left diagonal
            _rows[rowY - 1][_width - 2]->_besties[7] = currentSquare;
            currentSquare->_besties[0] = _rows[rowY - 1][_width - 2];
        }

        _rows.push_back(tempAcross);
    }

    while (_minesAssigned < _mines) {
        //Random Number generation using <random> and <chrono> - generates random numbers using current time
        //sources: https://cplusplus.com/reference/random/ and https://cplusplus.com/reference/random/uniform_int_distribution/operator()/
        // construct a trivial random generator engine from a time-based seed:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator (seed);

        std::uniform_int_distribution<int> distributionH(0, _height - 1);
        std::uniform_int_distribution<int> distributionW(0, _width - 1);

        int rand_row = distributionH(generator);  // generates number in the range 0.._height - 1
        for (int i = 0; i < 100; i++) { //makes it more random somehow don't question it if it works??
            rand_row = distributionH(generator);
        }
        int rand_col = distributionW(generator); // generates number in the range 0..._width - 1
        for (int j = 0; j < 1000; j++) {
            rand_col = distributionW(generator);
        }

        if (_rows[rand_row][rand_col]->_mineHere == false) {
            _rows[rand_row][rand_col]->_mineHere = true;
            _minesAssigned++;
        }
    }

}

SquareStorage::~SquareStorage() {
    /*
    Node* currentNode = _head;
    Node* delNode = _head;
    for(int i = 0; i < _countNodes; i++) {
        currentNode = currentNode->next;
        delete delNode;
        delNode = currentNode;
    }
    delete delNode;
     */
    for (int rowIter = 0; rowIter < _rows.size(); rowIter++) {
        Square* currentSquare = _rows[rowIter][0];
        Square* delSquare = _rows[rowIter][0];
        for (int i = 1; i < _rows[rowIter].size(); i++) {
            currentSquare = currentSquare->_besties[4]; //4 is middle right
            delete delSquare;
            delSquare = currentSquare;
        }
        delete delSquare;
    }
}

void SquareStorage::getSurroundingsForAll() {
    for (int i = 0; i < _rows.size(); i++) {
        for (int j = 0; j < _rows[i].size(); j++) {
            _rows[i][j]->checkSurroundings();
        }
    }
}

void SquareStorage::recursiveEmptyClick(Square* board2d) {
    //board._rows[i][j]->_clicked = true;
    //board._rows[i][j]->_squareSprites.tileSprite.setTexture(board._rows[i][j]->_squareSprites.revealedTexture);
    for (int k = 0; k < board2d->_besties.size(); k++) {
        if (board2d->_besties[k] != nullptr && board2d->_besties[k]->_clicked == false && board2d->_besties[k]->_flagHere == false) {
            board2d->_besties[k]->_clicked = true;
            _squaresTilVictory -= 1;

            board2d->_besties[k]->_squareSprites.tileSprite.setTexture(board2d->_besties[k]->_squareSprites.revealedTexture);

            if (board2d->_besties[k]->_nothingHere) {
                recursiveEmptyClick(board2d->_besties[k]);
            }
        }
    }
}

void SquareStorage::NewTiles() {
    //reset _minesAssigned and _squaresTilVictory
    _minesAssigned = 0;
    _squaresTilVictory = _totalSquares - _mines;

    //delete old _rows and square pointers
    for (int rowIter = 0; rowIter < _rows.size(); rowIter++) {
        Square* currentSquare1 = _rows[rowIter][0];
        Square* delSquare = _rows[rowIter][0];
        for (int i = 1; i < _rows[rowIter].size(); i++) {
            currentSquare1 = currentSquare1->_besties[4]; //4 is middle right
            delete delSquare;
            delSquare = currentSquare1;
        }
        delete delSquare;
    }

    //reset _rows vector to have nothing in it
    _rows.clear();

    //make new _rows
    Square* currentSquare;
    for (int rowY = 0; rowY < _height; rowY++) {
        vector<Square*> tempAcross;
        Square* farLeft = new Square(0, rowY, CoinFlip(_totalSquares / _mines));
        tempAcross.push_back(farLeft);

        currentSquare = farLeft;
        for (int x = 0; x < _width - 1; x++) {
            Square* addRight = new Square(x + 1, rowY, CoinFlip(_totalSquares / _mines));
            addRight->_besties[3] = currentSquare;
            currentSquare->_besties[4] = addRight;
            if (rowY > 0) { //check top
                _rows[rowY - 1][x]->_besties[6] = currentSquare;
                currentSquare->_besties[1] = _rows[rowY -1][x];
            }
            if (x > 0 && rowY > 0) { // check top left diagonal
                _rows[rowY - 1][x - 1]->_besties[7] = currentSquare;
                currentSquare->_besties[0] = _rows[rowY - 1][x - 1];
            }
            if (x < _width - 1 && rowY > 0) { // check top right diagonal
                _rows[rowY - 1][x + 1]->_besties[5] = currentSquare;
                currentSquare->_besties[2] = _rows[rowY - 1][x + 1];
            }

            currentSquare = addRight;
            tempAcross.push_back(currentSquare);
        }
        //assign pointers to last in row
        if (rowY > 0) { //check top
            _rows[rowY - 1][_width - 1]->_besties[6] = currentSquare;
            currentSquare->_besties[1] = _rows[rowY -1][_width - 1];
            //check top left diagonal
            _rows[rowY - 1][_width - 2]->_besties[7] = currentSquare;
            currentSquare->_besties[0] = _rows[rowY - 1][_width - 2];
        }

        _rows.push_back(tempAcross);
    }
    //int a = 0;
    while (_minesAssigned < _mines) {
        //a++;
        //Random Number generation using <random> and <chrono> - generates random numbers using current time
        //sources: https://cplusplus.com/reference/random/ and https://cplusplus.com/reference/random/uniform_int_distribution/operator()/
        // construct a trivial random generator engine from a time-based seed:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator (seed);

        std::uniform_int_distribution<int> distributionH(0, _height - 1);
        std::uniform_int_distribution<int> distributionW(0, _width - 1);

        int rand_row = distributionH(generator);  // generates number in the range 0.._height - 1
        for (int i = 0; i < 100; i++) { //makes it more random somehow don't question it if it works??
            rand_row = distributionH(generator);
        }
        int rand_col = distributionW(generator); // generates number in the range 0..._width - 1
        for (int j = 0; j < 1000; j++) {
            rand_col = distributionW(generator);
        }
        if (_rows[rand_row][rand_col]->_mineHere == false) {
            _rows[rand_row][rand_col]->_mineHere = true;
            _minesAssigned = _minesAssigned + 1;
        }
    } //while loop scope

}

void SquareStorage::makeMinesVis() {
    for (int rowIter = 0; rowIter < _rows.size(); rowIter++) {
        for (int i = 0; i < _rows[rowIter].size(); i++) {
           if (_rows[rowIter][i]->_mineHere) {
               _rows[rowIter][i]->_mineVisible = true;
           }
        }
    } //outside for loop scope

    // function scope
}

void SquareStorage::makeMinesInvis() {
    for (int rowIter = 0; rowIter < _rows.size(); rowIter++) {
        for (int i = 0; i < _rows[rowIter].size(); i++) {
            if (_rows[rowIter][i]->_mineHere) {
                _rows[rowIter][i]->_mineVisible = false;
            }
        }
    } //outside for loop scope
}

void SquareStorage::onlyFlagMines() {
    for (int rowIter = 0; rowIter < _rows.size(); rowIter++) {
        for (int i = 0; i < _rows[rowIter].size(); i++) {
            if (_rows[rowIter][i]->_mineHere) {
                _rows[rowIter][i]->_flagHere = true;
            } else {
                _rows[rowIter][i]->_flagHere = false;
            }
        }
    }
}



