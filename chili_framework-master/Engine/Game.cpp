#include "Game.h"

template <typename T>
void println(T t)
{
	std::cout << t << "\n";
}

template <typename T, typename ...U>
void println(T t, U ...u)
{
	std::cout << t << " ";
	println(u...);
}



long double x = 0.1l;
long double y = 0.0l;
long double z = 0.0l;

const long double a = 10.0l;
const long double b = 28.0l;
const long double c = 8.0l/3.0;

int2 center(Graphics::ScreenWidth/2, y + Graphics::ScreenHeight/2);
float2 scale(15, 15);

bool is_in_win(int x, int y)
{
	return	x >= 0 && x < Graphics::ScreenWidth&&
		y >= 0 && y < Graphics::ScreenHeight;
}

auto hash = [](const int3 & pt) {
	return (size_t)(pt.x * 100 + pt.y);
};

auto equal = [](const int3& pt1, const int3& pt2) {
	return ((pt1.x == pt2.x) && (pt1.y == pt2.y));
};
using PointHash = std::unordered_set<int3, decltype(hash), decltype(equal)>;

PointHash points;

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
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
	for (int i = 0; i < 10; ++i)
	{
		const long double dt = 0.01l;

		long double dx = a * (y - x);
		long double dy = x * (b - z) - y;
		long double dz = x * y - c * z;
		x += dx * dt;
		y += dy * dt;
		z += dz * dt;

		println(points.size(), "                ", (int)x, (int)y, (int)z);
	
		int3 finalp (x * scale.x, y * scale.y, z);
		finalp += int3(center.x, center.y, 0);

		if(is_in_win(finalp.x, finalp.y) && points.size() < 50000)
			points.insert(finalp);
	}
}

void Game::ComposeFrame()
{
	for (const auto& p : points)
	{
		gfx.PutPixel_s(p.x, p.y, Colors::White);
	}
}
