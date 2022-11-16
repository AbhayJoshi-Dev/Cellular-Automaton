#pragma once

#include<iostream>
#include<SDL.h>

#include"Timer.h"

class Game
{
public:
	Game();
	~Game();
	void GameLoop();
	void Update();
	void Render();

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	bool quit;
	SDL_Event m_event;

	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
};