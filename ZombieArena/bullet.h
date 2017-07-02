#ifndef ZOMBIE_ARENA_BULLET_H
#define ZOMBIE_ARENA_BULLET_H

#include "SFML\Graphics.hpp"

class Bullet
{
public:
	Bullet();

	void stop();

	bool isInFlight();

	void shoot(float startX, float startY, float xTarget, float yTarget);

	sf::FloatRect getPosition();

	sf::RectangleShape getShape();

	void update(float elapsedTime);

private:
	sf::Vector2f m_position;
	sf::RectangleShape m_bulletShape;

	bool m_isInFlight = false;

	float m_bulletSpeed = 1000;

	float m_bulletDistanceX;
	float m_bulletDistanceY;

	float m_maxX;
	float m_minX;
	float m_maxY;
	float m_minY;
};

#endif // !ZOMBIE_ARENA_BULLET_H