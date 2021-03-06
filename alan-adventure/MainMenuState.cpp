#include "stdafx.h"
#include "MainMenuState.h"

MainMenuState::MainMenuState(sf::RenderWindow* window, std::stack<std::unique_ptr<State>>& states)
  : State(window, states)
{
  if (!bgTexture.loadFromFile("assets/Background/main-menu-bg.png"))
    throw("Error: Could not load background");

  bgMusic.setBuffer(am.getSoundBuffer("TITLE_SCREEN"));
  bgMusic.play();

  bg.setTexture(bgTexture);

  titleText.setFont(am.getFont("DPCOMIC"));
  titleText.setFillColor(sf::Color::Black);
  titleText.setOutlineThickness(2.f);
  titleText.setOutlineColor(sf::Color::White);
  titleText.setCharacterSize(96);
  titleText.setPosition(170.f, 60.f);
  titleText.setString("NO WAY OUT");

  nameText.setFont(am.getFont("DPCOMIC"));
  nameText.setFillColor(sf::Color::Black);
  nameText.setOutlineThickness(2.f);
  nameText.setOutlineColor(sf::Color::White);
  nameText.setCharacterSize(36);
  nameText.setPosition(10.f, 750.f);
  nameText.setString("Narongpol Kijrangsan | 64011106");

  initMenu();
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::initMenu()
{
  selectedOption = 0;
  maxOptions = 3;

  for (auto& elm : menu)
  {
    elm.setFont(am.getFont("DPCOMIC"));
    elm.setCharacterSize(42);
  }

  menu[0].setPosition(350.f, 300.f);
  menu[1].setPosition(310.f, 375.f);
  menu[2].setPosition(360.f, 450.f);

  menu[0].setString("Start");
  menu[1].setString("Scoreboard");
  menu[2].setString("Quit");
}

void MainMenuState::updateMenu()
{
  for (unsigned i = 0; i < maxOptions; i++)
  {
    if (i == selectedOption)
      menu[i].setFillColor(sf::Color::Red);
    else
      menu[i].setFillColor(sf::Color::White);
  }
}

void MainMenuState::updateSFMLEvents(sf::RenderTarget* target)
{
  sf::Event e;

  while (window->pollEvent(e))
  {
    if (e.type == sf::Event::Closed)
    {
      window->close();
    }
    if (e.type == sf::Event::KeyPressed)
    {
      if (e.key.code == sf::Keyboard::W
          || e.key.code == sf::Keyboard::Up)
      {
        selectedOption = (selectedOption + 2) % maxOptions;
      }
      else if (e.key.code == sf::Keyboard::S
               || e.key.code == sf::Keyboard::Down)
      {
        selectedOption = (selectedOption + 1) % maxOptions;
      }
      else if (e.key.code == sf::Keyboard::Enter)
      {
        changeState();
      }
    }
  }
}

void MainMenuState::updateInput(const float& dt)
{
}

void MainMenuState::changeState()
{
  switch (selectedOption)
  {
  case 0:
    newState = std::make_unique<GameState>(window, states);
    break;

  case 1:
    newState = std::make_unique<ScoreboardState>(window, states);
    break;

  case 2:
    break;
  }

  quit = true;
}

void MainMenuState::update(const float& dt, sf::RenderTarget* target)
{
  updateSFMLEvents(target);
  updateKeytime(dt);
  updateInput(dt);
  updateMenu();
}

void MainMenuState::render(sf::RenderTarget* target = nullptr)
{
  target->clear(sf::Color(38, 43, 68));

  target->draw(bg);

  target->draw(nameText);

  target->draw(titleText);

  for (int i = 0; i < maxOptions; i++)
    target->draw(menu[i]);
}