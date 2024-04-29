#include "Bird.h"
#include"Globals.h"
Bird::Bird() :
	gravity(10),
	should_fly(false),
	fly_speed(250)
{
	plane_texture.loadFromFile("assets/newPlane.png");
	crash_texture.loadFromFile("assets/crash.png");

	plane_sprite.setTexture(plane_texture);
	plane_sprite.setScale(1.f, 1.f);
	resetPosition();
}
void Bird::shouldFly(bool should_fly)
{
	this->should_fly = should_fly;
}

void Bird::resetPosition()
{
	plane_sprite.setPosition(100, 100);
	velocity_y = 0;
}

void Bird::update(sf::Time& dt)
{
	if (plane_sprite.getGlobalBounds().top < 700 && should_fly) {
		velocity_y += gravity * dt.asSeconds();
		plane_sprite.move(0, velocity_y);

		if (plane_sprite.getGlobalBounds().top < 0) {
			plane_sprite.setPosition(100, 0);
		}
	}
}

float Bird::getRightBound()
{
	return plane_sprite.getGlobalBounds().left + plane_sprite.getGlobalBounds().width;
}

void Bird::planeStart(sf::Time&dt)
{
	velocity_y = -fly_speed * dt.asSeconds();
}