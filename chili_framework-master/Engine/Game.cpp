#include "Game.h"

std::vector< std::vector<double2>> grid;
std::vector< std::vector<double2>> next;

double dA = 1.0f;
double dB = 0.5f;
double feed = 0.055f;
double k = 0.062f;

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{	
	for (int x = 0; x < Graphics::ScreenWidth; ++x)
	{
		std::vector<double2> tmp1;
		std::vector<double2> tmp2;
		for (int y = 0; y < Graphics::ScreenHeight; ++y)
		{
			tmp1.push_back({ 0.0f, 1.0f });
			tmp2.push_back({ 0.0f, 1.0f });
		}
		grid.push_back(tmp1);
		next.push_back(tmp2);
	}
	
	for (int n = 0; n < 10; n++) {
		int startx = int(rnd.next_double(20, Graphics::ScreenWidth - 20));
		int starty = int(rnd.next_double(20, Graphics::ScreenHeight- 20));
	
		for (int i = startx; i < startx + 10; i++) {
			for (int j = starty; j < starty + 10; j++) {
				double a = 1;
				double b = 0;
				grid[i][j] = double2(a, b);
				next[i][j] = double2(a, b);
			}
		}
	}
	//int startx = Graphics::ScreenWidth / 2;
	//int starty = Graphics::ScreenHeight / 2;
	//for (int x = startx; x < startx + 10; ++x)
	//{
	//	for (int y = starty; y < starty + 10; ++y)
	//	{
	//		grid[x][y].x = 1;
	//		next[x][y].x = 1;
	//		grid[x][y].y = 1;
	//		next[x][y].y = 1;
	//	}
	//}
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
}

void Game::UpdateModel()
{
	
}

auto laplaceA = [](int x, int y)
{
	double sum = 0;
	sum += next[x][y].x * -1;
	sum += next[x - 1][y].x * 0.2;
	sum += next[x + 1][y].x * 0.2;
	sum += next[x][y - 1].x * 0.2;
	sum += next[x][y + 1].x * 0.2;
	sum += next[x + 1][y + 1].x * 0.05;
	sum += next[x - 1][y - 1].x * 0.05;
	sum += next[x - 1][y + 1].x * 0.05;
	sum += next[x + 1][y - 1].x * 0.05;
	return sum;
};
auto laplaceB = [](int x, int y)
{
	double sum = 0;
	sum += next[x][y].y * -1;
	sum += next[x - 1][y].y * 0.2;
	sum += next[x + 1][y].y * 0.2;
	sum += next[x][y - 1].y * 0.2;
	sum += next[x][y + 1].y * 0.2;
	sum += next[x + 1][y + 1].y * 0.05;
	sum += next[x - 1][y - 1].y * 0.05;
	sum += next[x - 1][y + 1].y * 0.05;
	sum += next[x + 1][y - 1].y * 0.05;
	return sum;
};

void Game::ComposeFrame()
{
	for (int x = 1; x < Graphics::ScreenWidth-1; ++x)
	{
		for (int y = 1; y < Graphics::ScreenHeight-1; ++y)
		{
			double a = grid[x][y].x;
			double b = grid[x][y].y;
			
			next[x][y].x = a + (dA * laplaceA(x, y) - a * b * b + feed * (1 - a)) * 0.4;
			next[x][y].y = b + (dB * laplaceB(x, y) + a * b * b - (k + feed) * b) * 0.4;


			next[x][y].x = constrain_value(next[x][y].x, 0.0, 1.0);
			next[x][y].y = constrain_value(next[x][y].y, 0.0, 1.0);

			double rgb = constrain_value(std::abs(next[x][y].x - next[x][y].y) * 255, 0.0, 255.0);
			Color c = Colors::MakeRGB(rgb, rgb, rgb);
			gfx.PutPixel(x, y, c);
		}
	}

	std::swap(grid, next);
}
