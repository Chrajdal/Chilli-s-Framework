#include "Game.h"

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
	gfx.EndFrame();
}


void Game::UpdateModel()
{

}

void Game::ComposeFrame()
{

}