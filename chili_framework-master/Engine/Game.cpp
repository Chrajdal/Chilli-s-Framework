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
Bitmap font ("Font.bmp");

void Game::ComposeFrame()
{
	a.Draw(gfx, 50, 50, Colors::White);
	b.Draw(gfx, 100, 50, Colors::White);
	Bitmap f2 = font;
	for (int i = 0; i < 9; i++)
		f2.DrawPart(gfx, i * 105, 200, i * 105, 0, (i + 1) * 105, 250, Colors::White);
	for (int i = 0; i < 9; i++)
		f2.DrawPart(gfx, i * 105, 400, i * 105, 250, (i + 1) * 105, 500, Colors::White);
	for (int i = 0; i < 9; i++)
		f2.DrawPart(gfx, i * 105, 700, i * 105, 500, (i + 1) * 105, 650, Colors::White);

}