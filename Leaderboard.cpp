//
// Created by Ashley on 11/30/2023.
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include "Leaderboard.h"

Leaderboard::Leaderboard(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
}

void Leaderboard::ReadLeaderboardFile() {

    ifstream readLeader("leaderboard.txt");
    if (readLeader.is_open()) {

        timeVect.clear();
        nameVect.clear();

        string singleLine;
        string time;
        string name;
        string token;
        while(getline(readLeader, singleLine)) {

            istringstream stream(singleLine);
            token = "";

            getline(stream, token, ',');
            time = token;
            getline(stream, token, ',');
            name = token;
            timeVect.push_back(time);
            nameVect.push_back(name);
        }

        readLeader.close();
    }



} //ReadLeaderboardFile scope

void Leaderboard::WriteLeaderboardFile() {
    remove("leaderboard.txt");
    ofstream writeLeader("leaderboard.txt");
    if (writeLeader.is_open()) {
        writeLeader.clear();
        for (int i = 0; i < timeVect.size(); i++) {
            writeLeader << timeVect[i] << ',' << nameVect[i];
            /*
            if (justWon && i == justWonIndex) {
                writeLeader << "*";
                justWon = false;
            }
             */
            writeLeader << endl;
        }
    }
    writeLeader.close();
}

void Leaderboard::CheckIfBeatScore(int minutes, int seconds, string& name) {
    //WriteLeaderboardFile();
    ReadLeaderboardFile();
    bool madeIt = false;
    int place;

    if (timeVect.size() < 1) {
        madeIt = true;
        place = timeVect.size() + 1;
    } else {
        vector<int> allMins;
        vector<int> allSecs;
        int minuteNum;
        int secondNum;
        //get string position using string constructor string(n, character)
        // source: https://cplusplus.com/reference/string/string/string/
        for (int i = 0; i < timeVect.size(); i++) {
            minuteNum = stoi(string(1, timeVect[i][0])) * 10 + stoi(string(1, timeVect[i][1]));
            secondNum = stoi(string(1, timeVect[i][3])) * 10 + stoi(string(1, timeVect[i][4]));
            allMins.push_back(minuteNum);
            allSecs.push_back(secondNum);
        }
        int minimum = minutes;
        vector<int> potentials;
        for (int i = 0; i < allMins.size(); i++) {
            if (minutes < allMins[i]) {
                madeIt = true;
                place = i + 1;
                break;
            }
            if (minutes == allMins[i]) {
                potentials.push_back(i);
            }
        }

        if (!madeIt && !potentials.empty()) { //check seconds if some minutes were equal
            for (int k = 0; k < potentials.size(); k++) {
                if (seconds < allSecs[potentials[k]]) {
                    madeIt = true;
                    place = potentials[k] + 1;
                    break;
                }
            }
            if (!madeIt) {
                for (int k = 0; k < potentials.size(); k++) {
                    if (seconds == allSecs[potentials[k]]) {
                        madeIt = true;
                        place = potentials[k] + 1;
                        break;
                    }
                }
            }
        }
    } //went through minute and time vectors
    if(timeVect.size() < 5 && !madeIt) {
        madeIt = true;
        place = timeVect.size() + 1;
    }
    if (madeIt) {
        starVect = {"", "", "", "", ""};
        starVect[place - 1] = "*";
        string lineTime = "";

        if (minutes < 10) {
            lineTime = lineTime + "0";
        }
        lineTime = lineTime + to_string(minutes);
        lineTime = lineTime + ":";
        if (seconds < 10) {
            lineTime = lineTime + "0";
        }
        lineTime = lineTime + to_string(seconds);
        //line = line + ",";
        //line = line + name;
        if (timeVect.size() >= place) {
            timeVect.insert(timeVect.begin() + (place - 1), lineTime);
            nameVect.insert(nameVect.begin() + (place - 1), name);
            if (timeVect.size() > 5) {
                timeVect.pop_back();
                nameVect.pop_back();
            }

        } else {
            timeVect.push_back(lineTime);
            nameVect.push_back(name);
        }
        WriteLeaderboardFile();
    }

}

void Leaderboard::UpdateLeaderboardWindow() {
    sf::RenderWindow leaderboardWindow(sf::VideoMode(rows * 16 + 50, cols * 16), "Minesweeper");

    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cout << "No font file available for leaderboard" << std::endl;
    }

    sf::Text LeaderboardText;
    LeaderboardText.setString("LEADERBOARD");
    LeaderboardText.setFont(font);
    LeaderboardText.setCharacterSize(20);
    LeaderboardText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    LeaderboardText.setFillColor(sf::Color::White);

    sf::FloatRect LeaderboardTextRect = LeaderboardText.getLocalBounds();
    // Origin is default the top left corner, so we change this to be the middle of the x and y axis
    LeaderboardText.setOrigin(LeaderboardTextRect.left + LeaderboardTextRect.width/2.0f, LeaderboardTextRect.top + LeaderboardTextRect.height/2.0f);
    //position according to page 8
    LeaderboardText.setPosition(sf::Vector2f((rows * 16 + 50)/2.0f, (cols * 16)/2.0f - 120));

    sf::Text PlacesText;
    ReadLeaderboardFile();
    string placesTextString;
    for (int i = 0; i < timeVect.size(); i++) {
        placesTextString += to_string(i+1) + ".\t" + timeVect[i] + "\t" + nameVect[i] + starVect[i] + "\n\n";
    }
    PlacesText.setString(placesTextString);

    PlacesText.setFont(font);
    PlacesText.setCharacterSize(18);
    PlacesText.setStyle(sf::Text::Bold);
    PlacesText.setFillColor(sf::Color::White);

    sf::FloatRect PlacesTextRect = PlacesText.getLocalBounds();
    // Origin is default the top left corner, so we change this to be the middle of the x and y axis
    PlacesText.setOrigin(PlacesTextRect.left + PlacesTextRect.width/2.0f, PlacesTextRect.top + PlacesTextRect.height/2.0f);
    //position according to page 8
    PlacesText.setPosition(sf::Vector2f((rows * 16 + 50)/2.0f, (cols * 16)/2.0f + 20));

    while (leaderboardWindow.isOpen()) {
        sf::Event event;
        while (leaderboardWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // Click X on the window
                leaderboardWindow.close();
            }
        }

        leaderboardWindow.clear(sf::Color::Blue); // Set background color of the window
        leaderboardWindow.draw(LeaderboardText); // have to draw each object for this specific window
        leaderboardWindow.draw(PlacesText);
        leaderboardWindow.display(); // Final
    }
    WriteLeaderboardFile();
}
/*
void Leaderboard::printVectors() {
    for (int i = 0; i < timeVect.size(); i++) {
        cout << timeVect[i] << " " << nameVect[i] << endl;
    }
}
 */
