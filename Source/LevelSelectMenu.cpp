#include "LevelSelectMenu.h"

LevelSelectMenu::LevelSelectMenu()
{

}

LevelSelectMenu::~LevelSelectMenu()
{

}

void LevelSelectMenu::initMap(sf::RenderWindow &window, TextureManager &textureManager)
{
	//hardcoded positions
	
	backgroundSprite.setTexture(*textureManager.getResource("Config/Content/Images/Menus/Backgrounds/levelSelectBackground.png"));
	backgroundSprite.setScale((float)window.getSize().x / backgroundSprite.getTexture()->getSize().x, (float)window.getSize().y / backgroundSprite.getTexture()->getSize().y);

	sf::RectangleShape level1Button;
	level1Button.setSize(sf::Vector2f(206.0f, 72.0f));
	level1Button.setTexture(textureManager.getResource("Config/Content/Images/Menus/Buttons/level1Button.png"));
	level1Button.setPosition(window.getSize().x / 2 - level1Button.getSize().x / 2, 80);

	sf::RectangleShape level2Button;
	level2Button.setSize(sf::Vector2f(210.0f, 72.0f));
	level2Button.setTexture(textureManager.getResource("Config/Content/Images/Menus/Buttons/level2Button.png"));
	level2Button.setPosition(window.getSize().x / 2 - level2Button.getSize().x / 2, 165);

	sf::RectangleShape level3Button;
	level3Button.setSize(sf::Vector2f(210.0f,72.0f));
	level3Button.setTexture(textureManager.getResource("Config/Content/Images/Menus/Buttons/level3Button.png"));
	level3Button.setPosition(window.getSize().x / 2 - level3Button.getSize().x / 2, 250);

	sf::RectangleShape survivalModeButton;
	survivalModeButton.setSize(sf::Vector2f(420.0f,72.0f));
	survivalModeButton.setTexture(textureManager.getResource("Config/Content/Images/Menus/Buttons/survivalModeButton.png"));
	survivalModeButton.setPosition(window.getSize().x / 2 - survivalModeButton.getSize().x / 2, 335);

	sf::RectangleShape backButton;
	backButton.setSize(sf::Vector2f(140.0f,72.0f));
	backButton.setTexture(textureManager.getResource("Config/Content/Images/Menus/Buttons/backButton.png"));
	backButton.setPosition(window.getSize().x / 2 - backButton.getSize().x / 2, 420);


	menuItems[LEVEL1] = level1Button;
	menuItems[LEVEL2] = level2Button;
	menuItems[LEVEL3] = level3Button;
	menuItems[SURVIVAL] = survivalModeButton;
	menuItems[BACK] = backButton;
}

void LevelSelectMenu::Load()
{
	
}

void LevelSelectMenu::Update(sf::RenderWindow& window, InputHandler &input)
{
	
		for(it = menuItems.begin(); it != menuItems.end(); it++)
		{
			if(isInRect(input.getMousePos(window),it->second) &&  input.clicked(window))
			{
				menuValue = it->first;
				break;
			}
		}

}

void LevelSelectMenu::Draw(sf::RenderWindow& window)
{
	window.draw(backgroundSprite);
	for(it = menuItems.begin(); it != menuItems.end(); it++)
	{
		window.draw(it->second);
	}
}

bool LevelSelectMenu::isInRect(sf::Vector2i position, sf::RectangleShape menuItemRect)
{
	return(menuItemRect.getPosition().y < position.y && 
			menuItemRect.getPosition().y + menuItemRect.getSize().y > position.y &&
			menuItemRect.getPosition().x < position.x &&
			menuItemRect.getPosition().x + menuItemRect.getSize().x > position.x);
}

Menu::MenuResult LevelSelectMenu::changeScreen(sf::RenderWindow& window, InputHandler &input)
{
	menuValue = NOTHING;
	Update(window, input);
	return menuValue;
}