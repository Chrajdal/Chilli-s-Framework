#include "Game.h"
#include "Bitmap.h"
#include "Font.h"

constexpr double g_pi = 3.141592653589;
constexpr double g = 1;

constexpr Tpoint<double> origin(Graphics::ScreenWidth / 2, Graphics::ScreenWidth / 2 - 100);
vector<Color> rainbow;

class double_pendulum
{
	double r1;
	double r2;
	double m1;
	double m2;

	double theta1 = g_pi / 2;
	double theta2 = g_pi / 8;
	double theta1_v = 0;
	double theta2_v = 0;
	double theta1_a = 0;
	double theta2_a = 0;

	Tpoint<double> pendulum1;
	Tpoint<double> pendulum2;

	vector<Tpoint<double>> pendulum_stroke;

public:
	double_pendulum(double _r1, double _r2, double _theta1 = g_pi/ 2, double _theta2 = g_pi / 8)
		:
		r1(_r1),
		r2(_r2),
		m1(40),
		m2(40)
	{
		theta1 = _theta1;
		theta2 = _theta2;
		theta1_v = 0;
		theta2_v = 0;
		theta1_a = 0;
		theta2_a = 0;

		pendulum1.m_x = origin.m_x + r1 * std::sin(theta1);
		pendulum1.m_y = origin.m_y + r1 * std::cos(theta1);
		pendulum2.m_x = pendulum1.m_x + r2 * std::sin(theta2);
		pendulum2.m_y = pendulum1.m_y + r2 * std::cos(theta2);
	}

	void update(void)
	{
		double num1 = -g * (2 * m1 + m2) * std::sin(theta1);
		double num2 = -m2 * g * std::sin(theta1 - 2 * theta2);
		double num3 = -2 * std::sin(theta1 - theta2) * m2;
		double num4 = theta2_v * theta2_v * r2 + theta1_v * theta1_v * r1 * std::cos(theta1 - theta2);
		double num5 = r1 * (2 * m1 + m2 - m2 * std::cos(2 * theta1 - 2 * theta2));
		theta1_a = (num1 + num2 + num3 * num4) / num5;

		num1 = 2 * sin(theta1 - theta2);
		num2 = theta1_v * theta1_v * r1 * (m1 + m2);
		num3 = g * (m1 + m2) * std::cos(theta1);
		num4 = theta2_v * theta2_v * r2 * m2 * std::cos(theta1 - theta2);
		num5 = r2 * (2 * m1 + m2 - m2 * std::cos(2 * theta1 - 2 * theta2));
		theta2_a = (num1 * (num2 + num3 + num4)) / num5;

		pendulum1.m_x = origin.m_x + r1 * std::sin(theta1);
		pendulum1.m_y = origin.m_y + r1 * std::cos(theta1);
		pendulum2.m_x = pendulum1.m_x + r2 * std::sin(theta2);
		pendulum2.m_y = pendulum1.m_y + r2 * std::cos(theta2);

		theta1_v += theta1_a;
		theta2_v += theta2_a;
		theta1 += theta1_v;
		theta2 += theta2_v;

		theta1_v *= 0.999;
		theta2_v *= 0.999;

		if (pendulum2.m_x > 0 && pendulum2.m_x < Graphics::ScreenWidth &&
			pendulum2.m_y > 0 && pendulum2.m_y < Graphics::ScreenHeight)
			pendulum_stroke.emplace_back(pendulum2);
		if (pendulum_stroke.size() > 10)
			pendulum_stroke.erase(pendulum_stroke.begin());
	}

	void draw(Graphics & gfx, const Color & c) const
	{
		//// draw first pendulum
		//gfx.draw_line_s(origin.m_x, origin.m_y, pendulum1.m_x, pendulum1.m_y, Colors::Black);
		//gfx.draw_circle_s(pendulum1.m_x, pendulum1.m_y, m1 / 2, Colors::Black);
		//
		//// draw second pendulum
		//gfx.draw_line_s(pendulum1.m_x, pendulum1.m_y, pendulum2.m_x, pendulum2.m_y, Colors::Black);
		//gfx.draw_circle_s(pendulum2.m_x, pendulum2.m_y, m2 / 2, Colors::Black);

		if (pendulum_stroke.size() < 2)
			return;
		for (auto i = pendulum_stroke.begin(); i != pendulum_stroke.end() - 1; ++i)
			gfx.draw_line_s(i->m_x, i->m_y, (i + 1)->m_x, (i + 1)->m_y, c);
	}
};
vector<double_pendulum> pendulums;

vector<Color> test_rainbow;
Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	rainbow = generate_colors(Colors::Red, Colors::Magenta, 5000);
	for (int i = 0; i < rainbow.size(); ++i)
	{
		pendulums.push_back(double_pendulum(200 /*+ i * 1e-6*/, 200 /*- i * 1e-6*/, 50*g_pi/51 + i * 1e-7, 5*g_pi / 8 + i * 1e-7));
	}
}
bool update_model = false;
void Game::Go()
{
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		wnd.Kill();
	gfx.BeginFrame();

	HandleInput();
	if(update_model == true)
		UpdateModel();
	ComposeFrame();

	gfx.EndFrame();
}

void Game::HandleInput()
{
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
		update_model = true;
}

void Game::UpdateModel()
{
	for (auto & i : pendulums)
		i.update();
}

void Game::ComposeFrame()
{
	int idx = 0;
	for (auto & i : pendulums)
		i.draw(gfx, rainbow[idx++]);
}
