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

using namespace std;

#ifndef MINESWEEPER_TIMER_H
#define MINESWEEPER_TIMER_H

//timer
/*
//Partial code from COP3503 lab demo 2023
 ///Written by Gabriela Taboada, edited by Summer Bossman.
*/
class Timer {
public:
    int victoryMins = -1;
    int victorySecs = -1;
    sf::Texture& digitsTexture = TextureManager::getTexture("digits");
    sf::Sprite firstMinuteSprite;
    sf::Sprite secondMinuteSprite;
    sf::Sprite firstSecondSprite;
    sf::Sprite secondSecondSprite;
    //auto startTime = chrono::high_resolution_clock::now();
    bool paused = false;
    //auto pauseTime;
    //auto elapsedPauseTime;

    Timer(int rows, int cols);
    void nowPaused();
    void nowUnpaused();
    void victoryFreeze(int minutes, int seconds);
    void resetVictoryInts();
};


#endif //MINESWEEPER_TIMER_H
