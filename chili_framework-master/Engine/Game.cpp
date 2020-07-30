#include "Game.h"

float2 rotate_point(float cx, float cy, float angle, float2 p) {

	return float2(std::cos(angle) * (p.x - cx) - std::sin(angle) * (p.y - cy) + cx,
		std::sin(angle) * (p.x - cx) + std::cos(angle) * (p.y - cy) + cy);
}

class Box 
{
public:
	float2 point;
	float size;

	Box(float2 p, float size)
	{
		point = p;
		this->size = size;
	}

	void draw(Graphics& gfx, float angle)
	{
		float2 tmppoint = rotate_point(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2, angle, point);

		float2 p1 = float2(tmppoint.x - size / 2, tmppoint.y - size / 2);
		float2 p2 = float2(tmppoint.x - size / 2, tmppoint.y + size / 2);
		float2 p3 = float2(tmppoint.x + size / 2, tmppoint.y - size / 2);
		float2 p4 = float2(tmppoint.x + size / 2, tmppoint.y + size / 2);

		p1 = rotate_point(tmppoint.x, tmppoint.y, angle, p1);
		p2 = rotate_point(tmppoint.x, tmppoint.y, angle, p2);
		p3 = rotate_point(tmppoint.x, tmppoint.y, angle, p3);
		p4 = rotate_point(tmppoint.x, tmppoint.y, angle, p4);

		gfx.FillTriangle_s(
			p1.x , p1.y,
			p2.x, p2.y,
			p3.x, p3.y, Colors::Gray);
		gfx.FillTriangle_s(
			p4.x, p4.y,
			p2.x, p2.y,
			p3.x, p3.y, Colors::Gray);

		//gfx.draw_line_s(p1.x, p1.y, p2.x, p2.y, Colors::LightGray);
		//gfx.draw_line_s(p2.x, p2.y, p4.x, p4.y, Colors::LightGray);
		//gfx.draw_line_s(p3.x, p3.y, p4.x, p4.y, Colors::LightGray);
		//gfx.draw_line_s(p1.x, p1.y, p3.x, p3.y, Colors::LightGray);
	}

	std::vector<Box> generate(void)
	{
		std::vector<Box> res;
		for (int i = -1; i < 2; ++i)
		{
			for (int j = -1; j < 2; ++j)
			{
				if (!(i == 0 && j == 0))
				{
					float new_size = size / 3;
					res.push_back(Box(float2(point.x + i * new_size, point.y + j * new_size), new_size));
				}
			}
		}
		return res;
	}


};

float a = 1e-2f;
float box_size = Graphics::ScreenWidth;
Box box(float2(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2), box_size);

std::vector<Box> sponge;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	sponge.push_back(box);
}

bool shit_is_pressed = false;

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
	if (wnd.mouse.IsInWindow())
	{
		if (wnd.mouse.LeftIsPressed() && shit_is_pressed == false)
		{
			shit_is_pressed = true;
			std::this_thread::sleep_for(1500ms);
			std::vector<Box> tmp;
			for (auto& box : sponge)
			{
				auto next = box.generate();
				for (auto& b : next)
					tmp.push_back(b);
			}
			sponge = tmp;
		}
		if (!wnd.mouse.LeftIsPressed())
		{
			shit_is_pressed = false;
		}
	}
}

void Game::UpdateModel()
{

}

void Game::ComposeFrame()
{
	a += 0.005;
	for (auto& b : sponge)
	{
		b.draw(gfx, a);
	}
}

