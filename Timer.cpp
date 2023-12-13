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
#include "Timer.h"

//Partial code from COP3503 lab demo Fall 2023
///Written by Gabriela Taboada, edited by Summer Bossman.

Timer::Timer(int rows, int cols) {
    firstMinuteSprite.setTexture(digitsTexture);
    firstMinuteSprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
    firstMinuteSprite.setPosition((cols *32) - 97, 32 * (rows + 0.5f) + 16);

    secondMinuteSprite.setTexture(digitsTexture);
    secondMinuteSprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
    secondMinuteSprite.setPosition((cols *32) - 97 + 21, 32 * (rows + 0.5f) + 16);

    firstSecondSprite.setTexture(digitsTexture);
    firstSecondSprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
    firstSecondSprite.setPosition((cols *32) - 54, 32 * (rows + 0.5f) + 16);

    secondSecondSprite.setTexture(digitsTexture);
    secondSecondSprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
    secondSecondSprite.setPosition((cols *32) - 54 + 21, 32 * (rows + 0.5f) + 16);
}

void Timer::nowPaused() {
    paused = true;
}

void Timer::nowUnpaused() {
    paused = false;
}

void Timer::victoryFreeze(int minutes, int seconds) {
    this->victoryMins = minutes;
    this->victorySecs = seconds;
}

void Timer::resetVictoryInts() {
    this->victoryMins = -1;
    this->victorySecs = -1;
}


