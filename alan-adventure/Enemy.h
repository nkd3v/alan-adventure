#pragma once
#include <SFML/Graphics.hpp>
#include "Collider.h"
#include "Animation.h"

class Enemy
{
public:
  Enemy(
    sf::Texture* texture,
    sf::Vector2u imageCount,
    float switchTime,
    float speed,
    sf::Vector2f size,
    sf::Vector2f position
  );
  ~Enemy();

  void Draw(sf::RenderWindow &window);
  Collider GetCollider() { return Collider(body); }
  

private:
  sf::RectangleShape body;
  Animation anim;
};