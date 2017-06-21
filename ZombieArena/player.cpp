#include "player.h"

Player::Player()
	:m_speed(START_SPEED),
	m_health(START_HEALTH),
	m_maxHealth(START_HEALTH)
{
	if (m_texture.loadFromFile("graphics/player.png")) {
		m_sprite.setTexture(m_texture);
		m_sprite.setOrigin(25, 25);
	}
}

void Player::spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize)
{
	m_position.x = arena.width / 2;
	m_position.y = arena.height / 2;

	m_arena.left = arena.left;
	m_arena.width = arena.width;
	m_arena.top = arena.top;
	m_arena.height = arena.height;

	m_tileSize = tileSize;

	m_resolution.x = resolution.x;
	m_resolution.y = resolution.y;
}

void Player::resetPlayerStats()
{
	m_speed = START_SPEED;
	m_health = m_maxHealth = START_HEALTH;
}

bool Player::hit(sf::Time timeHit)
{
	if (timeHit.asMicroseconds() - m_lastHit.asMilliseconds() > 200) {
		m_lastHit = timeHit;
		m_health -= 10;
		return true;
	}
	return false;
}

sf::Time Player::getLastHitTime() { return m_lastHit; }

sf::FloatRect Player::getPosition() { return m_sprite.getGlobalBounds(); }

sf::Vector2f Player::getCenter() { return m_position; }

float Player::getRotation() { return m_sprite.getRotation(); }

sf::Sprite Player::getSprite() { return m_sprite; }

int Player::getHealth() { return m_health; }

void Player::moveLeft() { m_leftPressed = true; }

void Player::moveRight() { m_rightPressed = true; }

void Player::moveUp() { m_upPressed = true; }

void Player::moveDown() { m_downPressed = true; }

void Player::stopLeft() { m_leftPressed = false; }

void Player::stopRight() { m_rightPressed = false; }

void Player::stopUp() { m_upPressed = false; }

void Player::stopDown() { m_downPressed = false; }

void Player::update(float elapsedTime, sf::Vector2i mousePosition)
{
	if (m_upPressed) { m_position.y -= m_speed * elapsedTime; }
	if (m_downPressed) { m_position.y += m_speed * elapsedTime; }
	if (m_rightPressed) { m_position.x += m_speed * elapsedTime; }
	if (m_leftPressed) { m_position.x -= m_speed * elapsedTime; }

	m_sprite.setPosition(m_position);

	if (m_position.x > m_arena.width - m_tileSize) { m_position.x = m_arena.width - m_tileSize; }
	if (m_position.x < m_arena.left + m_tileSize) { m_position.x = m_arena.left + m_tileSize; }
	if (m_position.y > m_arena.height - m_tileSize) { m_position.y = m_arena.height - m_tileSize; }
	if (m_position.y < m_arena.top + m_tileSize) { m_position.y = m_arena.top + m_tileSize; }

	float angle = (atan2(mousePosition.y - m_resolution.y / 2, mousePosition.x - m_resolution.x / 2) * 180) / 3.1415;
	m_sprite.setRotation(angle);
}

void Player::upgradeSpeed() { m_speed += (START_SPEED * .2); }

void Player::upgradeHealth() { m_maxHealth += (START_HEALTH * .2); }

void Player::increaseHealthLevel(int amount) {
	m_health += amount;

	if (m_health > m_maxHealth) { m_health = m_maxHealth; }
}