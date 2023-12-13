#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
using namespace std;

//TextureManager code from COP3503 in-class demos Fall 2023

class TextureManager {
    static unordered_map<string, sf::Texture> textures;
    //unordered_map<string, sf::Texture> TextureManager::textures;

public:
    static sf::Texture& getTexture(string textureName);
    //unordered_map<string, sf::Texture> textures;
};
