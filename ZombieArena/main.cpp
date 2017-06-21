#include <SFML/Graphics.hpp>
#include "player.h"

int main()
{
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

				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close(); }

			if (currentState == State::PLAYING) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { player.moveUp(); }
				else { player.stopUp(); }

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { player.moveDown(); }
				else { player.stopDown(); }

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { player.moveLeft(); }
				else { player.stopLeft(); }

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { player.moveRight(); }
				else { player.stopRight(); }
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

					int tileSize = 50;

					player.spawn(arena, resolution, tileSize);

					clock.restart();
				}
			}
		}

		if (currentState == State::PLAYING) {
			sf::Time delta = clock.restart();

			gameTimeTotal += delta;

			float deltaAsSeconds = delta.asSeconds();

			mouseScreenPosition = sf::Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);
			player.update(deltaAsSeconds, sf::Mouse::getPosition());

			sf::Vector2f playerPosition(player.getCenter());

			mainView.setCenter(player.getCenter());
		}

		if (currentState == State::PLAYING) {
			window.clear();

			window.setView(mainView);

			window.draw(player.getSprite());
		}

		if (currentState == State::LEVELING_UP) {}
		if (currentState == State::PAUSED) {}
		if (currentState == State::GAME_OVER) {}

		window.display();

	}

	return 0;
}