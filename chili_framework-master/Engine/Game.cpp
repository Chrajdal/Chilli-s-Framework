#include "Game.h"
#include "Bitmap.h"

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

	UpdateModel();
	ComposeFrame();

	gfx.EndFrame();
}


void Game::UpdateModel()
{

}

Bitmap a("0.bmp");
Bitmap b("1.bmp");

void Game::ComposeFrame()
{
	a.Draw(gfx, 50, 50, Colors::White);
	b.Draw(gfx, 100, 50, Colors::White);
}