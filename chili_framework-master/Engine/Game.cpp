#include "Game.h"

float2 center(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2);

float angle = std::numbers::pi / 6.0;
float start_len = 100;

class Branch
{
public:
	std::shared_ptr<float2> S;
	float2 E;

	bool finished = false;

	Branch(const std::shared_ptr<float2> & start, float2 end = {})
		: S(start)
	{
		E = end;
	}

	void show(Graphics& gfx)
	{
		float len = (E - *S).Len();
		if (len > 1 || len < 1e4)
			gfx.draw_line_s(*S, E, Colors::White);
	}

	void jitter(void)
	{
		E += random2D();
	}

	void branch(std::vector<Branch> & tree)
	{
		if (finished == false)
		{
			finished = true;
			float len = (E - *S).Len();
			if (len < 1)
				return;

			float2 tmp = (E - *S) * 0.666666 + E;

			float2 e1 = rotate_point(E.x, E.y, angle, tmp);
			float2 e2 = rotate_point(E.x, E.y, -angle*1.5, tmp);

			Branch right(std::make_shared<float2>(E), e1);
			Branch left(std::make_shared<float2>(E), e2);

			tree.push_back(right);
			tree.push_back(left);
		}
	}
};

std::vector<Branch> tree;
std::vector<float2> leaves;
float2 START(center.x, Graphics::ScreenHeight - 10);

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	Branch root(std::make_shared<float2>(START), float2(center.x, Graphics::ScreenHeight - start_len - 10));

	tree.push_back(root);
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
int count = 0;
void Game::HandleInput()
{

	if (wnd.mouse.IsInWindow())
	{
		if (wnd.mouse.RightIsPressed())
		{
			//
		}

		if (wnd.mouse.LeftIsPressed() && !left_pressed)
		{
			left_pressed = true;

			for (int i = tree.size() - 1; i >= 0; --i)
			{
				tree[i].branch(tree);
			}

			count++;
			if (count == 5)
			{
				for (int i = tree.size()-1; i >= 0; --i)
				{
					if (!tree[i].finished)
					{
						leaves.push_back(tree[i].E);
					}					
				}
				count = 0;
			}

		}
		if (!wnd.mouse.LeftIsPressed())
			left_pressed = false;
	}

	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		angle += 0.005;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		angle -= 0.005;
	}

	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		start_len -= 1;
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		start_len += 1;
	}
}

void Game::UpdateModel()
{
	
}

void Game::ComposeFrame()
{
	for (auto& branch : tree)
	{
		branch.show(gfx);
		branch.jitter();
	}
	for (auto& leave : leaves)
	{
		gfx.draw_elipse_s(leave, 8, 8, Colors::Purple);
		leave.y += rnd.next_float(1, 3);
	}
	//branch(gfx, float2(center.x, Graphics::ScreenHeight - 10), float2(center.x, Graphics::ScreenHeight - start_len -10));
}

