#ifndef ZOMBIE_ARENA_ARENA_H
#define ZOMBIE_ARENA_ARENA_H

#include "zombie.h"

int createBackground(sf::VertexArray& rVA, sf::IntRect arena);

Zombie* createHorde(int numZombies, sf::IntRect arena);

#endif ZOMBIE_ARENA_ARENA_H