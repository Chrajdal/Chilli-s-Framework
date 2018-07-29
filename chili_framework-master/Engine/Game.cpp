#include "Game.h"

Cvector3<double> a (50,50,0);
Cvector3<double> b (600, 600, 0);

class plane
{
public:
};

class Camera
{
public:
	Cvector3<double> position;
	Cvector3<double> rotation;
	Cvector3<double> scale;

	//plane far, near, left, right, top, down;
};

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

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

}


void Game::ComposeFrame()
{
	gfx.draw_line_s(a.x, a.y, b.x, b.y, Colors::Blue);
}
