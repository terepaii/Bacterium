#ifndef LEVELSELECTMENU_H_
#define LEVELSELECTMENU_H_

#include "Menu.h"

class LevelSelectMenu: public Menu
{
public:

	LevelSelectMenu();
	~LevelSelectMenu();

	virtual void Load();
	virtual void Draw(sf::RenderWindow& window);
	virtual void Update(sf::RenderWindow& window, InputHandler &input);
	virtual void initMap(sf::RenderWindow& window, TextureManager &textureManager);

	bool isInRect(sf::Vector2i, sf::RectangleShape);
	MenuResult changeScreen(sf::RenderWindow& window, InputHandler &input);

private:
	sf::Sprite backgroundSprite;
	MenuResult menuValue;
	std::map<MenuResult,sf::RectangleShape>::iterator it;
};
#endif