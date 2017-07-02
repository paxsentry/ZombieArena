#include "bullet.h"

Bullet::Bullet()
{
	m_bulletShape.setSize(sf::Vector2f(2, 2));
}

void Bullet::stop() { m_isInFlight = false; }

bool Bullet::isInFlight() { return m_isInFlight; }

void Bullet::shoot(float startX, float startY, float xTarget, float yTarget)
{
	m_isInFlight = true;
	m_position.x = startX;
	m_position.y = startY;

	float gradient = (startX - xTarget) / (startY - yTarget);

	if (gradient < 0) { gradient *= -1; }

	float ratioXY = m_bulletSpeed / (1 + gradient);

	m_bulletDistanceY = ratioXY;
	m_bulletDistanceX = ratioXY * gradient;

	if (xTarget < startX) { m_bulletDistanceX *= -1; }
	if (yTarget < startY) { m_bulletDistanceY *= -1; }

	float range = 1000;
	m_minX = startX - range;
	m_maxX = startX + range;
	m_minY = startY - range;
	m_maxY = startY + range;

	m_bulletShape.setPosition(m_position);
}

sf::FloatRect Bullet::getPosition() { return m_bulletShape.getGlobalBounds(); }

sf::RectangleShape Bullet::getShape() { return m_bulletShape; }

void Bullet::update(float elapsedTime)
{
	m_position.x += m_bulletDistanceX * elapsedTime;
	m_position.y += m_bulletDistanceY * elapsedTime;

	m_bulletShape.setPosition(m_position);

	if (m_position.x < m_minX || m_position.x > m_maxX ||
		m_position.y < m_minY || m_position.y > m_maxY) {
		m_isInFlight = false;
	}
}