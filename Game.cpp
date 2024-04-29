#include "Game.h"
#include"Globals.h"
#include"Bird.h"
#include<SFML/Audio.hpp>
#include<sstream>
Game::Game(sf::RenderWindow& window): win(window), is_entered_pressed(false), run_game(true), pipe_counter(71), pipe_spawn_time(70), score(0), start_monitoring(false)
{
	win.setFramerateLimit(60);
	bg_texture.loadFromFile("assets/background.png");
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setScale(SCALE_FACTOR, SCALE_FACTOR);
	bg_sprite.setPosition(0.f, -250.f);
	
	/*ground_texture.loadFromFile("assets/ground.png");
	ground_sprite1.setTexture(ground_texture);
	ground_sprite2.setTexture(ground_texture);

	ground_sprite1.setScale(SCALE_FACTOR, SCALE_FACTOR);
	ground_sprite2.setScale(SCALE_FACTOR, SCALE_FACTOR);

	ground_sprite1.setPosition(0, 480);
	ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().width, 480);
	*/
	font.loadFromFile("assets/QuickSand.ttf");
	restart_text.setFont(font);
	restart_text.setCharacterSize(30);
	restart_text.setFillColor(sf::Color::Black);
	restart_text.setPosition(150, 350);
	restart_text.setString("Press to restart");

	score_text.setFont(font);
	score_text.setCharacterSize(50);
	score_text.setFillColor(sf::Color::Black);
	score_text.setPosition(15, 15);
	score_text.setString("Score = 0");

	Pipe::loadTextures();
}

void Game::startGameLoop()
{
	sf::Clock clock;
	//Game loop
	while (win.isOpen())
	{
		//Event loop
		sf::Time dt = clock.restart();
		sf::Event event;
		while (win.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				win.close();
			}
			if (event.type == sf::Event::KeyPressed && run_game)
			{
				if (event.key.code == sf::Keyboard::Enter && !is_entered_pressed) {
					is_entered_pressed = true;
					bird.shouldFly(true);
				}
				if (event.key.code == sf::Keyboard::Space && is_entered_pressed) {
					bird.planeStart(dt);
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !run_game)
			{
				if (restart_text.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					restartGame();
				}
			}
		}

		doProcessing(dt);
		
		draw();
		//Display window
		win.display();

	}
}

void Game::draw()
{
	win.draw(bg_sprite);
	for (Pipe& pipe : pipes)
	{
		win.draw(pipe.sprite_up);
		win.draw(pipe.sprite_down);

	}
	//win.draw(ground_sprite1);
	//win.draw(ground_sprite2);
	win.draw(bird.plane_sprite);
	win.draw(score_text);
	if (!run_game)
	{
		win.draw(restart_text);
	}
}

/*void Game::moveGround(sf::Time& dt)
{
	ground_sprite1.move(-move_speed * dt.asSeconds(), 0.f);
	ground_sprite2.move(-move_speed * dt.asSeconds(), 0.f);

	if(ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width < 0)
	{
		ground_sprite1.setPosition(ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width, 480);
	}

	if (ground_sprite2.getGlobalBounds().left + ground_sprite2.getGlobalBounds().width < 0)
	{
		ground_sprite2.setPosition(ground_sprite1.getGlobalBounds().left + ground_sprite1.getGlobalBounds().width, 480);
	}
}*/

void Game::doProcessing(sf::Time& dt)
{
	if(is_entered_pressed)
	{
	/*if (is_entered_pressed)
	{
		moveGround(dt);
	*/
	bird.update(dt);
	if (pipe_counter > pipe_spawn_time)
	{
		pipes.push_back(Pipe(dist(rd)));
		pipe_counter = 0;
	}
	pipe_counter++;

	for (int i = 0; i < pipes.size(); i++)
	{
		pipes[i].update(dt);
		if (pipes[i].getRightBound() < 0)
		{
			pipes.erase(pipes.begin() + i);
		}
	}
	checkCollision();
	checkScore();
	
	}
}

void Game::checkCollision()
{
	if (pipes.size() > 0)
	{
		if (pipes[0].sprite_down.getGlobalBounds().intersects(bird.plane_sprite.getGlobalBounds()) || pipes[0].sprite_up.getGlobalBounds().intersects(bird.plane_sprite.getGlobalBounds()) || bird.plane_sprite.getGlobalBounds().top >= 700)
		{
			is_entered_pressed = false;
			run_game = false;
			bird.plane_sprite.setTexture(bird.crash_texture);
			bird.plane_sprite.setScale(1.5f, 1.5f);

			sb.loadFromFile("assets/audio.mp3");

			s.setBuffer(sb);
			s.play();
		}
	}
}

void Game::restartGame()
{
	bird.plane_sprite.setTexture(bird.plane_texture);
	bird.plane_sprite.setScale(1.f, 1.f);
	bird.resetPosition();
	bird.shouldFly(false);
	run_game = true;
	is_entered_pressed = false;
	pipe_counter = 71;
	pipes.clear();
	score = 0;
	score_text.setString("Score: 0");
}

void Game::checkScore()
{
	if (pipes.size() > 0)
	{
		if (!start_monitoring)
		{
			if (bird.plane_sprite.getGlobalBounds().left > pipes[0].sprite_down.getGlobalBounds().left && bird.getRightBound() < pipes[0].getRightBound())
			{
				start_monitoring = true;
			}
		}
		else
		{
			if (bird.plane_sprite.getGlobalBounds().left > pipes[0].getRightBound())
			{
				score++;
				score_text.setString("Score: " + toString(score));
				start_monitoring = false;
			}
		}
	}
}

std::string Game::toString(int num)
{
	std::stringstream ss;
	ss << num;
	return ss.str();
}
