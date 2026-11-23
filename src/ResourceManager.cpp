#include "ResourceManager.h"
#include "DataPath.h"

using namespace escape;

/*
* Resource Manager Constructor
* Loads all the resources
*/
ResourceManager::ResourceManager(){

	if (!this -> font.loadFromFile(dataPath("aliensCows.ttf"))){}
	
	if (!backgroundSpriteSheet.loadFromFile(dataPath("BackgroundsSpriteSheet.png"))) {}

	if (!objectSpriteSheet.loadFromFile(dataPath("StolenObjectsSpriteSheet.png"))) {}
}

/*
* Returns aliens & cows font
*/
sf::Font* ResourceManager::getFont(){
  	return &this -> font;
}

/*
* Returns the background texture for the levels unlocked on the level select screen
*/
sf::Texture *ResourceManager::getBackgroundTexture(){
    return &this -> backgroundSpriteSheet;
}

/*
* Returns the object texture for the levels unlocked on the level select screen
*/
sf::Texture *ResourceManager::getObjectTexture(){
    return &this -> objectSpriteSheet;
}
