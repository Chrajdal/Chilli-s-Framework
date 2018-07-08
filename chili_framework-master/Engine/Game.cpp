#include "Game.h"
#include "Bitmap.h"
#include "Font.h"

#define CHAR_WIDTH 106;

string str = "";

void gen_random(string & str, const int len)
{
	str = "";
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i)
		str += alphanum[rand() % (sizeof(alphanum) - 1)];
}

void gen_randascii(string & str, const int len)
{
	str = "";
	static const char arr[] = "!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	for (int i = 0; i < len; ++i)
		str += arr[rand() % (sizeof(arr) - 1)];
}

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));
	
	str = "domecekas";
	//for (int i = 0; i < 9; i++)
	//{
	//	str += char(rand() % 26) + 'a';
	//}
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

Bitmap test("9.bmp");
Bitmap test_2("8.bmp");
bool has_been_resized = false;
void Game::ComposeFrame()
{

	test.Draw(gfx, 0, 0, Colors::White);
	if (wnd.kbd.KeyIsPressed(VK_SPACE) && has_been_resized == false);
	{
		test.resize(50, 50);
		has_been_resized = true;
		test_2 = test;
	}
	test_2.Draw(gfx, 101, 101, Colors::White);
}
