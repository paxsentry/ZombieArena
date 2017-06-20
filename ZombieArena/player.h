#ifndef ZOMBIE_ARENA_PLAYER_H
#define ZOMBIE_ARENA_PLAYER_H

#include <SFML\Graphics.hpp>

class Player
{
public:
	Player();

	void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);

	void resetPlayerStats();

	bool hit(sf::Time timeHit);

	sf::Time getLastHitTime();
	sf::FloatRect getPosition();
	sf::Vector2f getCenter();
	float getRotation();
	sf::Sprite getSprite();
	int getHealth();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	void update(float elapsedTime, sf::Vector2i mousePosition);

	void upgradeSpeed();

	void upgradeHealth();

	void increaseHealthLevel(int amount);

private:
	const float START_SPEED = 200.0f;
	const float START_HEALTH = 100.0f;

	sf::Vector2f m_position;
	sf::Vector2f m_resolution;

	sf::Sprite m_sprite;
	sf::Texture m_texture;

	sf::IntRect m_arena;
	int m_tileSize;

	bool m_upPressed;
	bool m_downPressed;
	bool m_rightPressed;
	bool m_leftPressed;

	int m_health;
	int m_maxHealth;

	sf::Time m_lastHit;

	float m_speed;
};
#endif