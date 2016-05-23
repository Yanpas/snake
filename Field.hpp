/*
 * Field.hpp
 *
 *  Created on: 22.05.2016
 *      Author: yanpas
 */

#ifndef FIELD_HPP_
#define FIELD_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <deque>
#include <iostream>
#include <set>
#include <random>
#include <ctime>

class Field
{
public:
	Field(sf::RenderWindow& window);
	/*Field(const Field& rhs);
	Field(Field&& rhs);
	Field& operator=(const Field& rhs);
	Field& operator=(Field&& rhs);*/
	virtual ~Field();
	void handleEvent(sf::Event& e);
	void refresh();
	void play();
	void moveSnake();
private:
	enum class Dir
	{
		Left, Right, Up, Down
	};

	class Cell
	{
	public:
		Cell();
		virtual ~Cell();
		sf::RectangleShape rect;
		std::deque<Dir> turns;
	};

	class Apple
	{
	public:
		Apple(uint x, uint y);
		virtual ~Apple();
		sf::RectangleShape rect;
		bool visible=false;
		void place(Field* f);
	private:
		Field* f;
		std::mt19937 eng;
		std::uniform_int_distribution<int> distx;
		std::uniform_int_distribution<int> disty;
	};

	void placeSnake(int length=4);

	Dir dir = Dir::Right;
	std::deque<Cell> snake;
	Apple apple;

	static const int delta = 20;
	sf::RenderWindow& window;
	sf::Clock clock;
	sf::Clock feedclock;
	friend void swap(Field& l, Field& r);
};


#endif /* FIELD_HPP_ */
