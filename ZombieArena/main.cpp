#include <SFML/Graphics.hpp>
#include "player.h"
#include "arena.h"
#include "TextureHolder.h"
#include "bullet.h"

int main()
{
	TextureHolder holder;

	enum class State {
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};

	State currentState = State::GAME_OVER;

	sf::Vector2f resolution;
	resolution.x = sf::VideoMode::getDesktopMode().width;
	resolution.y = sf::VideoMode::getDesktopMode().height;

	sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Arena", sf::Style::Fullscreen);

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
				arena.width = 1000;
				arena.height = 750;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);

				player.spawn(arena, resolution, tileSize);

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
		}

		if (currentState == State::LEVELING_UP) {}
		if (currentState == State::PAUSED) {}
		if (currentState == State::GAME_OVER) {}

		window.display();
	}

	delete[] zombies;

	return 0;
}