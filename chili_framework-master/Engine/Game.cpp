#include "Game.h"
Random rnd;

constexpr double g = 0;
const double m_pi = std::acos(-1);
constexpr double stiffness = 25;
constexpr double rest_lenght = 200;
constexpr double mass = 0.5;
constexpr double damping = 0.99;
constexpr double eps = 1e-5; // just to avoid infinities

Cvector2<double> origin(500, 300);
Cvector2<double> position(origin.m_x, origin.m_y + rnd.next(2,1000));
Cvector2<double> velocity(0.0, 0);
Cvector2<double> acceleration(0, 0);
double displacement = 0;
double lenght = 0;
double friction = 0.9;

bool keepUpdating = true;

class spring_node
{
public:
	void apply_force(const Cvector2<double> & force)
	{
		velocity += force;
		position += velocity;
	}

	void draw(Graphics & gfx)
	{
		gfx.draw_circle_filled_s(position.m_x, position.m_y, 5, Colors::Blue);
	}

	Cvector2<double> position;
	Cvector2<double> velocity;
};

class spring2d
{
public:
	spring2d(double _rest_length = 100, double _k = 0.1)
		:
		rest_length(_rest_length),
		k(_k)
	{
	}
	~spring2d(void)
	{
	}

	void connect(std::shared_ptr<spring_node> &_a, std::shared_ptr<spring_node> & _b)
	{
		a = _a;
		b = _b;
	}

	Cvector2<double> calculate_force(void) const
	{
		Cvector2<double> force = a->position - b->position;
		double distance = force.magnitude();
		double stretch = distance - rest_lenght;

		force.normalize();

		force *= (-k * stretch);

		return force;
	}

	void draw(Graphics & gfx)
	{
		double l = (a->position - b->position).magnitude();
		if (l < 2000 && l < rest_length)
			gfx.draw_line_s(origin.m_x, origin.m_y, position.m_x, position.m_y, Colors::Red);
		else
			gfx.draw_line_s(origin.m_x, origin.m_y, position.m_x, position.m_y, Colors::Blue);
	}
private:
	std::shared_ptr<spring_node> a;
	std::shared_ptr<spring_node> b;
	double rest_length;
	double k;
};

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));
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

		friction = 1.0;
		velocity = {0,0};
		acceleration = {0,0};

		keepUpdating = false;
		return;
	}

	if (wnd.kbd.KeyIsPressed(0x52))
	{
		origin = { 500, 300 };
		position = { origin.m_x + rnd.next(-50,50), origin.m_y + rnd.next(-500, 500) };
		velocity ={ rnd.next_double(-10,10), rnd.next_double(-10,10) };
		acceleration = { 0, 0 };
		displacement = 0;
		lenght = 0;
		friction = 1.0;

		keepUpdating = true;
	}
	
	if (!wnd.mouse.LeftIsPressed() && wnd.mouse.IsInWindow())
	{
		keepUpdating = true;
		return;
	}

}

void Game::UpdateModel()
{
	lenght = std::sqrt(sq_distance(origin, position));
	displacement = lenght - rest_lenght;
	
	double koverm_x;
	double koverm_y = koverm_x = (stiffness / (mass + eps));
	
	//double sin_x = ((position.m_x - origin.m_x) / (lenght + eps));
	//double cos_x = ((position.m_y - origin.m_y) / (lenght + eps));

	double theta = 1;
	double sin_x = std::sin(theta);
	double cos_x = std::cos(theta);

	double tmp_x = koverm_x * displacement * sin_x;
	double tmp_y = koverm_y * displacement * cos_x;

	double damping_x = (damping / (mass + eps)) * velocity.m_x;
	double damping_y = (damping / (mass + eps)) * velocity.m_y;

	acceleration.m_x = - tmp_x - damping_x;
	acceleration.m_y = g - tmp_y - damping_y;

	acceleration.m_x /= std::sqrt(acceleration.m_x*acceleration.m_x + acceleration.m_y * acceleration.m_y);
	acceleration.m_y /= std::sqrt(acceleration.m_x*acceleration.m_x + acceleration.m_y * acceleration.m_y);

	velocity.m_x += acceleration.m_x;
	velocity.m_y += acceleration.m_y;

	//velocity.m_x *= friction;
	//velocity.m_y *= friction;

	//if (std::abs(velocity.m_x) < 1e-5 && std::abs(acceleration.m_x) < 1) velocity.m_x = 0;
	//if (std::abs(velocity.m_y) < 1e-5 && std::abs(acceleration.m_x) < 1) velocity.m_y = 0;
	
	position.m_x += velocity.m_x;
	position.m_y += velocity.m_y;
	
	//position.m_x = std::floor(position.m_x);
	//position.m_y = std::floor(position.m_y);

	cout << "lenght :" << lenght;
	cout << " displacement :" << displacement;

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
	
	gfx.draw_circle_filled_s(position.m_x, position.m_y, 5, Colors::Blue);

}
