//
// Created by Ashley on 11/29/2023.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "TextureManager.h"
#include "Counter.h"


Counter::Counter(int mines, int rows) {
    rowsForPos = rows;
    negCounterSprite.setTexture(digitsTexture);
    negCounterSprite.setTextureRect(sf::IntRect(210, 0, 21, 32));
    negCounterSprite.setPosition(12, 32 * (rowsForPos + 0.5f) + 16);

    firstCounterSprite.setTexture(digitsTexture);
    firstCounterSprite.setTextureRect(sf::IntRect(0, 0, 21, 32)); //left, top, width, height
    firstCounterSprite.setPosition(33, 32 * (rowsForPos + 0.5f) + 16);

    secondCounterSprite.setTexture(digitsTexture);
    secondCounterSprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
    secondCounterSprite.setPosition(33 + 21, 32 * (rowsForPos + 0.5f) + 16);

    thirdCounterSprite.setTexture(digitsTexture);
    thirdCounterSprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
    thirdCounterSprite.setPosition(33 + 21 + 21, 32 * (rowsForPos + 0.5f) + 16);

    flagCount = mines;
}

void Counter::AddFlag() {
    flagCount = flagCount - 1;
}

void Counter::SubFlag() {
    flagCount = flagCount + 1;
}

void Counter::UpdateSprites() {
    if (flagCount < 0) {
        neg = true;
    }
    int temp = abs(flagCount) % 10;
    thirdCounterSprite.setTextureRect(sf::IntRect(21 * temp, 0, 21, 32)); //left, top, width, height
    temp = abs(flagCount) - temp;
    temp = (temp % 100) / 10;
    secondCounterSprite.setTextureRect(sf::IntRect(21 * temp, 0, 21, 32));
    temp = abs(flagCount) - (temp * 10);
    temp = (temp % 1000) / 100;
    firstCounterSprite.setTextureRect(sf::IntRect(21 * temp, 0, 21, 32));
}
