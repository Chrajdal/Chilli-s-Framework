#include "Game.h"

Random r;



class Star {
public:
	float x;
	float y;
	float z;

	float speed;

	float pz;

	Star()
	{
		x = r.next_float(-Graphics::ScreenWidth/2, Graphics::ScreenWidth/2);
		y = r.next_float(-Graphics::ScreenHeight/2, Graphics::ScreenHeight/2);
		z = r.next_float(0, Graphics::ScreenWidth);
		pz = z;
	}
	~Star() {};

	void set_speed(float speed)
	{
		this->speed = speed;
	}

	void update_and_show(Graphics & gfx)
	{
		z = z - speed;
		if (z < 1)
		{
			z = Graphics::ScreenWidth;
			x = r.next_float(-Graphics::ScreenWidth/2, Graphics::ScreenWidth/2);
			y = r.next_float(-Graphics::ScreenHeight/2, Graphics::ScreenHeight/2);
			pz = z;
		}

		float sx = map_value(x / z, 0, 1, 0, Graphics::ScreenWidth);
		float sy = map_value(y / z, 0, 1, 0, Graphics::ScreenHeight);

		sx += Graphics::ScreenWidth / 2;
		sy += Graphics::ScreenHeight / 2;
		
		float r = map_value(z, 0, Graphics::ScreenWidth, 8, 0);

		gfx.draw_circle_filled_s(sx , sy , r, Colors::White);

		float px = map_value(x / pz, 0, 1.1, 0, Graphics::ScreenWidth);
		float py = map_value(y / pz, 0, 1.1, 0, Graphics::ScreenHeight);

		pz = z;
		px += Graphics::ScreenWidth / 2;
		py += Graphics::ScreenHeight / 2;
		gfx.draw_line_s(px, py, sx, sy, Colors::Gray);

	}
};


std::vector<Star> stars;


Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	for (int i = 0; i < 1600; ++i)
		stars.push_back(Star());
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
	for (auto& star : stars)
		star.set_speed(map_value(wnd.mouse.GetPosX(), 0, Graphics::ScreenWidth, 0, 50));


}

void Game::UpdateModel()
{
}


void Game::ComposeFrame()
{
	for (auto& star : stars)
		star.update_and_show(gfx);
		
		
}

