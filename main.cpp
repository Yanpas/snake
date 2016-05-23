#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Field.hpp"

using namespace std;
int main(int argc, char *argv[])
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(600, 400), "Snake",
			sf::Style::Close, settings);
	sf::RectangleShape rectangle(sf::Vector2f(120, 50));
	Field field(window);
	window.setFramerateLimit(60);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			field.handleEvent(event);
		}
		field.play();
		window.clear();
		field.refresh();
		window.display();
	}
	std::vector<int>().push_back(1);
	return 0;
}

