#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Game.h"
#include "ScoreBoard.h"
#include <vector>
#include <memory>
#include <iostream>

static const unsigned int SCREEN_HEIGHT = 800;
static const unsigned int SCREEN_WIDTH = 800;

void ResizeView(const sf::RenderWindow &window, sf::View &view)
{
  float aspectRatio = float(window.getSize().x / float(window.getSize().y));
  view.setSize(SCREEN_HEIGHT * aspectRatio, SCREEN_HEIGHT);  
}

int main()
{
  ScoreBoard scoreBoard;
  int score = 0;

  std::string playerName;
  std::cout << "Enter your name: ";
  std::cin >> playerName;

  sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Alan's Adventure");
  window.setFramerateLimit(60);

  sf::Font font;
  font.loadFromFile("assets/Fonts/arial.ttf");

  sf::Text text;
  text.setPosition(sf::Vector2f(1100.0f, 20.0f));
  text.setFillColor(sf::Color::White);
  text.setOutlineThickness(2.0f);
  text.setOutlineColor(sf::Color::Black);
  text.setFont(font);
  text.setString("999");
  text.setCharacterSize(28);

  sf::View view(sf::Vector2f(765.0f, 408.0f), sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
  window.setView(view);

  sf::Texture playerTexture;
  playerTexture.loadFromFile("assets/Character/Character.png");

  Player player(&playerTexture, sf::Vector2u(4, 4), 0.2f, 100.0f);
  player.SetPosition(sf::Vector2f(765.0f, 408.0f));

  sf::Texture cherryTexture;
  cherryTexture.loadFromFile("assets/Items/cherry.png");
  std::vector<sf::Sprite*> cherrys;

  sf::Texture enemyTexture;
  enemyTexture.loadFromFile("assets/Enemy/spr_blob_big.png");
  std::vector<Enemy> enemies;
  for (int i = 0; i < 1; i++)
  {
    enemies.emplace_back(Enemy(&enemyTexture, sf::Vector2u(6, 4), 0.2f, 100.0f, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(rand() % 100 + 765.0f, rand() % 40 + 408.0f)));
    enemies.back().GetBody().setScale(0.6f, 0.6f);
    enemies.back().SetTarget(&player.body);
  }

  sf::Texture mapTexture;
  mapTexture.loadFromFile("assets/Maps/01.png");
  sf::Sprite map(mapTexture);
  map.setScale(sf::Vector2f(0.8f, 0.8f));

  float deltaTime = 0.0f;
  sf::Clock clock;

  while (window.isOpen())
  {
    deltaTime = clock.restart().asSeconds();

    auto t(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::Resized:
        ResizeView(window, view);
        break;
      }
    }

    for (size_t i = 0; i < enemies.size(); i++)
    {
      for (size_t j = 0; j < enemies.size(); j++)
      {
        if (i == j) continue;
        if (enemies.at(i).GetCollider().CheckCollision(enemies.at(j).GetCollider(), {}, 0)) {}
      }
    }

    for (size_t i = 0; i < enemies.size(); i++)
    {
      if (player.GetCollider().CheckCollision(enemies.at(i).GetCollider(), {}, 0))
      {
        player.TakeDamage(1);
        sf::Vector2f delta = player.body.getPosition() - enemies.at(i).GetBody().getPosition();
        
        if (abs(delta.x) < abs(delta.y))
        {
          if (delta.x > 0)
          {
            player.body.move(25, 0);
            enemies.at(i).GetBody().move(-25, 0);
          }
          else
          {
            player.body.move(-25, 0);
            enemies.at(i).GetBody().move(25, 0);
          }
        }
        else
        {
          if (delta.y > 0)
          {
            player.body.move(0, 25);
            enemies.at(i).GetBody().move(0, -25);
          }
          else
          {
            player.body.move(0, -25);
            enemies.at(i).GetBody().move(0, 25);
          }
        }
      }
    }

    std::vector<Bullet*> &bullets = player.weapon.GetBullets();
    bool killedEnemy = false;
    for (size_t i = 0; i < bullets.size(); i++)
    {
      for (size_t j = 0; j < enemies.size(); j++)
      {
        if (bullets.at(i)->GetBody().getGlobalBounds().intersects(enemies.at(j).GetBody().getGlobalBounds()))
        {
          delete bullets.at(i);

          cherrys.emplace_back(new sf::Sprite(cherryTexture));
          cherrys.back()->setPosition(enemies.at(j).GetPosition());
          cherrys.back()->setScale(3.0f, 3.0f);

          bullets.erase(bullets.begin() + i);
          enemies.erase(enemies.begin() + j);

          killedEnemy = true;
          // break is mandatory because of bullet deletion.
          break;
        }
      }
    }

    for (size_t i = 0; i < cherrys.size(); i++)
    {
      if (player.body.getGlobalBounds().intersects(cherrys.at(i)->getGlobalBounds()))
      {
        delete cherrys.at(i);
        cherrys.erase(cherrys.begin() + i);
        player.resetHealth();
        player.changeCooldown(-0.5f);
      }
    }

    if (killedEnemy)
    {
      size_t spawnCount = rand() % 2 + 1;
      for (size_t i = 0; i < spawnCount; i++)
      {
        enemies.emplace_back(Enemy(&enemyTexture, sf::Vector2u(6, 4), 0.2f, 100.0f, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(rand() % 100 + 477.0f, rand() % 40 + 1000.0f)));
        enemies.back().GetBody().setScale(0.6f, 0.6f);
        enemies.back().SetTarget(&player.body);
      }
      score++;
    }

    player.Update(deltaTime, window);
    window.clear();
    window.setView(view);
    window.draw(map);
    player.Draw(window);

    for (auto &enemy : enemies)
    {
      enemy.Update(deltaTime);
      enemy.Draw(window);
    }

    for (auto& cherry : cherrys)
    {
      window.draw(*cherry);
    }

    text.setString(std::to_string(score));
    window.draw(text);

    if (player.died) window.close();

    window.display();
  }

  scoreBoard.addScore(playerName, score);

  for (const auto& s : scoreBoard.getScores())
    std::cout << s.second << ' ' << s.first << '\n';

  return 0;
}