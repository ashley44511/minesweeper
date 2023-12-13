//
// Created by Ashley on 11/30/2023.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#ifndef MINESWEEPER_LEADERBOARD_H
#define MINESWEEPER_LEADERBOARD_H


class Leaderboard {
public:
    vector<string> timeVect;
    vector<string> nameVect;
    vector<string> starVect = {"", "", "", "", ""};
    int rows;
    int cols;
    bool justWon = false;
    bool justWonIndex;

    Leaderboard(int rows, int cols);
    void ReadLeaderboardFile();
    void WriteLeaderboardFile();
    void CheckIfBeatScore(int minutes, int seconds, string& name);
    void UpdateLeaderboardWindow();
    //void printVectors();
};


#endif //MINESWEEPER_LEADERBOARD_H
