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
	spring2d(double _rest_length = 20, double _k = 0.1)
		:
		rest_length(_rest_length),
		k(_k)
	{
	}
	~spring2d(void)
	{
	}

	void connect(vector<spring_node &> _a, vector<spring_node &> _b)
	{
		a = _a;
		b = _b;
	}

	Cvector2<double> calculate_force(void) const
	{
		Cvector2<double> result_force(0, 0);
		for (int i = 0; i < a.size(); ++i)
		{
			for (int j = 0; j < b.size(); ++j)
			{
				Cvector2<double> force = a[i].position - b[j].position;
				double distance = force.magnitude();
				double stretch = distance - rest_length;
				force.normalize();
				force *= -k * (stretch);
				
				result_force += force;
			}
		}
		return result_force;
	}

	void draw(Graphics & gfx) const
	{
		for (int i = 0; i < a.size(); ++i)
		{
			for (int j = 0; j < b.size(); ++j)
			{
				double l = (a[i].position - b[j].position).magnitude();
				if (l > 2000)
					return;
				if (l < rest_length)
					gfx.draw_line_s(a[i].position.m_x, a[i].position.m_y, b[j].position.m_x, b[j].position.m_y, Colors::Red);
				else
					gfx.draw_line_s(a[i].position.m_x, a[i].position.m_y, b[j].position.m_x, b[j].position.m_y, Colors::Blue);
			}
		}
		
	}
private:
	vector<spring_node &> a;
	vector<spring_node &> b;
	double rest_length;
	double k;
};

//vector<spring_node> spring_nodes;
//vector<spring2d> springs;
int g_count = 0;

vector<vector<spring_node>> nodes;
vector<vector<spring2d>> springs;
Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	for (int i = 0; i <  20; ++i)
	{
		nodes.push_back(vector<spring_node>());
		for (int j = 0; j < 20; ++j)
		{
			nodes[i].push_back(spring_node());
			nodes[i][j].position = Cvector2<double>(50 + 20.1 * i, 15 + 25.2*j);
			nodes[i][j].velocity = Cvector2<double>(0.0, 0.0);
			nodes[i][j].unlock();
		}
	}
	//for (int i = 0; i < nodes.size() - 1; ++i)
	//{
	//	springs.push_back(vector<spring2d>());
	//	for (int j = 0; j < nodes[i].size() - 1; ++j)
	//	{
	//		springs[i].push_back(spring2d(nodes[i][j], nodes[i + 1][j]));
	//		springs[i].push_back(spring2d(nodes[i][j], nodes[i][j+1]));
	//	}
	//}
		
	for (int i = 0; i < nodes[0].size(); ++i)
		nodes[0][i].lock();
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
	vector<vector<Cvector2<double>>> forces;
	for (int i = 0; i < springs.size(); ++i)
	{
		forces.push_back(vector<Cvector2<double>>());
		for (int j = 0; j < springs[i].size(); ++j)
			forces[i].push_back(springs[i][j].calculate_force());
	
		//cout << "i: " << i << ", force = (" << forces[i].m_x << ", " << forces[i].m_y << ")";
		//cout << " a.pos = (" << spring_nodes[i].position.m_x << ", " << spring_nodes[i].position.m_y <<
		//	"), a.vel = (" << spring_nodes[i].velocity.m_x << ", " << spring_nodes[i].velocity.m_y << ")";
		//cout << "\n";
	}
	for (int i = 0; i < springs.size()-1; ++i)
	{
		for (int j = 0; j < springs[j].size()-1; ++j)
		{
			//nodes[i][j].apply_force(forces[i][j]);
			//forces[i][j] *= (-1);
			//nodes[i + 1][j].apply_force(forces[i][j]);
			//nodes[i][j+1].apply_force(forces[i][j]);
		}
	}
	//cout << string(50, '*') << "\n";

	// apply gravity
	for (auto & i : nodes)
		for (auto & j : i)
			j.apply_force({ 0.0, 1 });

	//std::this_thread::sleep_for(50ms);
}


void Game::ComposeFrame()
{
	for (const auto & i : springs)
		for (const auto & j : i)
			j.draw(gfx);

	for (auto & i : nodes)
		for (auto & j : i)
			j.draw(gfx);
}
