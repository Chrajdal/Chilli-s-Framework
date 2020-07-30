#include "Game.h"

constexpr Color rain_color = Colors::MakeRGB(138, 43, 226);
Random rnd;


class Drop
{
public:
	float x;
	float y;
	float len;
	float yspeed;

	float z;

	int thickness;
	
	float zmax;

	Drop()
	{
		x = rnd.next(0, Graphics::ScreenWidth);
		y = rnd.next(-1500, -100);
		zmax = 10000.0f;
		z = rnd.next_float(0.0f, zmax);

		len =		map_value(z, 0, zmax, 10, 20);
		yspeed =	map_value(z, 0, zmax, 24.0f, 30.0f);
		thickness = map_value(z, 0, zmax, 1, 10);

	}

	void fall() 
	{
		y = y + yspeed;
		float grav = map_value(z, 0, zmax, 1, 2);
		yspeed = yspeed + grav;
		if (y > Graphics::ScreenHeight)
		{
			*this = Drop();
		}
	}

	void show(Graphics& gfx)
	{
		for (int i = 0; i < thickness; ++i)
		{
			gfx.draw_line_s(x + i, y, x + i, y + len, rain_color);
		}
		
	}
};


std::vector<Drop> rain;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	//for (int i = 0; i < 5; ++i)
	//{
	//	rain.push_back(Drop());
	//}
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

void Game::HandleInput()
{
	
}

void Game::UpdateModel()
{
	for (auto& drop : rain)
	{
		drop.fall();
	}
	if (rain.size() < 50000 /*&& rnd.next() % 5 == 0*/)
		rain.push_back(Drop());
	
}

void Game::ComposeFrame()
{
	for (auto& drop : rain)
	{
		drop.show(gfx);
	}
	gfx.draw_line(0, 0, map_value(rain.size(), 0, 50000, 0, Graphics::ScreenWidth), 0, Colors::Magenta);
}

