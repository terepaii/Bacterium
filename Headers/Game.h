#ifndef GAME_H_
#define GAME_H_

#include "MainMenu.h"
#include "CreditsMenu.h"
#include "LevelSelectMenu.h"
#include "SplashScreen.h"
#include "HighScores.h"

#include "InputHandler.h"
#include "World.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "SoundPlayer.h"



class Game
{
public:
	Game();
	~Game();

	void Start();

	void flush();

	static const enum GameState{UNINITIALIZED, SHOWINGSPLASH, CHOOSING_INPUT, PAUSED, 
					SHOWINGMENU, SHOWING_CREDITS, SHOWING_LEVEL_SELECT, PLAYING, SHOWOPTIONS, GAME_OVER, EXITING};

private:
	//Checks to see whether we are exiting the game
	bool isExiting();

	//Main game loop
	void gameLoop();

	//Function to show the splash screen
	void showSplashScreen(sf::RenderWindow& window);

	//Function to show the main menu
	void showMainMenu(sf::RenderWindow& window);

	//Function to show the credits menu
	void showCreditsMenu(sf::RenderWindow& window);

	//Function to show the level select menu
	void showLevelSelectMenu(sf::RenderWindow& window);

	//Loads all content creates the window
	void Load();

	//Handles input from player
	void handleInput();

	//updates custom mouse cursor
	void updateMouseCursor(sf::RenderWindow &window);

	GameState gameState;
	MainMenu mainMenu;
	SplashScreen splashScreen;
	CreditsMenu creditsMenu;
	LevelSelectMenu levelSelectMenu;

	sf::RenderWindow window;
	sf::Sprite mouseCursor;
	sf::Texture mouseCursorTexture;

	InputHandler input;

	World currentWorld;

	TextureManager textureManager;
	SoundManager soundManager;
	FontManager fontManager;

	HighScores highScores;
};

#endif