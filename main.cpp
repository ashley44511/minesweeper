#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include "TextureManager.h"
#include "SquareStorage.h"
#include "Counter.h"
#include "Leaderboard.h"
#include "Timer.h"

using namespace std;


int main() {
    //read config file
    //dimensions never less than 22 col by 16 row
    int cols;
    int rows;
    int mines;
    std::ifstream configFile("config.cfg");
    if (configFile.is_open()) {
        string token;
        std::getline(configFile, token);
        cols = std::stoi(token);
        std::getline(configFile, token);
        rows = std::stoi(token);
        std::getline(configFile, token);
        mines = std::stoi(token);


        configFile.close();
    } else {
        std::cout << "No config file available" << std::endl;
        return 0;
    }


    // Render welcomeWindow obj
    //width, height
    //blue bckgd set in while loop
    int windowWidth = cols * 32;
    int windowHeight = rows * 32 + 100;
    sf::RenderWindow welcomeWindow(sf::VideoMode(windowWidth, windowHeight), "Minesweeper");

    // font - instance of Font object
    sf::Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cout << "No font file available" << std::endl;
        return 0;
    }

    // welcomeText - instance of Text object
    sf::Text welcomeText;
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setFont(font);
    welcomeText.setCharacterSize(24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::White);

    sf::FloatRect welcomeTextRect = welcomeText.getLocalBounds();
    // Origin is default the top left corner, so we change this to be the middle of the x and y axis
    welcomeText.setOrigin(welcomeTextRect.left + welcomeTextRect.width/2.0f, welcomeTextRect.top + welcomeTextRect.height/2.0f);
    //position according to page 8
    welcomeText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f - 150));


    //enterNameText - instance of Text object
    sf::Text enterNameText;
    enterNameText.setString("Enter your name:");
    enterNameText.setFont(font);
    enterNameText.setCharacterSize(20);
    enterNameText.setStyle(sf::Text::Bold);
    enterNameText.setFillColor(sf::Color::White);

    sf::FloatRect enterNameTextRect = enterNameText.getLocalBounds();
    //change origin to be middle of rect
    enterNameText.setOrigin(enterNameTextRect.left + enterNameTextRect.width/2.0f, enterNameTextRect.top + enterNameTextRect.height/2.0f);
    //set position according to page 8
    enterNameText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f - 75));


    //nameInputText - instance of Text obj
    sf::Text nameInputText;
    string nameStr = "";
    nameInputText.setString(nameStr + "|");
    nameInputText.setFont(font);
    nameInputText.setCharacterSize(18);
    nameInputText.setStyle(sf::Text::Bold);
    nameInputText.setFillColor(sf::Color::Yellow);

    sf::FloatRect nameInputTextRect = nameInputText.getLocalBounds();
    //change origin to middle of rect
    nameInputText.setOrigin(nameInputTextRect.left + nameInputTextRect.width/2.0f, nameInputTextRect.top + nameInputTextRect.height/2.0f);
    //set position according to pg 8
    nameInputText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f - 45));

    while (welcomeWindow.isOpen()) {
        sf::Event event;
        while (welcomeWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) { // Click X on the window
                welcomeWindow.close();
                return 0;
            }

            if (event.type == sf::Event::KeyPressed) { // Getting user input
                if (event.key.code == sf::Keyboard::Enter && nameStr.length() > 0) {  // event.key.code == ASCII values
                    welcomeWindow.close();
                }
                if (event.key.code == sf::Keyboard::Backspace) {
                    if (nameStr.length() > 0) {
                        nameStr.erase(nameStr.length() - 1);
                        nameInputText.setString(nameStr + "|");
                        nameInputTextRect = nameInputText.getLocalBounds();
                        nameInputText.setOrigin(nameInputTextRect.left + nameInputTextRect.width/2.0f, nameInputTextRect.top + nameInputTextRect.height/2.0f);
                        //nameInputText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f - 45));
                    }
                }
            }

            if(event.type == sf::Event::TextEntered) { //Used SFML's "Events explained" documentation for character input; Source: https://www.sfml-dev.org/tutorials/2.6/window-events.php
                if (event.text.unicode < 128)
                    if (std::isalpha(static_cast<char>(event.text.unicode))) {
                        if (nameStr.length() == 0) {
                            nameStr.insert(0, 1, std::toupper(static_cast<char>(event.text.unicode)));
                        } else if (nameStr.length() < 10) {
                            nameStr.append(1, std::tolower(static_cast<char>(event.text.unicode)));
                        }
                        nameInputText.setString(nameStr + "|");
                        nameInputTextRect = nameInputText.getLocalBounds();
                        nameInputText.setOrigin(nameInputTextRect.left + nameInputTextRect.width/2.0f, nameInputTextRect.top + nameInputTextRect.height/2.0f);
                        //nameInputText.setPosition(sf::Vector2f(windowWidth/2.0f, windowHeight/2.0f - 45));
                    }
            }
        }

        welcomeWindow.clear(sf::Color::Blue); // Set background color of the window
        welcomeWindow.draw(welcomeText); // have to draw each object for this specific window
        welcomeWindow.draw(enterNameText);
        welcomeWindow.draw(nameInputText);
        welcomeWindow.display(); // Final
    }

    //main window assets after welcomeWindow is closed with enter
    sf::RenderWindow mainWindow(sf::VideoMode(windowWidth, windowHeight), "Minesweeper");

    //happy face button
    sf::Texture& happyfaceTexture = TextureManager::getTexture("face_happy");
    sf::Texture& losefaceTexture = TextureManager::getTexture("face_lose");
    sf::Texture& winfaceTexture = TextureManager::getTexture("face_win");
    sf::Sprite happyfaceSprite;
    happyfaceSprite.setTexture(happyfaceTexture);
    //happyfaceSprite.setScale(0.25, 0.25);
    //happyfaceSprite.setOrigin(happyfaceTexture.getSize().x/2.0f, happyfaceTexture.getSize().y/2.0f);
    happyfaceSprite.setPosition(((cols / 2.0f) * 32) - 32, 32 * (rows + 0.5f));

    //debug button
    sf::Texture& debugTexture = TextureManager::getTexture("debug");
    sf::Sprite debugSprite;
    debugSprite.setTexture(debugTexture);
    //debugSprite.setOrigin(debugTexture.getSize().x/2.0f, debugTexture.getSize().y/2.0f);
    debugSprite.setPosition((cols * 32) - 304, 32 * (rows + 0.5f));

    //pause play button
    sf::Texture& pauseTexture = TextureManager::getTexture("pause");
    sf::Texture& playTexture = TextureManager::getTexture("play");
    sf::Sprite pauseplaySprite;
    pauseplaySprite.setTexture(pauseTexture);
    //pauseplaySprite.setOrigin(pauseTexture.getSize().x/2.0f, pauseTexture.getSize().y/2.0f);
    pauseplaySprite.setPosition((cols * 32) - 240, 32 * (rows + 0.5f));
    bool pauseVisible = true;
    /*
    sf::Texture& playTexture = TextureManager::getTexture("play");
    sf::Sprite playSprite;
    playSprite.setTexture(playTexture);
    playSprite.setOrigin(playTexture.getSize().x/2.0f, playTexture.getSize().y/2.0f);
    playSprite.setPosition(pauseSprite.getPosition().x, pauseSprite.getPosition().y);
    */

    //leaderboard button
    sf::Texture& leaderboardTexture = TextureManager::getTexture("leaderboard");
    sf::Sprite leaderboardSprite;
    leaderboardSprite.setTexture(leaderboardTexture);
    //leaderboardSprite.setOrigin(leaderboardTexture.getSize().x/2.0f, leaderboardTexture.getSize().y/2.0f);
    leaderboardSprite.setPosition((cols * 32) - 176, 32 * (rows + 0.5f));
    bool leaderboardPressed = false;
    bool forceUnpause = false;
    Leaderboard lb(rows, cols);

    //counter
    Counter counter(mines, rows);
    counter.UpdateSprites();

    //generate board data
    SquareStorage board(rows, cols, mines);
    board.getSurroundingsForAll();

    bool victory = false;
    bool victoryLeaderboard = false;
    bool defeat = false;

    //timer
    //Partial code from COP3503 lab demo 2023
    ///Written by Gabriela Taboada, edited by Summer Bossman.
    auto start_time = chrono::high_resolution_clock::now();//starting the timer when the program begins to run
    auto pauseTime = chrono::high_resolution_clock::now(); //initialization
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count(); //initialization
    //bool paused = false; //false when game in not paused, true when the game is paused; fulfilled by pauseVisible on line 174
    int minutes;
    int seconds;
    Timer timerBoi(rows, cols);



    while (mainWindow.isOpen()) {
        sf::Event event;
        while (mainWindow.pollEvent(event)) {
            //Event for when user closes window (right corner button)
            if (event.type == sf::Event::Closed) {
                mainWindow.close();
            }

            // Event for when user clicks the mouse
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse; // 2-dimensional vector of floating point coordinates x,y
                mouse = sf::Mouse::getPosition(mainWindow); // Storing the values of where the event occurred in window


                if (event.mouseButton.button == sf::Mouse::Left) {

                    if (happyfaceSprite.getGlobalBounds().contains(mainWindow.mapPixelToCoords(mouse))) {
                        board.NewTiles();
                        board.getSurroundingsForAll();
                        board._minesVis = false;
                        victory = false;
                        defeat = false;
                        counter.flagCount = mines;
                        counter.UpdateSprites();
                        happyfaceSprite.setTexture(happyfaceTexture);
                        lb.starVect = {"", "", "", "", ""};

                        start_time = chrono::high_resolution_clock::now();//starting the timer when the program begins to run
                        pauseTime = chrono::high_resolution_clock::now(); //initialization
                        elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count(); //initialization
                        timerBoi.resetVictoryInts();
                    }

                    if (leaderboardSprite.getGlobalBounds().contains(mainWindow.mapPixelToCoords(mouse))) {
                        leaderboardPressed = true;
                        if (victory == false && defeat == false) {
                            if (pauseVisible) { //was not paused beforehand, leaderboard pressed
                                pauseVisible = false;
                                forceUnpause = true; //to tell game to unpause when leaderboard closed
                                //change timer
                                pauseTime = chrono::high_resolution_clock ::now();
                            } /*else if (!pauseVisible && leaderboardPressed){ //play button visible (paused), leaderboard pressed

                            }*/
                        }
                    }

                    if (victory == false && defeat == false) {
                        if (pauseplaySprite.getGlobalBounds().contains(mainWindow.mapPixelToCoords(mouse))) {
                            if (pauseVisible) {
                                pauseVisible = false;
                                //change timer
                                pauseTime = chrono::high_resolution_clock ::now();
                            } else { //play button visible
                                pauseVisible = true; //change back to pause button, resume play
                                //change timer
                                auto unPausedTime = chrono::high_resolution_clock::now();
                                elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count(); //Addition is necessary for when hitting the pause button more than once

                            }

                            /*
                             * //Partial code from COP3503 lab demo 2023
                                ///Written by Gabriela Taboada, edited by Summer Bossman.
                            if(pauseBttn.getGlobalBounds().contains(vec.x, vec.y)){
                                paused = !paused; //boolean changed everytime pause button is clicked

                                if(paused) {
                                    pauseTime = chrono::high_resolution_clock::now();

                                }else{
                                    //unpaused
                                    auto unPausedTime = chrono::high_resolution_clock::now();
                                    elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count(); //Addition is necessary for when hitting the pause button more than once
                                }
                            }
                             */
                        }


                        if (pauseVisible) {

                            //check all tiles
                            for (int i = 0; i < board._rows.size(); i++) {
                                for (int j = 0; j < board._rows[i].size(); j++) {
                                    if (board._rows[i][j]->_squareSprites.tileSprite.getGlobalBounds().contains(
                                            mainWindow.mapPixelToCoords(mouse))
                                        && !board._rows[i][j]->_flagHere
                                        && !board._rows[i][j]->_clicked ) {

                                        board._rows[i][j]->_clicked = true;
                                        board._squaresTilVictory -= 1;

                                        board._rows[i][j]->_squareSprites.tileSprite.setTexture(board._rows[i][j]->_squareSprites.revealedTexture);
                                        if (board._rows[i][j]->_nothingHere) {
                                            board.recursiveEmptyClick(board._rows[i][j]);
                                        }

                                        if (board._rows[i][j]->_mineHere) {
                                            defeat = true;
                                            timerBoi.victoryFreeze(minutes, seconds);
                                        } else if (board._squaresTilVictory == 0) {
                                            victory = true;
                                            victoryLeaderboard = true;
                                            timerBoi.victoryFreeze(minutes, seconds);
                                        }

                                    }
                                }
                            } //outside for loop scope


                            if (debugSprite.getGlobalBounds().contains(mainWindow.mapPixelToCoords(mouse))) {
                                if (board._minesVis == false) {
                                    board.makeMinesVis();
                                    board._minesVis = true;
                                } else {
                                    board.makeMinesInvis();
                                    board._minesVis = false;
                                }

                            } //debugSprite bounds check scope
                        } //check pauseVisible scope

                    } //check for victory/defeat scope


                } else if (event.mouseButton.button == sf::Mouse::Right && pauseVisible && !victory && !defeat) {
                    //Update Dec 8: I fixed the recursive empty flag bug
                    const sf::Texture& checkHiddenTexture = TextureManager::getTexture("tile_hidden");
                    for (int i = 0; i < board._rows.size(); i++) {
                        for (int j = 0; j < board._rows[i].size(); j++) {
                            if (board._rows[i][j]->_squareSprites.tileSprite.getGlobalBounds().contains(
                                    mainWindow.mapPixelToCoords(mouse)) && !board._rows[i][j]->_clicked) {

                                if (!board._rows[i][j]->_flagHere) {
                                    board._rows[i][j]->_flagHere = true;
                                    counter.AddFlag();
                                    counter.UpdateSprites();
                                } else {
                                    board._rows[i][j]->_flagHere = false;
                                    counter.SubFlag();
                                    counter.UpdateSprites();
                                }

                                //board._rows[i][j]->_squareSprites.flagSprite.setTexture(board._rows[i][j]->_squareSprites.flagTexture);
                            }
                        }
                    } //outside for loop scope
                } //right click scope

            } //mouse button pressed scope

        } //checking for event scope

        //check for victory/defeat conditions
        if (victory == true) {
            board.onlyFlagMines();
            board.makeMinesInvis();
            counter.flagCount = 0;
            counter.UpdateSprites();
            happyfaceSprite.setTexture(winfaceTexture);

        } else if (defeat == true) {
            board.makeMinesVis();
            happyfaceSprite.setTexture(losefaceTexture);
        }

        //update timer
        //Partial code from COP3503 lab demo 2023
        ///Written by Gabriela Taboada, edited by Summer Bossman.
        //this finds the time elapsed, so the current time - the time the window opened.
        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
        int total_time = game_duration.count(); // necessary to subtract elapsed time later because "game_duration.count()" is const

        //draw everything
        mainWindow.clear(sf::Color::White); //set bg color of window
        mainWindow.draw(happyfaceSprite);
        mainWindow.draw(debugSprite);
        if (pauseVisible) { //aka not paused
            pauseplaySprite.setTexture(pauseTexture);
            /*
             * //Partial code from COP3503 lab demo 2023
             * ///Written by Gabriela Taboada, edited by Summer Bossman.
            if(!paused) {
                //enters if the game is NOT paused. This is the condition that keeps the timer from incrementing when paused.
                total_time =  total_time - elapsed_paused_time; //
                minutes = total_time / 60;
                seconds = total_time % 60;
            }
             */
            //update timer
            total_time =  total_time - elapsed_paused_time; //
            minutes = total_time / 60;
            seconds = total_time % 60;

            for (int i = 0; i < board._rows.size(); i++) {
                for (int j = 0; j < board._rows[i].size(); j++) {
                    //mainWindow.draw(sprite)
                    mainWindow.draw(board._rows[i][j]->_squareSprites.tileSprite);
                    if (board._rows[i][j]->_clicked) {
                        mainWindow.draw(board._rows[i][j]->_squareSprites.numMineSprite);
                    } else if (board._rows[i][j]->_flagHere) {
                        mainWindow.draw(board._rows[i][j]->_squareSprites.flagSprite);
                    }
                    if (board._rows[i][j]->_mineVisible) {
                        mainWindow.draw(board._rows[i][j]->_squareSprites.numMineSprite);
                    }

                }
            }
        } else {
            pauseplaySprite.setTexture(playTexture);
           //draw normal board
            for (int i = 0; i < board._rows.size(); i++) {
                for (int j = 0; j < board._rows[i].size(); j++) {
                    //mainWindow.draw(sprite)
                    mainWindow.draw(board._rows[i][j]->_squareSprites.pauseSprite);
                }
            }
        }
        //update timer
        //Partial code from COP3503 lab demo 2023
        ///Written by Gabriela Taboada, edited by Summer Bossman.
        //"separating" the integers. So.... 68 -> seconds0 = 6 and seconds1 = 8
        int minutes0;
        int minutes1;
        int seconds0;
        int seconds1;
        if (victory || defeat) {
            minutes0 = timerBoi.victoryMins / 10 % 10;
            minutes1 = timerBoi.victoryMins % 10;
            seconds0 = timerBoi.victorySecs / 10 % 10;
            seconds1 = timerBoi.victorySecs % 10;
        } else {
            minutes0 = minutes / 10 % 10; //minutes index 0
            minutes1 = minutes % 10; // minutes index 1
            seconds0 = seconds / 10 % 10; // seconds index 0
            seconds1 = seconds % 10; // seconds index 1
        }

        timerBoi.firstMinuteSprite.setTextureRect(sf::IntRect(21 * minutes0, 0, 21, 32));
        timerBoi.secondMinuteSprite.setTextureRect(sf::IntRect(21 * minutes1, 0, 21, 32));
        timerBoi.firstSecondSprite.setTextureRect(sf::IntRect(21 * seconds0, 0, 21, 32));
        timerBoi.secondSecondSprite.setTextureRect(sf::IntRect(21 * seconds1, 0, 21, 32));
        mainWindow.draw(timerBoi.firstMinuteSprite);
        mainWindow.draw(timerBoi.secondMinuteSprite);
        mainWindow.draw(timerBoi.firstSecondSprite);
        mainWindow.draw(timerBoi.secondSecondSprite);

        mainWindow.draw(pauseplaySprite);
        mainWindow.draw(leaderboardSprite);

        if (counter.neg == true) {
            mainWindow.draw(counter.negCounterSprite);
        }
        mainWindow.draw(counter.firstCounterSprite);
        mainWindow.draw(counter.secondCounterSprite);
        mainWindow.draw(counter.thirdCounterSprite);

        /*
        mainWindow.draw(firstMinuteSprite);
        mainWindow.draw(secondMinuteSprite);
        mainWindow.draw(firstSecondSprite);
        mainWindow.draw(secondSecondSprite);
         */
        mainWindow.display();

        if (leaderboardPressed) {
            //Leaderboard lb(rows, cols);
            //lb.starVect.clear();
            lb.UpdateLeaderboardWindow();
            leaderboardPressed = false;

            //unpause game once leaderboard closed
            if (forceUnpause) {
                pauseVisible = true; //change back to pause button, resume play
                //change timer
                auto unPausedTime = chrono::high_resolution_clock::now();
                elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count(); //Addition is necessary for when hitting the pause button more than once
                forceUnpause = false;
            }
        }

        if (victoryLeaderboard) {
            //Leaderboard lb(rows, cols);
            lb.CheckIfBeatScore(timerBoi.victoryMins, timerBoi.victorySecs, nameStr);
            lb.UpdateLeaderboardWindow();
            victoryLeaderboard = false;
        }

    } //main window open scope


    return 0; //obligatory return for main() function
}