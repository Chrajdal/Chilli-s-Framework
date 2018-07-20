#include "Game.h"

Random rnd;

constexpr double g = 1;
const double m_pi = std::acos(-1);
constexpr double stiffness = 0.8;
constexpr double rest_lenght = 200;
constexpr double mass = 5;
constexpr double damping = 0.3;
constexpr double eps = 1e-5; // just to avoid infinities

Tpoint<double> origin(500, 300);
Tpoint<double> position(origin.m_x, origin.m_y + 326);
double theta = 0;
Tpoint<double> velocity(0.1, 0);
Tpoint<double> acceleration(0, 0);
double displacement = 0;
double lenght = 0;

bool keepUpdating = true;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));
	cout << fixed << setprecision(3);
}

void Game::Go()
{
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		wnd.Kill();
	gfx.BeginFrame();

	HandleInput();
	if(keepUpdating == true)
		UpdateModel();
	ComposeFrame();

	gfx.EndFrame();
}

void Game::HandleInput()
{
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		keepUpdating = false;
		return;
	}

	if (wnd.kbd.KeyIsPressed(VK_RETURN))
	{
		keepUpdating = true;
		return;
	}


	if (wnd.mouse.LeftIsPressed() && wnd.mouse.IsInWindow())
	{
		position.m_x = wnd.mouse.GetPosX();
		position.m_y = wnd.mouse.GetPosY();

		velocity = {};
		acceleration = {};

		keepUpdating = false;
		return;
	}

	if (wnd.kbd.KeyIsPressed(0x52))
	{
		origin = { 500, 300 };
		position = { origin.m_x + rnd.next(-50,50), origin.m_y + rnd.next(-500, 500) };
		theta = 0;
		velocity ={ rnd.next_double(-10,10), rnd.next_double(-10,10) };
		acceleration = { 0, 0 };
		displacement = 0;
		lenght = 0;

		keepUpdating = true;
	}
	
	//if (!wnd.mouse.LeftIsPressed())
	//{
	//	keepUpdating = true;
	//	return;
	//}

}

void Game::UpdateModel()
{
	lenght = sqrt(sq_distance(origin, position));
	displacement = lenght - rest_lenght;
	
	double koverm_x;
	double koverm_y = koverm_x = (stiffness / (mass + eps));
	
	double ssin_x = ((position.m_x - origin.m_x) / (lenght + eps));
	double scos_x = ((position.m_y - origin.m_y) / (lenght + eps));

	double tmp_x = koverm_x * displacement * ssin_x;
	double tmp_y = koverm_y * displacement * scos_x;

	double damping_x = (damping / (mass + eps)) * velocity.m_x;
	double damping_y = (damping / (mass + eps)) * velocity.m_y;;

	acceleration.m_x = -tmp_x - damping_x;
	acceleration.m_y = g - tmp_y - damping_y;

	double ax = acceleration.m_x;
	double ay = acceleration.m_y;

	double a = std::sqrt(ax*ax + ay * ay);
	acceleration.m_x /= a;
	acceleration.m_y /= a;

	velocity.m_x += acceleration.m_x;
	velocity.m_y += acceleration.m_y;

	position.m_x += velocity.m_x;
	position.m_y += velocity.m_y;

	cout << "lenght :" << lenght;

	cout << " pos.m_x: " << position.m_x;
	cout << " pos.m_y: " << position.m_y;

	cout << " vel.m_x: " << velocity.m_x;
	cout << " vel.m_y: " << velocity.m_y;

	cout << " acc.m_x: " << acceleration.m_x;
	cout << " acc.m_y: " << acceleration.m_y;

	cout << "\n";

	//if (position.m_x < 0) position.m_x = 0;
	//else if (position.m_x > Graphics::ScreenWidth - 1) position.m_x = Graphics::ScreenWidth - 1;
	//if (position.m_y < 0) position.m_y = 0;
	//else if (position.m_y > Graphics::ScreenWidth - 1) position.m_y = Graphics::ScreenHeight - 1;

	//std::this_thread::sleep_for(250ms);
}


void Game::ComposeFrame()
{
	

	Trect<double>a(origin.m_x - 5, origin.m_y - 5, origin.m_x + 5, origin.m_y + 5);
	a.draw(gfx, Colors::Gold);

	if (lenght > 2000)
		return;

	if (lenght < rest_lenght)
		gfx.draw_line_s(origin.m_x, origin.m_y, position.m_x, position.m_y, Colors::Red);
	else
		gfx.draw_line_s(origin.m_x, origin.m_y, position.m_x, position.m_y, Colors::Blue);

	gfx.draw_circle_s(position.m_x, position.m_y, 5, Colors::Black);
}
