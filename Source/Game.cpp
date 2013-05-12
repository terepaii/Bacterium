#include "Game.h"

Game::Game()
{
	gameState = UNINITIALIZED;
	Load();
}

Game::~Game()
{
	flush();
}

void Game::Load()
{
	srand((unsigned int)time(NULL));

	window.setFramerateLimit(60);
	//window.create(sf::VideoMode::getDesktopMode(),"Hexagonal Cat", sf::Style::Fullscreen);
	window.create(sf::VideoMode(800, 600), "Bacterium");
	window.setKeyRepeatEnabled(false);

	//load mouse cursor
	if(mouseCursorTexture.loadFromFile("Config/Content/Images/GUI/mouseCursor.png"))
	{
		mouseCursor.setTexture(mouseCursorTexture);
		window.setMouseCursorVisible(false);
	}

	SoundPlayer::getInstance()->init(&soundManager);
	input.load("Config/Input/Mouse_Keyboard.json");
	highScores.load("Config/Levels/Level2/HighScores.json", *fontManager.getResource("Config/Content/Fonts/Samdan.ttf"), *textureManager.getResource("Config/Content/Images/Menus/Backgrounds/HighScoresBackground.png"));

	mainMenu.initMap(window, textureManager);
	creditsMenu.initMap(window, textureManager);
	levelSelectMenu.initMap(window, textureManager);

	currentWorld.init(textureManager, soundManager);
}

void Game::flush()
{
	textureManager.Flush();
	soundManager.Flush();
	fontManager.Flush();

	currentWorld.flush();
}

void Game::Start()
{
	if(gameState != UNINITIALIZED)
		return;
	
	gameState = Game::SHOWINGSPLASH;

	while(!isExiting())
	{
		gameLoop();
	}

	window.close();
}

bool Game::isExiting()
{
	if(gameState == Game::EXITING) 
		return true;
	else 
		return false;
}

void Game::gameLoop()
{
	input.bigBootyLoop(window);
	updateMouseCursor(window);
	handleInput();
	SoundPlayer::getInstance()->Update();
	switch(gameState)
	{
	case Game::SHOWINGMENU:
		{
			window.clear(sf::Color(100, 100, 100));
			mainMenu.Draw(window);
			window.draw(mouseCursor);
			window.display();

			showMainMenu(window);
			break;
		}
	case Game::SHOWINGSPLASH:
		{
			showSplashScreen(window);
			break;
		}
	case Game::SHOWING_CREDITS:
		{
			window.clear();
			creditsMenu.Draw(window);
			window.draw(mouseCursor);
			window.display();

			showCreditsMenu(window);
			break;
		}
	case Game::SHOWING_LEVEL_SELECT:
		{
			window.clear();
			levelSelectMenu.Draw(window);
			window.draw(mouseCursor);
			window.display();

			showLevelSelectMenu(window);
			break;
		}
	case Game::PLAYING:
		{
			//update and draw world
			window.clear(sf::Color::White);
			currentWorld.Update(window);
			currentWorld.Draw(window);
			window.draw(mouseCursor);
			window.display();

			if(currentWorld.isOver())
			{
				highScores.setCurrentScore(currentWorld.getWorldScore());
				currentWorld.flush();
				gameState = GAME_OVER;
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				currentWorld.flush();
				gameState = Game::SHOWINGMENU;
			}

			break;
		}
	case Game::CHOOSING_INPUT:
		{
			sf::Sprite sprite;
			sprite.setTexture(*textureManager.getResource("Config/Content/Images/Menus/Backgrounds/continueBackground.png"));
			sprite.setScale((float)window.getSize().x / sprite.getTexture()->getSize().x, (float)window.getSize().y / sprite.getTexture()->getSize().y);
			sprite.setPosition(0, 0);
			window.clear(sf::Color::Black);
			window.draw(sprite);
			window.draw(mouseCursor);
			window.display();

			for(unsigned int i = 0; i < sf::Keyboard::KeyCount; i++)
			{
				//see if any button has been pressed
				if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i)))
				{
					input.setJoystickNum(-1);
					gameState = SHOWINGMENU;
					break;
				}
			}
			for(unsigned int i = 0; i < sf::Mouse::ButtonCount; i++)
			{
				if(sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i)))
				{
					input.setJoystickNum(-1);
					gameState = SHOWINGMENU;
					break;
				}
			}

			bool pressedAnyButton = false;
			//find out what joystick the player is using
			for(unsigned int i = 0; i < sf::Joystick::Count && !pressedAnyButton; i++)
			{
				if(sf::Joystick::isConnected(i))
				{
					for(unsigned int j = 0; j < sf::Joystick::getButtonCount(i) && !pressedAnyButton; j++)
					{
						if(sf::Joystick::isButtonPressed(i, j))
						{
							input.setJoystickNum(i);
							gameState = SHOWINGMENU;
							pressedAnyButton = true;
						}
					}
				}
			}

			break;
		}
	case GAME_OVER:
		{
			window.clear(sf::Color::Black);
			highScores.update();
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				highScores.addNewHighScore(highScores.getCurrentName(), highScores.getCurrentScore());
				gameState = SHOWINGMENU;
			}

			highScores.draw(window);
			window.draw(mouseCursor);
			window.display();

			break;
		}
	default:
		break;
	}
}

void Game::showSplashScreen(sf::RenderWindow& window)
{
	splashScreen.Show(window, textureManager, input);
	//gameState = Game::SHOWINGMENU;
	gameState = Game::CHOOSING_INPUT;
}

void Game::showCreditsMenu(sf::RenderWindow& window)
{
	CreditsMenu::MenuResult value = creditsMenu.changeScreen(window, input);

	switch(value)
	{
	case CreditsMenu::BACK:
		{
			gameState = Game::SHOWINGMENU;
			break;
		}
	default:
		break;
	}
}

void Game::showLevelSelectMenu(sf::RenderWindow& window)
{
	LevelSelectMenu::MenuResult value = levelSelectMenu.changeScreen(window, input);

	switch(value)
	{
	case LevelSelectMenu::LEVEL1:
		{
			//loading screen
			sf::Sprite sprite;
			sprite.setTexture(*textureManager.getResource("Config/Content/Images/Menus/Loading.png"));
			sprite.setPosition(250, 250);
			window.clear(sf::Color::Black);
			window.draw(sprite);
			window.display();

			gameState = Game::PLAYING;
			currentWorld.Load("Config/Levels/Level1/Setup.json", window, textureManager, fontManager);
			break;
		}
	case LevelSelectMenu::LEVEL2:
		{
			//loading screen
			sf::Sprite sprite;
			sprite.setTexture(*textureManager.getResource("Config/Content/Images/Menus/Loading.png"));
			sprite.setPosition(250, 250);
			window.clear(sf::Color::Black);
			window.draw(sprite);
			window.display();

			gameState = Game::PLAYING;
			currentWorld.Load("Config/Levels/Level2/Setup.json", window, textureManager, fontManager);
			break;
		}
	case LevelSelectMenu::LEVEL3:
		{
			//loading screen
			sf::Sprite sprite;
			sprite.setTexture(*textureManager.getResource("Config/Content/Images/Menus/Loading.png"));
			sprite.setPosition(250, 250);
			window.clear(sf::Color::Black);
			window.draw(sprite);
			window.display();

			gameState = Game::PLAYING;
			currentWorld.Load("Config/Levels/Level3/Setup.json", window, textureManager, fontManager);
			break;
		}
	case LevelSelectMenu::SURVIVAL:
		{
			//loading screen
			sf::Sprite sprite;
			sprite.setTexture(*textureManager.getResource("Config/Content/Images/Menus/Loading.png"));
			sprite.setPosition(250, 250);
			window.clear(sf::Color::Black);
			window.draw(sprite);
			window.display();

			gameState = Game::PLAYING;
			currentWorld.Load("Config/Levels/Survival/Setup.json", window, textureManager, fontManager);
			break;
		}
	case LevelSelectMenu::BACK:
		{
			gameState = Game::SHOWINGMENU;
			break;
		}
	default:
		break;
	}
}

void Game::showMainMenu(sf::RenderWindow& window)
{
	MainMenu::MenuResult value = mainMenu.changeScreen(window, input);

	switch(value)
	{
	case MainMenu::EXIT:
		{
			gameState = Game::EXITING;
			break;
		}
	case MainMenu::PLAY:
		{
			gameState = Game::SHOWING_LEVEL_SELECT;
			break;
		}
	case MainMenu::HIGHSCORES:
		{
			////OPEN DAY//////
			//gameState = Game::SHOWOPTIONS;
			highScores.setCurrentScore(0);
			gameState = Game::GAME_OVER;
			/////////////////
			break;
		}
	case MainMenu::CREDITS:
		{
			gameState = Game::SHOWING_CREDITS;
			break;
		}
	default:
		break;
	}
}

void Game::handleInput()
{
	const std::vector<Entity*> players = *currentWorld.getPlayers();
	for(unsigned int i = 0; i < players.size(); i++)
	{
		input.handlePlayerMovement(*dynamic_cast<Player*>(players.at(i)));
		input.handlePlayerAttacks(window, *dynamic_cast<Player*>(players.at(i)));
	}
}

void Game::updateMouseCursor(sf::RenderWindow &window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	mouseCursor.setPosition(VectorFuncs::intToFloat(mousePos));
}
