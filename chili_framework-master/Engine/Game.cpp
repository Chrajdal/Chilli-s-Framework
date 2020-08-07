#include "Game.h"

float2 center(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2);

float angle = std::numbers::pi / 4.0;
float start_len = 100;

void branch(Graphics& gfx, float2 start, float2 end)
{
	float len = (end - start).Len();
	//println(len);
	if (len < 1)
		return;
	
	gfx.draw_line_s(start.x, start.y, end.x, end.y, Colors::White);

	float2 s = end;
	float2 tmp = (end - start) * 0.666666 + end;

	auto e1 = rotate_point(s.x, s.y, angle, tmp);
	auto e2 = rotate_point(s.x, s.y, -angle, tmp);
	branch(gfx, s, e1);
	branch(gfx, s, e2);


}

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{	

}

void Game::Go()
{
	timer.restart();
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		wnd.Kill();
	gfx.BeginFrame();

	HandleInput();
	UpdateModel();
	ComposeFrame();

	gfx.EndFrame();

	println(int(1000.0/timer.elapsed()));
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

	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		angle += 0.005;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		angle -= 0.005;
	}

	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		start_len -= 1;
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		start_len += 1;
	}
}

void Game::UpdateModel()
{
	
}

void Game::ComposeFrame()
{

	branch(gfx, float2(center.x, Graphics::ScreenHeight - 10), float2(center.x, Graphics::ScreenHeight - start_len -10));
}

