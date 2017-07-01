#ifndef ZOMBIE_ARENA_ZOMBIE_H
#define ZOMBIE_ARENA_ZOMBIE_H

#include <SFML\Graphics.hpp>

class Zombie
{
public:
	bool hit();
	bool isAlive();

	void spawn(float startX, float startY, int type, int seed);

	sf::FloatRect getPosition();
	sf::Sprite getSprite();

	void update(float elapsedTime, sf::Vector2f playerLocation);

private:
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 5;
	const float CRAWLER_HEALTH = 5;

	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	sf::Vector2f m_position;

	sf::Sprite m_sprite;

	float m_speed;
	float m_health;

	bool m_isAlive;
};

#endif // !ZOMBIE_ARENA_ZOMBIE_H