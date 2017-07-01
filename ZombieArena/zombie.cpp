#include "zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

bool Zombie::hit()
{
	m_health--;

	if (m_health < 0) {
		m_isAlive = false;
		m_sprite.setTexture(TextureHolder::getTexture("graphics/blood.png"));

		return true;
	}

	return false;
}

bool Zombie::isAlive() { return m_isAlive; }

void Zombie::spawn(float startX, float startY, int type, int seed)
{
	switch (type)
	{
	case 0: // Bloater
		m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/bloater.png"));
		m_speed = 40;
		m_health = 5;
		break;
	case 1: // Chaser
		m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/chaser.png"));
		m_speed = 70;
		m_health = 1;
		break;
	case 2: // Crawler
		m_sprite = sf::Sprite(TextureHolder::getTexture("graphics/crawler.png"));
		m_speed = 20;
		m_health = 3;
		break;
	default:
		break;
	}

	srand((int)time(0) * seed);
	float modifier = (rand() % MAX_VARIANCE) + OFFSET;

	modifier /= 100;
	m_speed *= modifier;

	m_position.x = startX;
	m_position.y = startY;

	m_sprite.setOrigin(25, 25);
	m_sprite.setPosition(m_position);
}

sf::FloatRect Zombie::getPosition() { return m_sprite.getGlobalBounds(); }

sf::Sprite Zombie::getSprite() { return m_sprite; }

void Zombie::update(float elapsedTime, sf::Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	if (playerX > m_position.x) {
		m_position.x = m_position.x + m_speed * elapsedTime;
	}

	if (playerY > m_position.y) {
		m_position.y = m_position.y + m_speed * elapsedTime;
	}

	if (playerX < m_position.x) {
		m_position.x = m_position.x - m_speed * elapsedTime;
	}

	if (playerY < m_position.y) {
		m_position.y = m_position.y - m_speed * elapsedTime;
	}

	m_sprite.setPosition(m_position);

	float angle = (atan2(playerY - m_position.y, playerX - m_position.x) * 180) / 3.1415;

	m_sprite.setRotation(angle);
}