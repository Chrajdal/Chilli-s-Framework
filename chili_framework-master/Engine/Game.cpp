#include "Game.h"

inline ostream & nl(ostream & os) { return os << "\n"; }

QuadTree tree;
Camera cam;
vector<Surface> tile_map;
vector<Color> rainbow;
Random rnd;
Bitmap tile_sheet("32x32_map_tile v3.1.bmp");
double dx = 0.05;
double dy = 0.05;
int framecount = 0;
double time_concatenated = 0.0;
constexpr double dmax = std::numeric_limits<double>::max();
constexpr double dmin = std::numeric_limits<double>::lowest();
constexpr double tile_size = 32;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	//srand(unsigned(time(0)));

	for (int i = 0; i < tile_sheet.width / tile_size; ++i)
	{
		for (int j = 1; j < tile_sheet.height / tile_size; ++j)
		{
			if ((tile_sheet.data() + i * 32 + j * 32 * tile_sheet.width)->dword != (tile_sheet.data()->dword))
				tile_map.push_back(Surface(&tile_sheet, i * 32, j * 32, 32, 32));
		}
	}
	std::random_shuffle(std::begin(tile_map), std::end(tile_map));
}

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

	double elapsed = t.elapsed_ns() + 1e-9; // to avoid infinities
	time_concatenated += elapsed;
	framecount++;
	//cout << fixed << setw(3) << setprecision(3)
	//	 << 1e9 / elapsed << " AVG= " << 1e9 / (time_concatenated / framecount) << " ... " << tree.size() << endl;
}

void Game::HandleInput()
{
	double speed = 1e1;
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		cam.m_y -= speed;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		cam.m_y += speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		cam.m_x -= speed;
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		cam.m_x += speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		for (int i = 0; i < 100000; ++i)
		{
			int x = rnd.next_double(-10000, 10000);
			int y = rnd.next_double(-10000, 10000);
			unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * (tile_map.size()));
			tree.insert(Node(x, y, &tile_map[idx]));
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_DELETE))
	{
		tree.clear();
		//tile_map.clear();
		//for (int i = 0; i < rainbow.size(); ++i)
		//{
		//	//tile_map.push_back(Surface(rainbow[i]));
		//}
	}
}

void Game::UpdateModel()
{
	for (int i = (0 + cam.m_x) / 32; i < (1024 + cam.m_x) / 32; ++i)
	{
		for (int j = (0 + cam.m_y) / 32; j < (1024 + cam.m_y) / 32; ++j)
		{
			int x = i;
			int y = j;
			unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * (tile_map.size()));
			tree.insert(Node(x, y, &tile_map[idx]));
		}
	}
}

void Game::ComposeFrame()
{
	// offset screen by cam
	Trect<double> screen({ 0 + cam.m_x,0 + cam.m_y }, { 1024 + cam.m_x, 1024 + cam.m_y });

	// scale screen to pick up only tile_size'th od the pixels
	screen.m_upleft.m_x /= tile_size;
	screen.m_upleft.m_y /= tile_size;
	screen.m_downright.m_x /= tile_size;
	screen.m_downright.m_y /= tile_size;

	vector<const Node *> vec = tree.range(screen);
	for (const auto & i : vec)
	{
		int x = i->m_x * tile_size - cam.m_x;
		int y = i->m_y * tile_size - cam.m_y;

		if (x >= 0 && x + tile_size < Graphics::ScreenWidth  &&
			y >= 0 && y + tile_size < Graphics::ScreenHeight)
			gfx.draw_surface(x, y, *i->m_tile_data);
	}

	// easy way out:
	gfx.draw_rect(0, 0, 32, Graphics::ScreenHeight, Colors::White);
	gfx.draw_rect(Graphics::ScreenWidth - 32, 0, 32, Graphics::ScreenHeight, Colors::White);
	gfx.draw_rect(0, 0, Graphics::ScreenWidth, 32, Colors::White);
	gfx.draw_rect(0, Graphics::ScreenHeight - 32, Graphics::ScreenWidth, 32, Colors::White);
}
