#pragma once

#include "GameState.h"

class Game
{
private:
	// Variables
	sf::RenderWindow *window;
	sf::Event sfEvent;

	std::stack<std::unique_ptr<State>> states;

	sf::Clock dtClock;
	float dt;

	sf::Image icon;

	// Initialization
	void initWindow();
	void initStates();
	void initResources();

public:
	// Constructor and destructor
	Game();
	virtual ~Game();

	sf::RenderWindow& getWindow();

	// Methods
	void update();
	void updateDt();
	void run();
	void render();
};

extern Game game;