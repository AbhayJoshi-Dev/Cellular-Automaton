#include"Game.h"

Game::Game()
	:m_quit(false), m_countedFrames(0), m_rightBottonHold(false), m_leftBottonHold(false), m_canDraw(false), m_pause(false), m_speed(30)
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

	m_fpsTimer.Start();

	m_generation = 0;
	m_play = false;
	m_currentTime = 0;
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

	//buttons
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SameLine(238.f / 2.f - 35);

	ImGui::Text("Generation");
	std::string str = std::to_string(m_generation);
	ImGui::NewLine();
	ImGui::SameLine(238.f / 2.f - 10);
	ImGui::Text(str.c_str());

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SameLine(238.f / 2.f - 30);

	if (!m_pause)
	{
		if (ImGui::Button("Run" , ImVec2(60, 30)))
		{
			m_pause = true;
			m_play = true;
		}
	}
	else
	{
		if (ImGui::Button("Pause", ImVec2(60, 30)))
		{
			m_play = false;
			m_pause = false;
		}
	}

	if (m_play)
	{
		m_currentTime += (float)SCREEN_TICKS_PER_FRAME / 1000;
		if (m_currentTime >= 1.f / m_speed)
		{
			Life();
			m_currentTime = 0.f;
		}
	}

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SameLine(238.f / 2.f - 30);

	if (ImGui::Button("Step", ImVec2(60, 30)))
	{
		Life();
	}

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SameLine(238.f / 2.f - 30);

	if (ImGui::Button("Reset", ImVec2(60, 30)))
	{
		memset(m_cells, 0, sizeof(m_cells));
		m_generation = 0;
	}

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::SameLine(238.f / 2.f - 20);
	ImGui::Text("Speed");
	ImGui::NewLine();
	ImGui::SameLine(238.f / 2.f - 75);
	ImGui::SliderInt(" ", &m_speed, 0, 100);


	if (ImGui::IsWindowHovered() || ImGui::IsAnyItemActive())
		m_canDraw = false;
	else
		m_canDraw = true;

	ImGui::End();

	if (m_canDraw)
	{
		if (m_leftBottonHold)
			m_cells[m_mouseY / CELL_SIZE][m_mouseX / CELL_SIZE] = 1;
		else if (m_rightBottonHold)
			m_cells[m_mouseY / CELL_SIZE][m_mouseX / CELL_SIZE] = 0;
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
	if (m_canDraw)
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
	m_generation++;
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