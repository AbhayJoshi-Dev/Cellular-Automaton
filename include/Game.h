#pragma once

#include<iostream>
#include<SDL.h>
#include<imgui.h>
#include<imgui_impl_sdl.h>
#include<imgui_impl_sdlrenderer.h>
#include<string>

#include"Timer.h"

class Game
{
public:
	Game();
	~Game();
	void GameLoop();
	void Update();
	void Render();
	void Life();

private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	bool m_quit;
	SDL_Event m_event;

	const int CELL_SIZE = 6;
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 768;

	const static int NUM_CELLS = 130;

	bool m_cells[NUM_CELLS][NUM_CELLS];
	bool m_backupCells[NUM_CELLS][NUM_CELLS];

	const int SCREEN_FPS = 60;
	const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

	Timer m_fpsTimer;
	Timer m_capTimer;

	int m_countedFrames;
	int m_mouseX, m_mouseY;

	bool m_leftBottonHold;
	bool m_rightBottonHold;

	bool m_canDraw;

	bool m_pause;
	bool m_play;
	int m_speed;
	int m_generation;

	float m_currentTime;
};