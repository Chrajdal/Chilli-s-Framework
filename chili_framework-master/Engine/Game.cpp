#include "Game.h"
#include "Bitmap.h"
#include "Font.h"

Bitmap goat1("goat.bmp");
Bitmap goat2(goat1);

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));
	
	
	//goat2.resize(goat1.get_width() / 2, goat1.get_height() / 2);
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


void Game::ComposeFrame()
{
	goat1.Draw(gfx, 0, 0, Colors::White);
	goat2.Draw(gfx, goat1.get_width(), 0, Colors::White);
}
