#pragma once
#include "stdafx.h"

enum MovementStates {
  IDLE,
  MOVING,
  MOVING_LEFT,
  MOVING_RIGHT,
  MOVING_UP,
  MOVING_DOWN
};

class MovementComponent
{
private:
  sf::Sprite& sprite;

  float maxVelocity;
  float acceleration;
  float deceleration;

  sf::Vector2f velocity;

public:
  MovementComponent(sf::Sprite& sprite,
    float maxVelocity, float acceleration, float deceleration);
  virtual ~MovementComponent();

  void setMaxVelocity(float newVelocity);

  const float& getMaxVelocity() const;
  const sf::Vector2f& getVelocity() const;

  const bool getState(const short unsigned state) const;
  void stopVelocity();
  void stopVelocityX();
  void stopVelocityY();

  void move(const float dirX, const float dirY, const float& dt);
  void update(const float& dt);
};

