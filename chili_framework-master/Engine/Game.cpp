#include "Game.h"

class Planet
{
public:
	float distance;
	float radius;
	float angle;
	float angle_vel;
	Color col;
	std::vector<Planet> planets;

	Planet(float d = 0,  float r = 0, float angle = 0, const Color & col = Colors::randomColor())
		:
		distance(d), radius(r), angle(angle), col(col), planets({})
	{
		angle_vel = rnd.next_float(-0.5, 0.5) / radius;
	}

	void move(void)
	{
		angle += angle_vel;
		for (auto& planet : planets)
			planet.move();
	}

	void spawnMoons(int total, int level)
	{
		for (int i = 0; i < total; ++i)
		{
			float r = rnd.next_float(0.7, 0.9) * radius / level;
			float d = rnd.next_float(4, 8) * (radius + r) / level;
			float a = rnd.next_float(0, std::numbers::pi * 2);
			planets.push_back(Planet(d, r, a));
		}
		if (level < 4)
		{
			for (auto& pl : planets)
			{
				pl.spawnMoons(rnd.next(0, 3), level + 1);
			}
		}
	}

	void show(Graphics& gfx, float2 translation)
	{
		float2 next_translation = rotate_point(
			translation.x,
			translation.y,
			angle,
			translation + float2(distance,0));

		gfx.draw_circle_filled_s(next_translation.x, next_translation.y, radius * 2, col);

		for (auto& planet : planets)
		{
			planet.show(gfx, next_translation);
		}
	}


};

Planet planet (0, 25, 0, Colors::Yellow);

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	planet.spawnMoons(5, 1);
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
	planet.move();	
}

void Game::ComposeFrame()
{	
	planet.show(gfx, { Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2 });
}