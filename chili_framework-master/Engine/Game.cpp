#include "Game.h"

inline ostream & nl(ostream & os) { return os << "\n"; }

QuadTree tree;
Camera cam;
Bitmap tile_sheet_dirt(".\\Assets\\grass.bmp");
//Bitmap tile_sheet_stone(".\\Assets\\stone.bmp");
vector<Surface> tile_map_dirt;
vector<Surface> tile_map_stone;
vector<Color> rainbow;
Random rnd;
double dx = 1e-1;
double dy = 1e-1;
int framecount = 0;
double time_concatenated = 0.0;
constexpr double dmax = std::numeric_limits<double>::max();
constexpr double dmin = std::numeric_limits<double>::lowest();
constexpr double tile_size = 16;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	// Load assets:
	for (int i = 0; i < tile_sheet_dirt.width / tile_size; ++i)
		for (int j = 0; j < tile_sheet_dirt.height / tile_size; ++j)
			tile_map_dirt.push_back(Surface(&tile_sheet_dirt, i * tile_size, j * tile_size, tile_size, tile_size));
	//for (int i = 0; i < tile_sheet_dirt.width / tile_size; ++i)
	//	for (int j = 0; j < tile_sheet_dirt.height / tile_size; ++j)
	//		tile_map_stone.push_back(Surface(&tile_sheet_stone, i * tile_size, j * tile_size, tile_size, tile_size));
	
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

	//if (wnd.kbd.KeyIsPressed(VK_SPACE))
	//{
	//	for (int i = 0; i < 100000; ++i)
	//	{
	//		int x = rnd.next_double(-10000, 10000);
	//		int y = rnd.next_double(-10000, 10000);
	//		unsigned idx = std::abs(perlin::noise(x * dx, y * dy) * (tile_map_dirt.size()));
	//		tree.insert(Node(x, y, &tile_map_dirt[idx]));
	//	}
	//}

	if (wnd.kbd.KeyIsPressed(VK_DELETE))
	{
		tree.clear();
		//tile_map_dirt.clear();
		//for (int i = 0; i < rainbow.size(); ++i)
		//{
		//	//tile_map_dirt.push_back(Surface(rainbow[i]));
		//}
	}
}

void Game::UpdateModel()
{	
	int tmp = -200;
	for (int i = (tmp + 0 + cam.m_x) / tile_size; i < (1024 + cam.m_x - tmp) / tile_size; ++i)
	{
		for (int j = (tmp + 0 + cam.m_y) / tile_size; j < (1024 + cam.m_y - tmp) / tile_size; ++j)
		{
			double height_treshold = perlin::noise(/*i * dx * 0.1, */ i * dx * 0.15) * 100;

			//cout << height_treshold << endl;

			int x = i;
			int y = j;
			//unsigned idx = 0;
			if (height_treshold > j)
			{
				tree.insert(Node(x, y, tile_type::air));
			}
			else if (height_treshold < j)
			{
				tree.insert(Node(x, y, tile_type::dirt));
			}
			else if (height_treshold * 5 < j)
			{
				tree.insert(Node(x, y, tile_type::stone));
			}
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
		{
			if (i->m_tile == tile_type::dirt)
			{
				//gfx.draw_surface(x, y, tile_map_dirt[5]);

				const Node * n = tree.at(i->m_x, i->m_y + 1);
				const Node * s = tree.at(i->m_x, i->m_y - 1);
				const Node * w = tree.at(i->m_x - 1, i->m_y);
				const Node * e = tree.at(i->m_x + 1, i->m_y);
				cout << string(50, '*') << endl;
				if (n == NULL)
					cout << "n is NULL" << endl;
				if (s == NULL)
					cout << "s is NULL" << endl;
				if (w == NULL)
					cout << "w is NULL" << endl;
				if (e == NULL)
					cout << "e is NULL" << endl;
				cout << string(50, '*') << endl;
				if (n != NULL && s != NULL && w != NULL && e != NULL)
				{
					if (n->m_tile == tile_type::air && s->m_tile == tile_type::air &&
						w->m_tile == tile_type::air && e->m_tile == tile_type::air)
						gfx.draw_surface(x, y, tile_map_dirt[15]);
					else if (
						n->m_tile == tile_type::dirt && s->m_tile == tile_type::dirt &&
						w->m_tile == tile_type::dirt && e->m_tile == tile_type::dirt)
						gfx.draw_surface(x, y, tile_map_dirt[5]);
					else if (
						n->m_tile == tile_type::air  && s->m_tile == tile_type::dirt &&
						w->m_tile == tile_type::dirt && e->m_tile == tile_type::dirt)
						gfx.draw_surface(x, y, tile_map_dirt[1]);
					else if (
						n->m_tile == tile_type::air  && s->m_tile == tile_type::air  &&
						w->m_tile == tile_type::dirt && e->m_tile == tile_type::dirt)
						gfx.draw_surface(x, y, tile_map_dirt[13]);
				}
			}
		}
	}

	// easy way out:
	gfx.draw_rect(0, 0, tile_size, Graphics::ScreenHeight, Colors::White);
	gfx.draw_rect(Graphics::ScreenWidth - tile_size, 0, tile_size, Graphics::ScreenHeight, Colors::White);
	gfx.draw_rect(0, 0, Graphics::ScreenWidth, tile_size, Colors::White);
	gfx.draw_rect(0, Graphics::ScreenHeight - tile_size, Graphics::ScreenWidth, tile_size, Colors::White);
}
