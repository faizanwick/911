#pragma once
#include<SFML/Graphics.hpp>
class Bird
{
private:
	const int gravity = 10;
	float velocity_y;
	bool should_fly;
	const int fly_speed;
public:
	sf::Sprite plane_sprite;
	sf::Texture plane_texture, crash_texture;
	Bird();
	void shouldFly(bool);
	void resetPosition();
	void update(sf::Time&);
	float getRightBound();
	void planeStart(sf::Time&);

};