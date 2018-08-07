#include "Game.h"

constexpr double dmax = std::numeric_limits<double>::max();
constexpr double dmin = std::numeric_limits<double>::lowest();

inline ostream & nl(ostream & os) { return os << "\n"; }

class Surface
{
public:
	Surface(const Color & _c)
	{
		std::fill_n(c, 32 * 32, _c);
	}

	Surface(const Surface & src)
	{
		std::copy_n(src.c, 32 * 32, c);
	}

	~Surface(void) {}

	Color const * get_data(void) const
	{
		return c;
	}
private:
	Color c [32*32];
};


QuadTree tree;
vector<Surface> tile_map;
vector<Color> rainbow;
Random rnd;
double dx = 0.1;
double dy = 0.1;

double camx = 0;
double camy = 0;

class Camera
{
public:
	Camera(void)
	{
		m_x = 0;
		m_y = 0;
		m_width = Graphics::ScreenWidth;
		m_height = Graphics::ScreenHeight;
	}

private:
	double m_x; // current position of the camera
	double m_y; // current position of the camera

	int m_width;  // viewport size = Graphics::ScreenWidth
	int m_height; // viewport size = Graphics::ScreenHeight
};

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	rainbow = generate_colors(Colors::Red, Colors::Blue, 20);
	for (int i = 0; i < rainbow.size(); ++i)
	{
		tile_map.push_back(Surface(rainbow[i]));
	}
	tree.insert(Node(0, 0, &World_Origin));
	//for (int i = 0; i < 100000; ++i)
	//{
	//	int x = rnd.next_double(-1000, 1000);
	//	int y = rnd.next_double(-1000, 1000);
	//	unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
	//	tree.insert(Node(x, y, &tile_map[idx]));
	//}
	for (int i = 0; i < 32; ++i)
	{
		for (int j = 0; j < 32; j++)
		{
			int x = i;
			int y = j;
			unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
			tree.insert(Node(x, y, &tile_map[idx]));
		}
	}
}

int framecount = 0;
double time_concatenated = 0.0;

void Game::Go()
{
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		wnd.Kill();

	CTimer t;
	gfx.BeginFrame();

	HandleInput();
	UpdateModel();
	ComposeFrame();

	gfx.EndFrame();

	double elapsed = t.elapsed() + 1e-3; // to avoid infinities
	time_concatenated += elapsed;
	framecount++;
	cout << 1000.0 / elapsed << " AVG= " << 1000 / (time_concatenated / framecount) << " ... " << tree.size() << endl;
}

void Game::HandleInput()
{
	double speed = 1.5e1;
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		camy -= speed;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		camy += speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		camx -= speed;
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		camx += speed;
	}

	if (wnd.mouse.LeftIsPressed())
	{
		int x = wnd.mouse.GetPosX() - camx;
		int y = wnd.mouse.GetPosY() - camy;
		unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
		tree.insert(Node(x, y, &tile_map[idx]));
	}

	if (wnd.mouse.LeftIsPressed() && wnd.kbd.KeyIsPressed(VK_CONTROL))
	{
		for (int i = 0; i < 1000; ++i)
		{
			int x = wnd.mouse.GetPosX() + rnd.next(-50, 50) - camx;
			int y = wnd.mouse.GetPosY() + rnd.next(-50, 50) - camy;
			unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
			tree.insert(Node(x, y, &tile_map[idx]));
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		for (int i = 0; i < 100000; ++i)
		{
			int x = rnd.next_double(-10000, 10000);
			int y = rnd.next_double(-10000, 10000);
			unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
			tree.insert(Node(x, y, &tile_map[idx]));
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_DELETE))
	{
		tree.clear();
		tile_map.clear();
		for (int i = 0; i < rainbow.size(); ++i)
		{
			tile_map.push_back(Surface(rainbow[i]));
		}
	}
}

void Game::UpdateModel()
{
	//for (int i = (0 + camx) / 32; i < (1024 + camx) / 32; ++i)
	//{
	//	for (int j = (0 + camy) / 32; j < (1024 + camy) / 32; ++j)
	//	{
	//		int x = i;
	//		int y = j;
	//		unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
	//		tree.insert(Node(x, y, &tile_map[idx]));
	//	}
	//}
}

void Game::ComposeFrame()
{
	//tree.Draw(gfx, camx, camy);

	//Trect<double> screen_range = {
	//	{ 0 - camx, 0 - camy },
	//	{ Graphics::ScreenWidth - camx, Graphics::ScreenHeight - camy } };
	//Trect<double> screen_range = {
	//	{ (0 - camx) / 32, (0 - camy)/32 },
	//{ (Graphics::ScreenWidth - camx)/32, (Graphics::ScreenHeight - camy)/32 } };
	//
	//vector<const Node *> vec = tree.range(screen_range);
	//for (const auto & i : vec)
	//{
	//	if (!(
	//		(i->m_x + camx) * 32 < 32 || (i->m_x + camx) * 32 >= Graphics::ScreenWidth  - 32||
	//		(i->m_y + camy) * 32 < 32 || (i->m_y + camy) * 32 >= Graphics::ScreenHeight - 32))
	//	{
	//		//gfx.PutPixel(i->m_x + camx, i->m_y + camy, i->m_tile_data->get_data()[0]);
	//
	//		//gfx.PutPixel((i->m_x + camx)*32, (i->m_y + camy)*32, i->m_tile_data->get_data()[0]);
	//
	//		gfx.draw_rect((i->m_x + camx) * 32, (i->m_y + camy) * 32, 32, 32, i->m_tile_data->get_data()[0]);
	//	
	//	}
	//}

	// performance test:
	//for (int i = 0; i < 1024; ++i)
	//	for (int j = 0; j < 1024; ++j)
	//		gfx.PutPixel(i, j, Colors::Blue); // cca 26 FPS
	//gfx.draw_rect(0, 0, 1024, 1024, Colors::Blue); // cca 83 FPS

	//camx = -200;
	//camy = 0;

	double tile_size = 32;

	// offset screen by cam
	Trect<double> screen({ 0 + camx,0 + camy }, { 1024 + camx, 1024 + camy });

	// scale screen up to include clipped tiles
	//screen.m_upleft.m_x -= tile_size;
	//screen.m_upleft.m_y -= tile_size;
	//screen.m_downright.m_x += tile_size;
	//screen.m_downright.m_y += tile_size;

	// scale screen to pick up only tile_size'th od the pixels
	screen.m_upleft.m_x /= tile_size;
	screen.m_upleft.m_y /= tile_size;
	screen.m_downright.m_x /= tile_size;
	screen.m_downright.m_y /= tile_size;

	CTimer mtimer;
	vector<const Node *> vec = tree.range(screen);
	double elapsed = mtimer.elapsed();
	// cout << screen.m_upleft.m_x << ", " << screen.m_upleft.m_y << " - " << screen.m_downright.m_x << ", " << screen.m_downright.m_y << endl;
	//cout << camx << ", " << camy << " - " << vec.size() << endl;
	cout << vec.size() << " - took me " << elapsed << " ms  ";
	for (const auto & i : vec)
	{
		int x = i->m_x * tile_size - camx;
		int y = i->m_y * tile_size - camy;

		if (x >= 0 && x + tile_size < Graphics::ScreenWidth  &&
			y >= 0 && y + tile_size < Graphics::ScreenHeight)
			//gfx.draw_rect(x, y, tile_size, tile_size, i->m_tile_data->get_data()[0]);
			gfx.draw_bitmap(x, y, *(i->m_tile_data));
	}


}
