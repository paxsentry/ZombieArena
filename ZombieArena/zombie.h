#ifndef ZOMBIE_ARENA_ZOMBIE_H
#define ZOMBIE_ARENA_ZOMBIE_H

#include <SFML\Graphics.hpp>

class Zombie
{
public:
	Zombie();
	~Zombie();

private:
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 5;
	const float CRAWLER_HEALTH = 5;

	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

};

#endif // !ZOMBIE_ARENA_ZOMBIE_H