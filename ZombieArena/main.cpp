#include <SFML/Graphics.hpp>
#include <sstream> 
#include "player.h"
#include "arena.h"
#include "TextureHolder.h"
#include "bullet.h"
#include "pickup.h"

int main()
{
   TextureHolder holder;

   enum class State {
      PAUSED, LEVELING_UP, GAME_OVER, PLAYING
   };

   State currentState = State::GAME_OVER;

   sf::Vector2f resolution;
   resolution.x = 1920;
   resolution.y = 1080;

   sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Arena");

   sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

   sf::Clock clock;

   sf::Time  gameTimeTotal;

   sf::Vector2f mouseWorldPosition;
   sf::Vector2i mouseScreenPosition;

   Player player;

   sf::IntRect arena;

   sf::VertexArray background;
   sf::Texture textureBackground = TextureHolder::getTexture("graphics/background_sheet.png");

   int numZombies;
   int numZombiesAlive;

   Zombie* zombies = nullptr;

   Bullet bullets[100];
   int currentBullet = 0;
   int bulletsSpare = 24;
   int bulletsInClip = 6;
   int clipSize = 6;
   float fireRate = 1;

   sf::Time lastPressed;

   window.setMouseCursorVisible(false);
   sf::Sprite spriteCrosshair;
   sf::Texture textureCrossHair = TextureHolder::getTexture("graphics/crosshair.png");
   spriteCrosshair.setTexture(textureCrossHair);
   spriteCrosshair.setOrigin(25, 25);

   Pickup healthPickup(1);
   Pickup ammoPickup(2);

   int score = 0;
   int highScore = 0;

   sf::Sprite spriteGameOver;
   sf::Texture textureGameOver = TextureHolder::getTexture("graphics/background.png");
   spriteGameOver.setTexture(textureGameOver);
   spriteGameOver.setPosition(0, 0);

   sf::View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

   sf::Sprite spriteAmmoIcon;
   sf::Texture textureAmmoIcon = TextureHolder::getTexture("graphics/ammo_icon.png");
   spriteAmmoIcon.setTexture(textureAmmoIcon);
   spriteAmmoIcon.setPosition(20, 980);

   sf::Font font;
   font.loadFromFile("fonts/zombiecontrol.ttf");

   sf::Text textPaused;
   textPaused.setFont(font);
   textPaused.setCharacterSize(85);
   textPaused.setFillColor(sf::Color::White);
   textPaused.setPosition(400, 400);
   textPaused.setString("Press Enter \nto continue");

   sf::Text textGameOver;
   textGameOver.setFont(font);
   textGameOver.setCharacterSize(80);
   textGameOver.setFillColor(sf::Color::White);
   textGameOver.setPosition(250, 850);
   textGameOver.setString("Press Enter to play");

   sf::Text textLevelUp;
   textLevelUp.setFont(font);
   textLevelUp.setCharacterSize(60);
   textLevelUp.setFillColor(sf::Color::White);
   textLevelUp.setPosition(150, 250);
   std::stringstream levelupStream;
   levelupStream <<
      "1- Increase rate of fire" <<
      "\n2- Increase clip size (next reload)" <<
      "\n3- Increase max health" <<
      "\n4- Increase run speed" <<
      "\n5- More and better health supplies" <<
      "\n6- More and better ammo supplies";
   textLevelUp.setString(levelupStream.str());

   sf::Text textAmmo;
   textAmmo.setFont(font);
   textAmmo.setCharacterSize(50);
   textAmmo.setFillColor(sf::Color::White);
   textAmmo.setPosition(200, 980);

   sf::Text textScore;
   textScore.setFont(font);
   textScore.setCharacterSize(50);
   textScore.setFillColor(sf::Color::White);
   textScore.setPosition(20, 0);

   sf::Text textHiScore;
   textScore.setFont(font);
   textScore.setCharacterSize(50);
   textScore.setFillColor(sf::Color::White);
   textScore.setPosition(1400, 0);
   std::stringstream hiScoreStream;
   hiScoreStream << "Hiscore:" << highScore;
   textHiScore.setString(hiScoreStream.str());

   sf::Text textZombiesRemaning;
   textZombiesRemaning.setFont(font);
   textZombiesRemaning.setCharacterSize(50);
   textZombiesRemaning.setFillColor(sf::Color::White);
   textZombiesRemaning.setPosition(1500, 980);
   textZombiesRemaning.setString("Zombies: 100");

   int wave = 0;
   sf::Text textWaveNumber;
   textWaveNumber.setFont(font);
   textWaveNumber.setCharacterSize(50);
   textWaveNumber.setFillColor(sf::Color::White);
   textWaveNumber.setPosition(1250, 980);
   textWaveNumber.setString("Wave: 0");

   sf::RectangleShape healthBar;
   healthBar.setFillColor(sf::Color::Red);
   healthBar.setPosition(450, 980);

   int frameSinceLastHudUpdate = 0;
   int fpsMeasurementFrameInterval = 1000;

   while (window.isOpen())
   {
      sf::Event event;

      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Return && currentState == State::PLAYING) {
               currentState = State::PAUSED;
            }
            else if (event.key.code == sf::Keyboard::Return && currentState == State::PAUSED) {
               currentState = State::PLAYING;
               clock.restart();
            }
            else if (event.key.code == sf::Keyboard::Return && currentState == State::GAME_OVER) {
               currentState = State::LEVELING_UP;
            }

            if (currentState == State::PLAYING) {
               if (event.key.code == sf::Keyboard::R) {
                  if (bulletsSpare >= clipSize) {
                     bulletsInClip = clipSize;
                     bulletsSpare -= clipSize;
                  }
                  else if (bulletsSpare > 0) {
                     bulletsInClip = bulletsSpare;
                     bulletsSpare = 0;
                  }
                  else {

                  }
               }
            }
         }
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close(); }

      if (currentState == State::PLAYING) { // Controls
         if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { player.moveUp(); }
         else { player.stopUp(); }

         if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { player.moveDown(); }
         else { player.stopDown(); }

         if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { player.moveLeft(); }
         else { player.stopLeft(); }

         if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { player.moveRight(); }
         else { player.stopRight(); }

         if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0) {
               bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);

               currentBullet++;

               if (currentBullet > 99) { currentBullet = 0; }
               lastPressed = gameTimeTotal;

               bulletsInClip--;
            }
         }
      }

      if (currentState == State::LEVELING_UP) {
         if (event.key.code == sf::Keyboard::Num1) { currentState = State::PLAYING; }
         if (event.key.code == sf::Keyboard::Num2) { currentState = State::PLAYING; }
         if (event.key.code == sf::Keyboard::Num3) { currentState = State::PLAYING; }
         if (event.key.code == sf::Keyboard::Num4) { currentState = State::PLAYING; }
         if (event.key.code == sf::Keyboard::Num5) { currentState = State::PLAYING; }
         if (event.key.code == sf::Keyboard::Num6) { currentState = State::PLAYING; }

         if (currentState == State::PLAYING) {
            arena.width = 500;
            arena.height = 500;
            arena.left = 0;
            arena.top = 0;

            int tileSize = createBackground(background, arena);

            player.spawn(arena, resolution, tileSize);

            healthPickup.setArena(arena);
            ammoPickup.setArena(arena);

            numZombies = 10;

            delete[] zombies;
            zombies = createHorde(numZombies, arena);
            numZombiesAlive = numZombies;

            clock.restart();
         }
      }

      if (currentState == State::PLAYING) { // Update the frame
         sf::Time delta = clock.restart();

         gameTimeTotal += delta;

         float deltaAsSeconds = delta.asSeconds();

         mouseScreenPosition = sf::Mouse::getPosition();
         mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);
         spriteCrosshair.setPosition(mouseWorldPosition);

         player.update(deltaAsSeconds, sf::Mouse::getPosition());

         sf::Vector2f playerPosition(player.getCenter());

         mainView.setCenter(player.getCenter());

         for (int i = 0; i < numZombies; i++)
         {
            if (zombies[i].isAlive()) {
               zombies[i].update(delta.asSeconds(), playerPosition);
            }
         }

         for (int i = 0; i < 100; i++)
         {
            if (bullets[i].isInFlight()) { bullets[i].update(deltaAsSeconds); }
         }

         healthPickup.update(deltaAsSeconds);
         ammoPickup.update(deltaAsSeconds);

         for (int i = 0; i < 100; i++) // Bullet and zombie collision detection
         {
            for (int j = 0; j < numZombies; j++)
            {
               if (bullets[i].isInFlight() && zombies[j].isAlive()) {
                  if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
                     bullets[i].stop();

                     if (zombies[j].hit()) {
                        score += 10;

                        if (score >= highScore) { highScore = score; }

                        numZombiesAlive--;

                        if (numZombiesAlive == 0) { currentState = State::LEVELING_UP; }
                     }
                  }
               }
            }
         }

         for (int i = 0; i < numZombies; i++) // Player and zombie collision detection
         {
            if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive()) {

               if (player.hit(gameTimeTotal)) {}

               if (player.getHealth() <= 0) { currentState = State::GAME_OVER; }
            }
         }

         // Player and pickup collision detection
         if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
            player.increaseHealthLevel(healthPickup.gotIt());
         }

         if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
            bulletsSpare += ammoPickup.gotIt();
         }

         healthBar.setSize(sf::Vector2f(player.getHealth() * 3, 70));

         frameSinceLastHudUpdate++;

         if (frameSinceLastHudUpdate > fpsMeasurementFrameInterval) {
            std::stringstream ssAmmo;
            std::stringstream ssScore;
            std::stringstream ssHiScore;
            std::stringstream ssWave;
            std::stringstream ssZombiesAlive;

            ssAmmo << bulletsInClip << "/" << bulletsSpare;
            textAmmo.setString(ssAmmo.str());

            ssScore << "Score:" << score;
            textScore.setString(ssScore.str());

            ssHiScore << "Hiscore:" << highScore;
            textHiScore.setString(ssHiScore.str());

            ssWave << "Wave:" << wave;
            textWaveNumber.setString(ssWave.str());

            ssZombiesAlive << "Zombies:" << numZombiesAlive;
            textZombiesRemaning.setString(ssZombiesAlive.str());

            frameSinceLastHudUpdate = 0;
         }
      }

      if (currentState == State::PLAYING) { // Drawing
         window.clear();

         window.setView(mainView);

         window.draw(background, &textureBackground);

         for (int i = 0; i < numZombies; i++)
         {
            window.draw(zombies[i].getSprite());
         }

         for (int i = 0; i < 100; i++)
         {
            if (bullets[i].isInFlight()) { window.draw(bullets[i].getShape()); }
         }

         window.draw(player.getSprite());

         if (ammoPickup.isSpawned()) { window.draw(ammoPickup.getSprite()); }
         if (healthPickup.isSpawned()) { window.draw(healthPickup.getSprite()); }

         window.draw(spriteCrosshair);

         window.setView(hudView);

         window.draw(spriteAmmoIcon);
         window.draw(textAmmo);
         window.draw(textScore);
         window.draw(textHiScore);
         window.draw(healthBar);
         window.draw(textWaveNumber);
         window.draw(textZombiesRemaning);
      }

      if (currentState == State::LEVELING_UP) {
         window.draw(spriteGameOver);
         window.draw(textLevelUp);
      }

      if (currentState == State::PAUSED) {
         window.draw(textPaused);
      }

      if (currentState == State::GAME_OVER) {
         window.draw(spriteGameOver);
         window.draw(textGameOver);
         window.draw(textScore);
         window.draw(textHiScore);
      }

      window.display();
   }

   delete[] zombies;

   return 0;
}