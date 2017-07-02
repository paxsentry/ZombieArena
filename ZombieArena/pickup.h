#ifndef ZOMBIE_ARENA_PICKUP_H
#define ZOMBIE_ARENA_PICKUP_H

#include "SFML\Graphics.hpp"

class Pickup
{
public:
   Pickup(int type);

   void setArena(sf::IntRect arena);

   void spawn();

   sf::FloatRect getPosition();

   sf::Sprite getSprite();

   void update(float elapsedTime);

   bool isSpawned();

   int gotIt();

   void upgrade();

private:
   const int HEALTH_START_VALUE = 50;
   const int AMMO_START_VALUE = 12;
   const int START_WAIT_TIME = 10;
   const int START_SECONDS_TO_LIVE = 5;

   sf::Sprite m_sprite;

   sf::IntRect m_arena;

   int m_value;

   int m_type; // 1- health, 2- ammo

   bool m_spawned;
   float m_secondsSinceSpawn;
   float m_secondsSinceDeSpawn;
   float m_secondsToLive;
   float m_secondsToWait;
};

#endif // !ZOMBIE_ARENA_PICKUP_H