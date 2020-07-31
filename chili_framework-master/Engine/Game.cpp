#include "Game.h"

class Drop
{
public:
public:
	float2 pos;
	float radius;
	float speed;

	Drop(float x, float y)
	{
		pos.x = x;
		pos.y = y;
		radius = 10;
		speed = 5.345f;
	}

	void show(Graphics& gfx)
	{
		gfx.draw_circle_filled_s(pos.x, pos.y, radius, Colors::Cyan);
	}

	void move()
	{
		this->pos.y -= std::abs(speed);
	}
};


class Flower 
{
public:
	float2 pos;
	float radius;
	float2 speed;

	Flower(float x, float y)
	{
		pos.x = x;
		pos.y = y;
		radius = 20;
		speed = float2(1, 0);
	}

	void show(Graphics & gfx)
	{
		gfx.draw_circle_filled_s(pos.x, pos.y, radius, Colors::Yellow);
	}

	void move(void)
	{
		pos.x += speed.x;
		pos.y += speed.y;
	}

	void shift_down(void)
	{
		speed.x *= -1;
		pos.y += 50;
	}

	inline bool hits(const Drop& drop) const {
		return (drop.pos - this->pos).Len() <= (drop.radius + this->radius);
	}
};

class Ship
{
public:
	float2 pos;
	float2 size;

	float speed;

	Ship(void)
	{
		pos.x = Graphics::ScreenWidth / 2;
		pos.y = Graphics::ScreenHeight - 40;
		size = { 20, 60 };
		speed = 25.0f;
	}

	void move(int dir)
	{
		this->pos.x += dir * speed;
	}

	void show(Graphics& gfx)
	{
		float2 p1(pos.x - size.x / 2, pos.y - size.y / 2);
		float2 p2(pos.x + size.x / 2, pos.y - size.y / 2);
		float2 p3(pos.x - size.x / 2, pos.y + size.y / 2);
		float2 p4(pos.x + size.x / 2, pos.y + size.y / 2);

		gfx.fill_triangle_s(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, Colors::Magenta);
		gfx.fill_triangle_s(p4.x, p4.y, p2.x, p2.y, p3.x, p3.y, Colors::Magenta);
	}
};

Ship ship;
std::vector<Flower> flowers;
std::vector<Drop> drops;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			flowers.push_back(Flower(i * 80 + 80, j * 60 + 60));
		}
	}
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
		ship.pos.x = wnd.mouse.GetPosX();

		if (wnd.mouse.LeftIsPressed() && !left_pressed)
		{
			drops.push_back(Drop(ship.pos.x, Graphics::ScreenHeight - 50));
			left_pressed = true;
		}
		if (!wnd.mouse.LeftIsPressed())
		{
			left_pressed = false;
		}
	}
	//if (wnd.kbd.KeyIsPressed(VK_LEFT))
	//{
	//	ship.move(-1);
	//}
	//if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	//{
	//	ship.move(1);
	//}
	//if (wnd.kbd.KeyIsPressed(VK_SPACE))
	//{
	//	drops.push_back(Drop(ship.pos.x, Graphics::ScreenHeight - 50));
	//}
}

void Game::UpdateModel()
{
	if (flowers.empty())
	{
		for (int i = 0; i < 12; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				flowers.push_back(Flower(i * 80 + 80, j * 60 + 60));
			}
		}
	}

	bool hit_wall = false;
	for (Flower & flower : flowers)
	{
		flower.move();
		if (flower.pos.x + flower.radius >= Graphics::ScreenWidth-10)
		{
			hit_wall = true;
		}
		if (flower.pos.x - flower.radius <= 10)
		{
			hit_wall = true;
		}
	}

	if (hit_wall == true)
	{
		for (Flower & flower : flowers)
		{
			flower.shift_down();
		}
		hit_wall = false;
	}



	for (auto& drop : drops)
	{
		drop.move();
		auto tmp_flower = flowers;
		std::erase_if(flowers, [drop](Flower& flower) {return flower.hits(drop); });
		for (auto & flower : tmp_flower)
		{
			std::erase_if(drops, [flower](Drop& drop) {return flower.hits(drop); });
		}
	}
	std::erase_if(drops, [](const Drop& drop) {return drop.pos.y < drop.radius; });

	
}

void Game::ComposeFrame()
{
	ship.show(gfx);
	for (auto& drop : drops)
		drop.show(gfx);
	for (auto& flower : flowers)
		flower.show(gfx);
}