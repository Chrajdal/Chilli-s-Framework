#include "Game.h"


Bitmap goat("goat.bmp");
Bitmap goatdithered("goat.bmp");

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	goat.convert_to_gray_scale4();
	goatdithered = goat;
	goatdithered.apply_dithering();
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
	goat.Draw(gfx,0,0, Colors::Cyan);
	goatdithered.Draw(gfx, goat.get_width(), 0, Colors::Cyan);
}
