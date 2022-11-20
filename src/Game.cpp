#include"Game.h"

Game::Game()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;

	m_window = SDL_CreateWindow("Cellular Automaton", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (m_window == NULL)
		std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

	if (m_renderer == NULL)
		std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;

	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
	ImGui_ImplSDLRenderer_Init(m_renderer);


	m_quit = false;

	m_countedFrames = 0;
	m_fpsTimer.Start();
	m_mouseX = 0;
	m_mouseY = 0;
	m_rightBottonHold = false;
	m_leftBottonHold = false;
}

Game::~Game()
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}

void Game::GameLoop()
{
	while (!m_quit)
	{
		m_capTimer.Start();

		while (SDL_PollEvent(&m_event) != 0)
		{
			ImGui_ImplSDL2_ProcessEvent(&m_event);
			if (m_event.type == SDL_QUIT)
				m_quit = true;
			if (m_event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (m_event.button.button == SDL_BUTTON_LEFT)
				{
					m_leftBottonHold = true;
				}
				else if (m_event.button.button == SDL_BUTTON_RIGHT)
				{
					m_rightBottonHold = true;
				}
			}

			if (m_event.type == SDL_MOUSEBUTTONUP)
			{
				if (m_event.button.button == SDL_BUTTON_LEFT)
					m_leftBottonHold = false;
				else if (m_event.button.button == SDL_BUTTON_RIGHT)
					m_rightBottonHold = false;
			}
		}

		float avgFps = m_countedFrames / (m_fpsTimer.GetTicks() / 1000.f);
		if (avgFps > 2000000)
			avgFps = 0.f;

		Update();
		Render();

		++m_countedFrames;

		int framesTicks = m_capTimer.GetTicks();
		if (framesTicks < SCREEN_TICKS_PER_FRAME)
		{
			SDL_Delay(SCREEN_TICKS_PER_FRAME - framesTicks);
		}
	}
}

void Game::Update()
{
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Cellular Automaton", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos(ImVec2(128 * 6, 0));
	ImGui::SetWindowSize(ImVec2(238, 768));
	if (ImGui::Button("Save"))
	{
		std::cout << "Saved" << std::endl;
	}
	ImGui::End();


	if (m_leftBottonHold)
		m_cells[m_mouseY / CELL_SIZE][m_mouseX / CELL_SIZE] = 1;
	else if (m_rightBottonHold)
		m_cells[m_mouseY / CELL_SIZE][m_mouseX / CELL_SIZE] = 0;

	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_RETURN])
	{
		Life();
	}
}

void Game::Render()
{
	ImGui::Render();

	SDL_Rect vertical_rect = { 0, 0, 1, SCREEN_HEIGHT };
	SDL_Rect horizontal_rect = { 0, 0, SCREEN_WIDTH, 1 };


	SDL_RenderClear(m_renderer);
	SDL_SetRenderDrawColor(m_renderer, 40, 40, 40, 255);


	for (int i = 0; i < NUM_CELLS; i++)
	{
		vertical_rect.x = (CELL_SIZE * i) - 1;
		SDL_RenderDrawRect(m_renderer, &vertical_rect);

		horizontal_rect.y = (CELL_SIZE * i) - 1;
		SDL_RenderDrawRect(m_renderer, &horizontal_rect);
	}

	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

	//mouse pos rect
	SDL_GetMouseState(&m_mouseX, &m_mouseY);
	m_mouseX -= m_mouseX % CELL_SIZE;
	m_mouseY -= m_mouseY % CELL_SIZE;
	SDL_Rect cell_rect = { m_mouseX, m_mouseY, CELL_SIZE - 1, CELL_SIZE - 1 };
	SDL_RenderFillRect(m_renderer, &cell_rect);

	//draw rect
	for (int y = 0; y < NUM_CELLS; y++)
	{
		for (int x = 0; x < NUM_CELLS; x++)
		{
			if (m_cells[y][x] == 1)
			{
				cell_rect = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1 };
				SDL_RenderFillRect(m_renderer, &cell_rect);
			}
		}
	}

	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void Game::Life()
{
	for (int y = 0; y < NUM_CELLS; y++)
	{
		for (int x = 0; x < NUM_CELLS; x++)
		{
			m_backupCells[y][x] = m_cells[y][x];
		}
	}

	int neighbours = 0;
	for (int y = 0; y < NUM_CELLS; y++)
	{
		for (int x = 0; x < NUM_CELLS; x++)
		{
			neighbours = m_backupCells[y - 1][x - 1] + m_backupCells[y - 1][x] + m_backupCells[y - 1][x + 1] +
				m_backupCells[y][x - 1] + m_backupCells[y][x + 1] +
				m_backupCells[y + 1][x - 1] + m_backupCells[y + 1][x] + m_backupCells[y + 1][x + 1];

			if (neighbours < 2)
				m_cells[y][x] = 0;
			else if (neighbours > 3)
				m_cells[y][x] = 0;
			else if (neighbours == 3)
				m_cells[y][x] = 1;
		}
	}
}