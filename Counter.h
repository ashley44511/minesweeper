//
// Created by Ashley on 11/29/2023.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "TextureManager.h"

#ifndef MINESWEEPER_COUNTER_H
#define MINESWEEPER_COUNTER_H

using namespace std;


class Counter {
public:
    sf::Texture& digitsTexture = TextureManager::getTexture("digits");
    sf::Sprite negCounterSprite;
    sf::Sprite firstCounterSprite;
    sf::Sprite secondCounterSprite;
    sf::Sprite thirdCounterSprite;
    int flagCount;
    int rowsForPos;
    bool neg = false;

    Counter(int mines, int rows);
    void AddFlag();
    void SubFlag();
    void UpdateSprites();

};



#endif //MINESWEEPER_COUNTER_H
