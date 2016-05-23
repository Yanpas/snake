/*
 * Field.cpp
 *
 *  Created on: 22.05.2016
 *      Author: yanpas
 */

#include "Field.hpp"

Field::Field(sf::RenderWindow& window) :
		apple(Apple(window.getSize().x, window.getSize().y)), window(window)
{
	placeSnake();
}

Field::~Field()
{

}

void Field::handleEvent(sf::Event& e)
{
	if (e.type == sf::Event::KeyPressed)
	{
		sf::Event::KeyEvent k = e.key;
		switch (k.code)
		{
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
				dir = dir == Dir::Down ? Dir::Down : Dir::Up;
				break;
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
				dir = dir == Dir::Right ? Dir::Right : Dir::Left;
				break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				dir = dir == Dir::Up ? Dir::Up : Dir::Down;
				break;
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
				dir = dir == Dir::Left ? Dir::Left : Dir::Right;
				break;
		}
	}
}

void Field::play()
{
	sf::Time t = clock.getElapsedTime();
	sf::Time ft = feedclock.getElapsedTime();
	if (t >= sf::milliseconds(140))
	{
		try {
			moveSnake();
		} catch (std::string& s)
		{
			sf::sleep(sf::seconds(2));
			placeSnake();
		}
		clock.restart();
	}
	if(apple.visible)
	{
		if(ft >= sf::seconds(5))
		{
			apple.visible = false;
			feedclock.restart();
		}
	}
	else
	{
		if(ft >= sf::seconds(2))
		{
			apple.visible = true;
			apple.place(this);
			feedclock.restart();
		}
	}
}

void Field::moveSnake()
{
	switch (snake.back().turns.front())
	{
		case Dir::Up:
			if(dir==Dir::Down) dir=snake.back().turns.front();
			break;
		case Dir::Down:
			if(dir==Dir::Up) dir=snake.back().turns.front();
			break;
		case Dir::Left:
			if(dir==Dir::Right) dir=snake.back().turns.front();
			break;
		case Dir::Right:
			if(dir==Dir::Left) dir=snake.back().turns.front();
			break;
	}
	std::set<std::pair<int,int>> posset;
	Cell newcell;

	for (auto& cell : snake)
	{
		cell.turns.push_back(dir);
		sf::Vector2f newpos = cell.rect.getPosition();
		switch (cell.turns.front())
		{
			case Dir::Up:
				newpos.y -= delta;
				break;
			case Dir::Down:
				newpos.y += delta;
				break;
			case Dir::Left:
				newpos.x -= delta;
				break;
			case Dir::Right:
				newpos.x += delta;
				break;
		}

		if (newpos.x >= window.getSize().x)
			newpos.x = 0;
		else if (newpos.x < 0)
			newpos.x = window.getSize().x;
		if (newpos.y >= window.getSize().y)
			newpos.y = 0;
		else if (newpos.y < 0)
			newpos.y = window.getSize().y;

		cell.turns.pop_front();
		if(posset.count(std::make_pair(newpos.x, newpos.y))) throw std::string("Byte");
		else posset.insert(std::make_pair(newpos.x, newpos.y));
		cell.rect.setPosition(newpos);

		if(&cell == &snake.back()
				and round(newpos.x) == round(apple.rect.getPosition().x)
				and round(newpos.y) == round(apple.rect.getPosition().y)
				and snake.size()<100
				and apple.visible)
		{
			newcell = snake.front();
			auto newpos2 = newcell.rect.getPosition();
			switch(newcell.turns.front())
			{
				case Dir::Up:
					newpos2.y += delta;
					break;
				case Dir::Down:
					newpos2.y -= delta;
					break;
				case Dir::Left:
					newpos2.x += delta;
					break;
				case Dir::Right:
					newpos2.x -= delta;
					break;
			}
			newcell.rect.setPosition(newpos2);
			Dir d = newcell.turns.front();
			newcell.turns.push_front(d);
			apple.visible=false;
		}
		if (!newcell.turns.empty()) snake.push_front(newcell);
	}
	/*std::cerr << "Head x: " << snake.back().rect.getPosition().x << "\ty: "
			<< snake.back().rect.getPosition().y << std::endl ;*/
}

void Field::refresh()
{
	for (auto& cell : snake)
		window.draw(cell.rect);
	if(apple.visible) window.draw(apple.rect);
}

Field::Cell::Cell() :
		rect(sf::RectangleShape(sf::Vector2f(delta, delta)))
{
	sf::Color col = sf::Color::Green;
	col.a=192;
	rect.setFillColor(col);
}

Field::Cell::~Cell()
{
}

Field::Apple::Apple(uint x, uint y): rect(sf::RectangleShape(sf::Vector2f(delta, delta))),
		f(f),
		distx(std::uniform_int_distribution<int>(1,x/delta-1)),
		disty(std::uniform_int_distribution<int>(1,y/delta-1))
{
	rect.setFillColor(sf::Color::Red);
	eng.seed(time(nullptr));
}

Field::Apple::~Apple()
{
}

void Field::Apple::place(Field* f)
{
	int newx = distx(eng) * delta;
	int newy = disty(eng) * delta;
	for (auto& c : f->snake)
		if (newx == static_cast<int>(c.rect.getPosition().x)
				and newy == static_cast<int>(c.rect.getPosition().y))
		{
			place(f);
			return;
		}
	rect.setPosition(sf::Vector2f(newx,newy));
}

void Field::placeSnake(int length)
{
	dir = Dir::Right;
	snake = std::deque<Cell>(length);
	sf::Vector2u field = window.getSize();
	int xmid = field.x / 2 - delta * length / 2;
	int ymid = field.y / 2;
	int i = length;
	for (Cell& c : snake)
	{
		for (int j = 0; j < i; j++)
			c.turns.push_back(dir);
		c.rect.setPosition(xmid, ymid);
		xmid += delta;
		i--;
	}
	snake.back().rect.setFillColor(sf::Color::Green);
}
