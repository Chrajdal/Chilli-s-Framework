#include "Game.h"
Random rnd;

//constexpr double g = 0;
//const double m_pi = std::acos(-1);
//constexpr double stiffness = 25;
//constexpr double rest_lenght = 200;
//constexpr double mass = 0.5;
//constexpr double damping = 0.99;
//constexpr double eps = 1e-5; // just to avoid infinities
//
//Cvector2<double> origin(500, 300);
//Cvector2<double> position(origin.m_x, origin.m_y + rnd.next(2,1000));
//Cvector2<double> velocity(0.0, 0);
//Cvector2<double> acceleration(0, 0);
//double displacement = 0;
//double lenght = 0;
//double friction = 0.9;
//
bool keepUpdating = true;

class spring_node
{
public:
	void apply_force(const Cvector2<double> & force)
	{
		if (locked) return;
		velocity += force;
		velocity *= 0.85;
		position += velocity;
	}

	inline void lock(void) { locked = true; }
	inline void unlock(void) { locked = false; }


	void draw(Graphics & gfx) const
	{
		gfx.draw_circle_filled_s(position.m_x, position.m_y, 5, Colors::Blue);
	}

	Cvector2<double> position;
	Cvector2<double> velocity;

	bool locked;
};

class spring2d
{
public:
	spring2d(spring_node &_a, spring_node & _b, double _rest_length = 20, double _k = 0.15)
		:
		a(_a), b(_b),
		rest_length(_rest_length),
		k(_k)
	{
	}
	~spring2d(void)
	{
	}

	void connect(spring_node & _a, spring_node & _b)
	{
		a = _a;
		b = _b;
	}

	Cvector2<double> calculate_force(void) const
	{
		Cvector2<double> force = a.position - b.position;
		double distance = force.magnitude();
		double stretch = distance - rest_length;

		force *= -k * (stretch);
		force.normalize();

		//force += (force / distance) * intensity - ((b.velocity - a.velocity) * 40000);

		return force;
	}

	void draw(Graphics & gfx) const
	{
		double l = (a.position - b.position).magnitude();
		if (l > 2000)
			return;
		if (l < rest_length)
			gfx.draw_line_s(a.position.m_x, a.position.m_y, b.position.m_x, b.position.m_y, Colors::Red);
		else
			gfx.draw_line_s(a.position.m_x, a.position.m_y, b.position.m_x, b.position.m_y, Colors::Blue);
	}
private:
	spring_node & a;
	spring_node & b;
	double rest_length;
	double k;
};

vector<spring_node> spring_nodes;
vector<spring2d> springs;
int g_count = 0;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	for (int i = 0; i < 25; ++i)
	{
		spring_nodes.push_back(spring_node());
		spring_nodes[i].position = Cvector2<double>(20 + 20.05 * i, 150);
		spring_nodes[i].velocity = Cvector2<double>(0.0, 0.0);
		spring_nodes[i].unlock();
	}
	for (int i = 0; i < spring_nodes.size() - 1; ++i)
		springs.push_back(spring2d(spring_nodes[i], spring_nodes[i + 1]));
	
	spring_nodes[0].lock();
	spring_nodes[spring_nodes.size() - 1].lock();
	spring_nodes[spring_nodes.size()/2].lock();
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
	g_count++;
	gfx.EndFrame();
}

void Game::HandleInput()
{
	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		keepUpdating = false;
		return;
	}
}

void Game::UpdateModel()
{
	// apply string force
	//cout << "g_count: " << g_count << "\n";
	vector<Cvector2<double>> forces;
	for (int i = 0; i < springs.size(); ++i)
	{
		forces.push_back(springs[i].calculate_force());
	
		//cout << "i: " << i << ", force = (" << forces[i].m_x << ", " << forces[i].m_y << ")";
		//cout << " a.pos = (" << spring_nodes[i].position.m_x << ", " << spring_nodes[i].position.m_y <<
		//	"), a.vel = (" << spring_nodes[i].velocity.m_x << ", " << spring_nodes[i].velocity.m_y << ")";
		//cout << "\n";
	}
	for (int i = 0; i < springs.size(); ++i)
	{
		spring_nodes[i].apply_force(forces[i]);
		forces[i] *= (-1);
		spring_nodes[i + 1].apply_force(forces[i]);
	}
	//cout << string(50, '*') << "\n";

	// apply gravity
	for (auto & i : spring_nodes)
		i.apply_force({ 0.0, 0.1 });

	//std::this_thread::sleep_for(50ms);
}


void Game::ComposeFrame()
{
	for (const auto & i : springs)
		i.draw(gfx);

	for (const auto & i : spring_nodes)
		i.draw(gfx);
}
