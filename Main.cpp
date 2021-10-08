#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Player.h"
#include "PlayerUI.h"
#include "Platform.h"
#include <iostream>

static const float SCREEN_HEIGHT = 800.0f;
static const float SCREEN_WIDTH = 800.0f;

void ResizeView(const sf::RenderWindow &window, sf::View &view)
{
  float aspectRatio = float(window.getSize().x / float(window.getSize().y));
  view.setSize(SCREEN_HEIGHT * aspectRatio, SCREEN_HEIGHT);  
}

int main()
{
  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML works!");
  sf::View view(sf::Vector2f(765.0f, 408.0f), sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
  window.setView(view);

  sf::Texture playerTexture;
  playerTexture.loadFromFile("./assets/Character/Character.png");

  sf::Texture heartTexture;
  heartTexture.loadFromFile("./assets/UI/Heart.png");

  sf::Sprite heart(heartTexture);
  heart.setScale(2.0f, 2.0f);

  Player player(&playerTexture, sf::Vector2u(4, 4), 0.2f, 200.0f);
  player.SetPosition(sf::Vector2f(765.0f, 408.0f));

  auto platformPos = window.mapPixelToCoords(sf::Vector2i(291, 40));
  std::cout << platformPos.x << ' ' << platformPos.y << '\n';
  Platform platform1(nullptr, sf::Vector2f(8.0f, 16.0f), platformPos);

  sf::Texture mapTexture;
  mapTexture.loadFromFile("./assets/Maps/01.png");

  sf::Sprite map(mapTexture);
  map.setScale(sf::Vector2f(0.8f, 0.8f));

  float deltaTime = 0.0f;
  sf::Clock clock;

  PlayerUI playerUI(heart);

  while (window.isOpen())
  {
    deltaTime = clock.restart().asSeconds();

    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::TextEntered:
        putchar(event.text.unicode);
        break;

      case sf::Event::Resized:
        ResizeView(window, view);
        break;
      }
    }

    platform1.GetCollider().CheckCollision(player.GetCollider(), sf::Vector2f(0.0f, 0.0f), 1.0f);
    // player.GetCollider().CheckCollision(platform1.GetCollider(), sf::Vector2f(0.0f, 0.0f), 1.0f);

    player.Update(deltaTime);
    // view.setCenter(player.GetPosition());
    // sf::Vector2f mousePos(sf::Mouse::getPosition(window));
    // player.setPosition(mousePos);
    window.clear(sf::Color::Magenta);
    window.draw(map);
    platform1.Draw(window);
    player.Draw(window);
    playerUI.SetHeart(5, window);
    window.display();
    //std::cout << player.GetPosition().x << ' ' << player.GetPosition().y << '\n';
    std::cout << sf::Mouse::getPosition(window).x << ' ' << sf::Mouse::getPosition(window).y << '\n';
  }

  return 0;
}