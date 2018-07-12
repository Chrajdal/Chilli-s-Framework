#include "Game.h"

constexpr int    N = 1500;
constexpr double G = 6.673e-10;
constexpr double TIMESTAMP = 1e11;
constexpr double max_mass = 1e-10;
constexpr double min_mass = 1e-15;
constexpr double max_speed = 1e-9;
constexpr double min_speed = -1e-9;

double g_max_mass = std::numeric_limits<double>::min();
double g_min_mass = std::numeric_limits<double>::max();

const double pi = std::acos(-1);

vector<Color> gradient;

class particle
{
public:
	particle(double rx, double ry, double vx, double vy, double fx, double fy, double mass)
		:rx(rx), ry(ry),
		vx(vx), vy(vy),
		fx(fx), fy(fy),
		mass(mass)
	{}

	particle(const particle & src)
		:rx(src.rx), ry(src.ry),
		vx(src.vx), vy(src.vy),
		fx(src.fx), fy(src.fy),
		mass(src.mass)
	{}

	particle & operator = (const particle & src)
	{
		if (this == &src)
			return *this;

		rx = src.rx;
		ry = src.ry;
		vx = src.vx;
		vy = src.vy;
		fx = src.fx;
		fy = src.fy;
		mass = src.mass;

		return *this;
	}
	
	void update(double timestamp)
	{
		vx += timestamp * fx / mass;
		vy += timestamp * fy / mass;

		if (vx < min_speed)
			vx = min_speed;
		if (vx > max_speed)
			vx = max_speed;
		if (vy < min_speed)
			vy = min_speed;
		if (vy > max_speed)
			vy = max_speed;

		rx += timestamp * vx;
		ry += timestamp * vy;
	}
	
	void draw(Graphics & gfx) const
	{
		if (rx > 10 && rx < Graphics::ScreenWidth - 10 && ry > 10 && ry < Graphics::ScreenHeight - 10)
		{
			double gradient_step = (g_max_mass - g_min_mass) / gradient.size();
			int grad_index = (int)std::floor(mass / gradient_step);
			Color c = gradient[grad_index];
			gfx.draw_circle(rx, ry, 5, c);
		}
	}
	void resetForce()
	{
		fx = 0.0;
		fy = 0.0;
	}
	void addForce(particle & b)
	{
		double EPS = 3E3;      // softening parameter (just to avoid infinities)
		double dx = b.rx - rx;
		double dy = b.ry - ry;
		double dist = sqrt(dx*dx + dy * dy);

		double F = (G * mass * b.mass) / (dist*dist + EPS * EPS);
		fx += F * dx / dist;
		fy += F * dy / dist;

		b.fx -= F * dx / dist;
		b.fy -= F * dy / dist;
	}

	double rx, ry;//position components
	double vx, vy;//velocity components
	double fx, fy;//force components
	double mass;//mass of the particle

};

std::vector<particle> particles;

double rand_double(double min, double max)
{
	return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

Tpoint<double> random_pos(void)
{
	double angle = rand_double(0.0, 1.0) * pi * 2;
	double radius = rand_double(0.0, 500);
	return Tpoint<double> {std::cos(angle)*radius + Graphics::ScreenWidth / 2, std::sin(angle)*radius + Graphics::ScreenHeight/ 2};
}

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));
	
	for (int x = 0, y = 0; x < 255; ++x, ++y)
	{
		int red = x - 255;
		int green = 255 - x;
		int blue = y - x;

		gradient.push_back(Colors::MakeRGB(red, green, blue));
	}

	particles.reserve(N);

	for (int i = 0; i < N; i++)
	{
		Tpoint<double> rnd_pos = random_pos();
		particles.emplace_back(
			rnd_pos.m_x, rnd_pos.m_y, // position
			rand_double(-1e-13, 1e-13), rand_double(-1e-13, 1e-13), // velocity
			rand_double(-1e-13, 1e-10), rand_double(-1e-13, 1e-10), // force
			rand_double(min_mass, max_mass)); //mass
	
		//rnd_pos = random_pos();
		//particles.emplace_back(
		//	rnd_pos.m_x + 500, rnd_pos.m_y, // position
		//	rand_double(-1e-13, 1e-13), rand_double(-1e-13, 1e-13), // velocity
		//	rand_double(-1e-13, 1e-10), rand_double(-1e-13, 1e-10), // force
		//	rand_double(min_mass, max_mass)); //mass
		//
		//rnd_pos = random_pos();
		//particles.emplace_back(
		//	rnd_pos.m_x, rnd_pos.m_y - 500, // position
		//	rand_double(-1e-13, 1e-13), rand_double(-1e-13, 1e-13), // velocity
		//	rand_double(-1e-13, 1e-10), rand_double(-1e-13, 1e-10), // force
		//	rand_double(min_mass, max_mass)); //mass
		//
		//rnd_pos = random_pos();
		//particles.emplace_back(
		//	rnd_pos.m_x, rnd_pos.m_y + 500, // position
		//	rand_double(-1e-13, 1e-13), rand_double(-1e-13, 1e-13), // velocity
		//	rand_double(-1e-13, 1e-10), rand_double(-1e-13, 1e-10), // force
		//	rand_double(min_mass, max_mass)); //mass
	}
	//Tpoint<double> rnd_pos = random_pos();
	//particles.emplace_back(
	//	rnd_pos.m_x, rnd_pos.m_y, // position
	//	rand_double(-1e-13, 1e-13), rand_double(-1e-13, 1e-13), // velocity
	//	rand_double(-1e-13, 1e-10), rand_double(-1e-13, 1e-10), // force
	//	rand_double(min_mass, max_mass)); //mass

	particles[0].mass = max_mass;
}

void Game::Go()
{
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		wnd.Kill();
	gfx.BeginFrame();


	g_max_mass = std::numeric_limits<double>::min();
	g_min_mass = std::numeric_limits<double>::max();
	for (const auto & i : particles)
	{
		if (i.mass > g_max_mass)
		{
			g_max_mass = i.mass;
		}
		if (i.mass < g_min_mass)
		{
			g_min_mass = i.mass;
		}
	}
	
	UpdateModel();
	ComposeFrame();
	
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	double epsilon = 1e0;
	for (int i = 0; i < particles.size(); ++i)
	{
		for (int j = i + 1; j < particles.size(); ++j)
		{
			if (sq_distance(Tpoint<double>{ particles[i].rx , particles[i].ry}, Tpoint<double>{ particles[j].rx, particles[j].ry  }) < epsilon * epsilon)
			{
				// p = m * v					
				if (particles[i].mass > particles[j].mass)
				{
					particles[i].vx += -(particles[j].vx * particles[j].mass) / particles[i].mass;
					particles[i].vy += -(particles[j].vy * particles[j].mass) / particles[i].mass;
					particles[i].mass += particles[j].mass;
				
					particles.erase(particles.begin() + j);
				}
				if (particles[i].mass <= particles[j].mass)
				{
					particles[j].vx += -(particles[i].vx * particles[i].mass) / particles[j].mass;
					particles[j].vy += -(particles[i].vy * particles[i].mass) / particles[j].mass;
					particles[j].mass += particles[i].mass;
				
					particles.erase(particles.begin() + i);
				}
				//particles[i].mass += particles[j].mass;
				//particles.erase(particles.begin() + j);
			}
		}
	}


	
	for (int i = 0; i < particles.size(); ++i)
	{
		particles[i].resetForce();
		for (int j = 0; j < particles.size(); ++j)
			if (i != j)
				particles[i].addForce(particles[j]);
	}
	for (auto & i : particles)
		i.update(TIMESTAMP);
	
	if (!particles.empty())
	{
		particles[0].rx = 500;
		particles[0].ry = 500;
	}

	if (wnd.mouse.LeftIsPressed())
	{
		particles.push_back(particle(
			wnd.mouse.GetPosX(), wnd.mouse.GetPosY(), // position
			rand_double(-1e-13, 1e-13), rand_double(-1e-13, 1e-13), // velocity
			rand_double(-1e-13, 1e-10), rand_double(-1e-13, 1e-10), // force
			rand_double(min_mass, max_mass))); //mass
	}


	if (wnd.mouse.RightIsPressed())
	{
		for(int i = 0; i < 100; ++i)
		particles.push_back(particle(
			wnd.mouse.GetPosX() + rand_double(-10, 10), wnd.mouse.GetPosY() + rand_double(-10, 10), // position
			0, 0, // velocity
			rand_double(-1e-13, 1e-10), rand_double(-1e-13, 1e-10), // force
			rand_double(min_mass, max_mass))); //mass
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		g_max_mass = std::numeric_limits<double>::min();
		g_min_mass = std::numeric_limits<double>::max();
		particles.clear();
	}

	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		for (int i = 0; i < 200; ++i)
		{

			Tpoint<double> rnd_pos = random_pos();
			particles.emplace_back(
				rnd_pos.m_x, rnd_pos.m_y, // position
				rand_double(-1e-13, 1e-13), rand_double(-1e-13, 1e-13), // velocity
				rand_double(-1e-13, 1e-10), rand_double(-1e-13, 1e-10), // force
				rand_double(min_mass, max_mass)); //mass
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		for (auto & i : particles)
		{
			i.vx = 0.0;
			i.vy = 0.0;
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_CONTROL))
		if (particles.size() > 10)
			for (int i = 0; i < 10; ++i)
				particles.erase(particles.begin() + rand() % particles.size());

}

void Game::ComposeFrame()
{
	for (const auto & i : particles)
		i.draw(gfx);
	//this_thread::sleep_for(250ms);
	
	cout << "n: " <<  particles.size() << ", max_mass = " << g_max_mass << ", min_mass = " << g_min_mass << "\n";
}
