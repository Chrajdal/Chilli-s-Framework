#include "Game.h"

const int w = 16;

const int cols = Graphics::ScreenWidth / w;
const int rows = Graphics::ScreenHeight / w;

int visited_cells = 0;

int index(int i, int j)
{
	return i + j * cols;
}

class Cell
{
public:
	int i;
	int j;

	bool visited;

	enum wall
	{
		top = 0, right = 1, bottom = 2, left = 3
	};

	bool open[4] = { true, true, true, true };

	Cell(int _i = 0, int _j = 0)
		: i(_i), j(_j), visited(false)
	{
	}

	~Cell() {}

	void show(Graphics& gfx)
	{
		int x = i * w;
		int y = j * w;
		int h = w;
		auto col = Colors::LightGray;
		if (visited)
		{
			//gfx.draw_rect_filled(x + 1, y + 1, w - 1, h - 1, Colors::Magenta);
			gfx.draw_rect_filled(x, y, w, h, Colors::Magenta);
		}

		if (open[wall::top])
			gfx.draw_line_s(x    , y    , x + w, y    , col);	// ---
		if (open[wall::right])
			gfx.draw_line_s(x    , y    , x    , y + h, col);	// |
		if (open[wall::left])
			gfx.draw_line_s(x + w, y    , x + w, y + h, col);	//   |
		if (open[wall::bottom])
			gfx.draw_line_s(x    , y + h, x + w, y + h, col);	// ___ 
	}

	auto checkNeigbors(std::array<Cell, rows * cols> & grid)
	{
		std::vector<Cell* > neigbors(4);
		
		if (j > 0)
			neigbors[wall::top] = &grid[index(i, j - 1)];
		if (i < rows - 1)
			neigbors[wall::right] = &grid[index(i + 1, j)];
		if (j < cols - 1)
			neigbors[wall::bottom] = &grid[index(i, j + 1)];
		if (i > 0)
			neigbors[wall::left] = &grid[index(i - 1, j)];
		
		std::vector<Cell* > res;
		for (auto& c : neigbors)
			if (c != nullptr)
				if (c->visited == false)
					res.push_back(c);
		
		if (res.size() > 1)
			return res[rnd.next(0, res.size() - 1)];
		else if (res.size() == 1)
			return res[0];
		else
			throw "not found anything";
	}


};

std::array<Cell, rows*cols> grid;
Cell* current = &grid[0];
std::list<Cell*> stack;
void remove_wall(Cell* a, Cell* b)
{
	auto x = a->i - b->i;
	if (x == 1)
	{
		a->open[Cell::wall::right] = false;
		b->open[Cell::wall::left] = false;
	}
	else if (x == -1)
	{
		a->open[Cell::wall::left] = false;
		b->open[Cell::wall::right] = false;
	}
	else // x == 0
	{
		x = a->j - b->j;
		if (x == 1)
		{
			a->open[Cell::wall::top] = false;
			b->open[Cell::wall::bottom] = false;
		}
		else if (x == -1)
		{
			a->open[Cell::wall::bottom] = false;;
			b->open[Cell::wall::top] = false; 
		}
	}
}

inline bool unvisited()
{
	return std::find_if(grid.begin(), grid.end(), [](const Cell& c) {return c.visited == false; }) != grid.end();
}

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	for (int j = 0; j < cols; j++)
	{
		for (int i = 0; i < rows; i++)
		{
			grid[index(i, j)] = Cell(i, j);
		}
	}

	while (unvisited())
	{
		current->visited = true;
		try
		{
			// STEP 1
			Cell* next = current->checkNeigbors(grid);
			if (next->visited == false)
				++visited_cells;
			next->visited = true;
			

			// STEP 2
			stack.push_back(current);

			// STEP 3
			remove_wall(current, next);

			// STEP 4
			current = next;
		}
		catch (...)
		{
			//gfx.draw_circle_filled(500, 500, 200, Colors::Red);
			if (!stack.empty())
			{
				current = *stack.rbegin();
				stack.pop_back();
			}
		}
	}
}

void Game::Go()
{
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		wnd.Kill();
	gfx.BeginFrame();

	HandleInput();
	UpdateModel();
	ComposeFrame();

	gfx.EndFrame();
}

bool left_pressed = false;

void Game::HandleInput()
{
	if (wnd.mouse.IsInWindow())
	{
		if (wnd.mouse.LeftIsPressed() && !left_pressed)
		{
			left_pressed = true;
		}
		if (!wnd.mouse.LeftIsPressed())
			left_pressed = false;
	}


}

void Game::UpdateModel()
{

} 

void Game::ComposeFrame()
{
	for (auto& cell : grid)
	{
		cell.show(gfx);
	}
	int x = current->i * w;
	int y = current->j * w;
	int h = w;
	gfx.draw_rect_filled(x, y, w, h, Colors::Cyan);
}