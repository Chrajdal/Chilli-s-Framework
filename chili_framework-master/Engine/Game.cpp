#include "Game.h"

class Cell
{
public:
	float2 pos;
	float r;

	Color col;
	Random rnd;
	Cell()
		: pos(random2D()), r(80), col(Colors::randomColor())
	{
		pos.x = std::abs(pos.x) * Graphics::ScreenWidth;
		pos.y = std::abs(pos.y) * Graphics::ScreenHeight;
	}

	Cell(float2 pos, float r, Color col)
		:
		pos(pos), r(r), col(col)
	{
	}

	bool clicked(int x, int y) const
	{
		float dist = std::sqrtf((x - pos.x) * (x - pos.x) + (y - pos.y) * (y - pos.y));
		return dist <= r;
	}

	Cell mitosis()
	{
		Color newColor;
		newColor.SetR(col.GetR() + rnd.next(-5, 5));
		newColor.SetG(col.GetG() + rnd.next(-5, 5));
		newColor.SetB(col.GetB() + rnd.next(-5, 5));
		col = newColor;
		newColor.SetR(col.GetR() + rnd.next(-5, 5));
		newColor.SetG(col.GetG() + rnd.next(-5, 5));
		newColor.SetB(col.GetB() + rnd.next(-5, 5));

		float half_area = std::numbers::pi * r * r / 2.0f;
		float newr = std::sqrtf((half_area) / std::numbers::pi);
		r = newr;
		return Cell (pos, r, newColor);
	}

	void move(void)
	{
		float2 vel = random2D();
		pos += vel;

		if (pos.x < 0 || pos.x > Graphics::ScreenWidth)
			pos.x *= -1;
		if (pos.y < 0 || pos.y > Graphics::ScreenHeight)
			pos.y *= -1;
	}

	void show(Graphics& gfx)
	{
		gfx.draw_circle_filled_s(pos.x, pos.y, r, col);
	}
};

std::list<Cell> cells;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	for (int i = 0; i < 5; ++i)
	{
		cells.push_back({});
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
			int l = cells.size();
			
			for (auto& cell : cells)
			{
				if (cell.clicked(wnd.mouse.GetPosX(), wnd.mouse.GetPosY()))
				{
					if (cell.r > 2)
					{
						cells.push_back(cell.mitosis());
					}
				}
			}
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
	for (auto& cell : cells)
	{
		cell.move();
		cell.show(gfx);
	}
}