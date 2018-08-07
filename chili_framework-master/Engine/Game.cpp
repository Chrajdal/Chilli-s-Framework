#include "Game.h"

constexpr double dmax = std::numeric_limits<double>::max();
constexpr double dmin = std::numeric_limits<double>::lowest();

inline ostream & nl(ostream & os) { return os << "\n"; }

QuadTree tree;
Camera cam;
vector<Surface> tile_map;
vector<Color> rainbow;
Random rnd;
double dx = 0.1;
double dy = 0.1;

int framecount = 0;
double time_concatenated = 0.0;

Bitmap tile_sheet("32x32_map_tile v3.1.bmp");
Surface a(&tile_sheet, 0, 32, 32, 32);

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	tree.insert(Node(0, 0, &a));

	for (int i = 0; i < 23; ++i)
	{
		for (int j = 1; j < 29; ++j)
		{
			if (i < 19 && j < 29 - 13)
				tile_map.push_back(Surface(&tile_sheet, i * 32, j * 32, 32, 32));
		}
	}

	//for (int i = 0; i < 100000; ++i)
	//{
	//	int x = rnd.next_double(-1000, 1000);
	//	int y = rnd.next_double(-1000, 1000);
	//	unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
	//	tree.insert(Node(x, y, &tile_map[idx]));
	//}

	//for (int i = 0; i < 32; ++i)
	//{
	//	for (int j = 0; j < 32; j++)
	//	{
	//		int x = i;
	//		int y = j;
	//		unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
	//		tree.insert(Node(x, y, &tile_map[idx]));
	//	}
	//}
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

	if (wnd.mouse.LeftIsPressed())
	{
		int x = wnd.mouse.GetPosX() - cam.m_x;
		int y = wnd.mouse.GetPosY() - cam.m_y;
		unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
		//tree.insert(Node(x, y, &tile_map[idx]));
	}

	if (wnd.mouse.LeftIsPressed() && wnd.kbd.KeyIsPressed(VK_CONTROL))
	{
		for (int i = 0; i < 1000; ++i)
		{
			int x = wnd.mouse.GetPosX() + rnd.next(-50, 50) - cam.m_x;
			int y = wnd.mouse.GetPosY() + rnd.next(-50, 50) - cam.m_y;
			unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
			//tree.insert(Node(x, y, &tile_map[idx]));
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		for (int i = 0; i < 100000; ++i)
		{
			int x = rnd.next_double(-1000, 1000);
			int y = rnd.next_double(-1000, 1000);
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
			//tile_map.push_back(Surface(rainbow[i]));
		}
	}
}

void Game::UpdateModel()
{
	for (int i = (300 + 0 + cam.m_x) / 32; i < (1024 + cam.m_x - 300) / 32; ++i)
	{
		for (int j = ( 300 + 0 + cam.m_y) / 32; j < (1024 + cam.m_y - 300) / 32; ++j)
		{
			int x = i;
			int y = j;
			unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * tile_map.size());
			tree.insert(Node(x, y, &tile_map[idx]));
		}
	}
}

void Game::ComposeFrame()
{
	//tree.Draw(gfx, camx, camy);

	double tile_size = 32;

	// offset screen by cam
	Trect<double> screen({ 0 + cam.m_x,0 + cam.m_y }, { 1024 + cam.m_x, 1024 + cam.m_y });

	// scale screen to pick up only tile_size'th od the pixels
	screen.m_upleft.m_x /= tile_size;
	screen.m_upleft.m_y /= tile_size;
	screen.m_downright.m_x /= tile_size;
	screen.m_downright.m_y /= tile_size;

	CTimer mtimer;
	vector<const Node *> vec = tree.range(screen);
	double elapsed = mtimer.elapsed();
	// cout << vec.size() << " - took me " << elapsed << " ms  ";
	for (const auto & i : vec)
	{
		int x = i->m_x * tile_size - cam.m_x;
		int y = i->m_y * tile_size - cam.m_y;

		if (x >= 0 && x + tile_size < Graphics::ScreenWidth  &&
			y >= 0 && y + tile_size < Graphics::ScreenHeight)
			//gfx.draw_rect(x, y, tile_size, tile_size, i->m_tile_data->get_data()[0]);
			//gfx.draw_bitmap(x, y, *(i->m_tile_data));
			gfx.draw_surface(x, y, *i->m_tile_data);
	}

}
