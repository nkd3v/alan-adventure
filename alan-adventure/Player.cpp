#include "stdafx.h"

#include "Player.h"

void Player::initVariables()
{
	this->damageTimerMax = 500;

	weapon = new Bow(*this);
}

void Player::initAnimations()
{
	this->animationComponent->addAnimation("IDLE",       15.f, 0, 0, 3, 0, 32, 64);
	this->animationComponent->addAnimation("WALK_DOWN",  11.f, 0, 1, 3, 1, 32, 64);
	this->animationComponent->addAnimation("WALK_LEFT",  11.f, 0, 1, 3, 1, 32, 64);
	this->animationComponent->addAnimation("WALK_RIGHT", 11.f, 0, 2, 3, 2, 32, 64);
	this->animationComponent->addAnimation("WALK_UP",    11.f, 0, 2, 3, 2, 32, 64);
}

Player::Player(float x, float y, sf::Texture& texture_sheet)
{
	initVariables();

	createHitboxComponent(this->sprite, 0.f, 32.f, 32.f, 32.f);
	createMovementComponent(140.f, 1400.f, 1000.f);
	createAnimationComponent(texture_sheet);
	createAttributeComponent(8, 1, 2, 1, 1);

	setPosition(x, y);
	initAnimations();
}

Player::~Player()
{
}

AttributeComponent* Player::getAttributeComponent()
{
	return this->attributeComponent;
}

Bow* Player::getWeapon()
{
	return weapon;
}

const bool Player::getDamageTimer()
{
	if (this->damageTimer.getElapsedTime().asMilliseconds() >= this->damageTimerMax)
	{
		this->damageTimer.restart();
		return true;
	}

	return false;
}

void Player::attack(sf::Vector2f mousePos)
{
	weapon->shoot(getCenter(), atan2f(mousePos.y - getCenter().y, mousePos.x - getCenter().x));
}

void Player::loseHP(const int hp)
{
	this->attributeComponent->loseHP(hp);
}

void Player::gainHP(const int hp)
{
	this->attributeComponent->gainHP(hp);
}

void Player::updateAnimation(const float& dt)
{
	if (this->movementComponent->getState(IDLE))
	{
		this->animationComponent->play("IDLE", dt);
	}
	else if (this->movementComponent->getState(MOVING_LEFT))
	{
		this->animationComponent->play("WALK_LEFT", dt);
	}
	else if (this->movementComponent->getState(MOVING_RIGHT))
	{
		this->animationComponent->play("WALK_RIGHT", dt);
	}
	else if (this->movementComponent->getState(MOVING_UP))
	{
		this->animationComponent->play("WALK_UP", dt);
	}
	else if (this->movementComponent->getState(MOVING_DOWN))
	{
		this->animationComponent->play("WALK_DOWN", dt);
	}
}

void Player::update(const float& dt)
{
	this->movementComponent->update(dt);

	this->updateAnimation(dt);

	this->hitboxComponent->update();

	weapon->update(dt);
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);

	weapon->render(target);
}
