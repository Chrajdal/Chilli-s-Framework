#include "Game.h"

float2 center(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2);

float angle = 90 * (std::numbers::pi / 180);
float start_len = 500;
float2 START(center.x, center.y);

struct Rule
{
	std::string a;
	std::string b;
};

std::string axiom = "FX";
std::string sentence = axiom;
std::vector<Rule> rules;

void generate(void)
{
	start_len *= 0.5;
	std::string next = "";
	for (auto & c : sentence)
	{
		bool matchedrule = false;
		for (auto & rule : rules)
		{
			std::string x(1,c);
			if (rule.a == x)
			{
				next += rule.b;
				matchedrule = true;
				break;
			}
		}
		if (!matchedrule)
		{
			next += std::string(1, c);
		}
	}
	sentence = next;
}

class Translation
{
public:
	Translation()
	{}
	float2 translation;
	float2 direction;
};

Translation current;
std::list<Translation> stack;

void turtle(Graphics & gfx)
{
	current.translation = START;
	current.direction = float2(1, 0);

	stack.push_back(current);

	for (auto & c : sentence)
	{
		float2 from = stack.rbegin()->translation;
		float2 to = from + stack.rbegin()->direction * start_len;
		float2 rotated_dirR = rotate_point(0, 0, angle, stack.rbegin()->direction);
		float2 rotated_dirL = rotate_point(0, 0, -angle, stack.rbegin()->direction);
		auto to_be_saved = *stack.rbegin();
		
		switch (c)
		{
		case 'X': // draw forward
			gfx.draw_line_s(from.x, from.y, to.x, to.y, Colors::LightGray);
			stack.rbegin()->translation = to;
			break;
		case 'Y': // draw forward
			gfx.draw_line_s(from.x, from.y, to.x, to.y, Colors::LightGray);
			stack.rbegin()->translation = to;
			break;
		case '+': // rotate right
			rotated_dirR.Normalize();
			stack.rbegin()->direction = rotated_dirR;
			break;
		case '-': // rotate left
			rotated_dirL.Normalize();
			stack.rbegin()->direction = rotated_dirL;
			break;
		case '[': // push translation and angle
			stack.push_back(to_be_saved);
			break;
		case ']': // pop translation and angle
			stack.pop_back();
			break;
		}
	}
}

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	rules.push_back({ "X", "X+YF+" });
	rules.push_back({ "Y", "-FX-Y" });
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

	//println(int(1000.0/timer.elapsed()));
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
			generate();
			println(sentence);

		}
		if (!wnd.mouse.LeftIsPressed())
			left_pressed = false;
	}

	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		//angle += 0.005;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		//angle -= 0.005;
	}

	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		start_len *=1.01;
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		start_len *= 0.99;
	}
}

void Game::UpdateModel()
{
	
}

void Game::ComposeFrame()
{
	turtle(gfx);
}

