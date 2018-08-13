#include "Game.h"

inline ostream & nl(ostream & os) { return os << "\n"; }

QuadTree tree;
Camera cam;
Bitmap tile_sheet_dirt(".\\Assets\\grass.bmp");
Bitmap tile_sheet_stone(".\\Assets\\stone.bmp");
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
tile_type selected_tile = tile_type::dirt;


Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	// Load assets:
	for (int j = 0; j < tile_sheet_dirt.height / tile_size; ++j)
	{
		for (int i = 0; i < tile_sheet_dirt.width / tile_size; ++i)
		{
			tile_map_dirt.push_back(Surface(&tile_sheet_dirt, i * tile_size, j * tile_size, tile_size, tile_size));
		}
	}
	
	for (int j = 0; j < tile_sheet_dirt.height / tile_size; ++j)
	{
		for (int i = 0; i < tile_sheet_dirt.width / tile_size; ++i)
		{
			tile_map_stone.push_back(Surface(&tile_sheet_stone, i * tile_size, j * tile_size, tile_size, tile_size));
		}
	}
	
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
	cout << fixed << setw(3) << setprecision(3)
		 << 1e9 / elapsed << " AVG= " << 1e9 / (time_concatenated / framecount) << " ... " << tree.size() << endl;
}

void Game::HandleInput()
{
	double speed = 1e1;
	if (wnd.kbd.KeyIsPressed(VK_UP) || wnd.kbd.KeyIsPressed(0x57))
	{
		cam.m_y -= speed;
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN) || wnd.kbd.KeyIsPressed(0x53))
	{
		cam.m_y += speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_LEFT) || wnd.kbd.KeyIsPressed(0x41))
	{
		cam.m_x -= speed;
	}
	if (wnd.kbd.KeyIsPressed(VK_RIGHT) || wnd.kbd.KeyIsPressed(0x44))
	{
		cam.m_x += speed;
	}

	if (wnd.kbd.KeyIsPressed(0x31))
	{
		selected_tile = tile_type::dirt;
	}
	if (wnd.kbd.KeyIsPressed(0x32))
	{
		selected_tile = tile_type::stone;
	}

	if (wnd.mouse.IsInWindow())
	{
		double x = wnd.mouse.GetPosX();
		double y = wnd.mouse.GetPosY();

		x += cam.m_x;
		y += cam.m_y;

		x /= tile_size;
		y /= tile_size;

		if (wnd.mouse.LeftIsPressed())
		{
			Node * tmp = tree.access(x, y);
			tmp->m_tile = tile_type::air;
		}
		if (wnd.mouse.RightIsPressed())
		{
			Node * tmp = tree.access(x, y);
			tmp->m_tile = selected_tile;
		}
	}
}

void Game::UpdateModel()
{	
	int tmp = -200;
	for (int i = (tmp + 0 + cam.m_x) / tile_size; i < (1024 + cam.m_x - tmp) / tile_size; ++i)
	{
		for (int j = (tmp + 0 + cam.m_y) / tile_size; j < (1024 + cam.m_y - tmp) / tile_size; ++j)
		{
			double height_treshold = perlin::noise(/*i * dx * 0.1, */ perlin::noise(i * dx * 0.05)) * 100;

			int x = i;
			int y = j;
			if (height_treshold > j)
			{
				tree.insert(Node(x, y, tile_type::air));
			}
			else if (height_treshold <= j)
			{
				//if (rand() % 1000 > 33)
					tree.insert(Node(x, y, tile_type::dirt));
				//else
				//	tree.insert(Node(x, y, tile_type::stone));
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
				vector<const Node *> up = tree.range(Trect<double>(Tpoint<double>(i->m_x - 0.5, i->m_y - 1.0 - 0.5), Tpoint<double>(i->m_x + 0.5, i->m_y - 1.0 + 0.5)));
				vector<const Node *> dw = tree.range(Trect<double>(Tpoint<double>(i->m_x - 0.5, i->m_y + 1.0 - 0.5), Tpoint<double>(i->m_x + 0.5, i->m_y + 1.0 + 0.5)));
				vector<const Node *> lf = tree.range(Trect<double>(Tpoint<double>(i->m_x - 1.0 - 0.5, i->m_y - 0.5), Tpoint<double>(i->m_x - 1.0 + 0.5, i->m_y + 0.5)));
				vector<const Node *> rt = tree.range(Trect<double>(Tpoint<double>(i->m_x + 1.0 - 0.5, i->m_y - 0.5), Tpoint<double>(i->m_x + 1.0 + 0.5, i->m_y + 0.5)));
				if (!up.empty() && !dw.empty() && !lf.empty() && !rt.empty())
				{
					if (
						up[0]->m_tile != tile_type::dirt && dw[0]->m_tile == tile_type::dirt &&
						lf[0]->m_tile != tile_type::dirt && rt[0]->m_tile == tile_type::dirt)
					{
						gfx.draw_surface_alpha(x, y, tile_map_dirt[0], Colors::White);
					}
					else if (
						up[0]->m_tile != tile_type::dirt && dw[0]->m_tile == tile_type::dirt &&
						lf[0]->m_tile == tile_type::dirt && rt[0]->m_tile == tile_type::dirt)
					{
						gfx.draw_surface_alpha(x, y, tile_map_dirt[1], Colors::White);
					}
					else if (
						up[0]->m_tile != tile_type::dirt && dw[0]->m_tile == tile_type::dirt &&
						lf[0]->m_tile == tile_type::dirt && rt[0]->m_tile != tile_type::dirt)
					{
						gfx.draw_surface_alpha(x, y, tile_map_dirt[2], Colors::White);
					}
					else if (
						up[0]->m_tile != tile_type::dirt && dw[0]->m_tile == tile_type::dirt &&
						lf[0]->m_tile != tile_type::dirt && rt[0]->m_tile != tile_type::dirt)
					{
						gfx.draw_surface_alpha(x, y, tile_map_dirt[3], Colors::White);
					}
					else if (
						up[0]->m_tile != tile_type::dirt && dw[0]->m_tile != tile_type::dirt &&
						lf[0]->m_tile != tile_type::dirt && rt[0]->m_tile != tile_type::dirt)
					{
						gfx.draw_surface_alpha(x, y, tile_map_dirt[15], Colors::White);
					}
					//if (up[0]->m_tile == tile_type::dirt && dw[0]->m_tile == tile_type::dirt &&
					//	lf[0]->m_tile == tile_type::dirt && rt[0]->m_tile == tile_type::dirt)
					else
					{
						gfx.draw_surface_alpha(x, y, tile_map_dirt[5], Colors::White);
					}
				}
			}
			if (i->m_tile == tile_type::stone)
			{
				gfx.draw_surface_alpha(x, y, tile_map_stone[5], Colors::White);
			}
		}
	}

	// easy way out:
	gfx.draw_rect(0, 0, tile_size, Graphics::ScreenHeight, Colors::White);
	gfx.draw_rect(Graphics::ScreenWidth - tile_size, 0, tile_size, Graphics::ScreenHeight, Colors::White);
	gfx.draw_rect(0, 0, Graphics::ScreenWidth, tile_size, Colors::White);
	gfx.draw_rect(0, Graphics::ScreenHeight - tile_size, Graphics::ScreenWidth, tile_size, Colors::White);
}
