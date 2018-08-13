#include "Game.h"

inline ostream & nl(ostream & os) { return os << "\n"; }

class Player
{
public:
	Player(const Bitmap & bmp)
		:
		player_sprite(&bmp,0,0,bmp.width,bmp.height)
	{
		m_x = 0.0;
		m_y = 0.0;
	}

	Surface player_sprite;
	double m_x;
	double m_y;
};

QuadTree tree;
Camera cam;
Bitmap tile_sheet_dirt(".\\Assets\\grass.bmp");
Bitmap tile_sheet_stone(".\\Assets\\stone.bmp");
Bitmap player_bmp(".\\Assets\\Main Character.bmp");
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
constexpr int tile_size = 16;
tile_type selected_tile = tile_type::dirt;
Player player(player_bmp);

//struct v2d
//{
//	double x;
//	double y;
//};

v2d screen_to_world(const v2d & rhs, const Camera & cam)
{
	v2d res;
	res.x = (rhs.x + cam.m_x) * tile_size;
	res.y = (rhs.y + cam.m_y) * tile_size;
	return res;
}

v2d world_to_screen(const v2d & rhs, const Camera & cam)
{
	v2d res;
	res.x = (rhs.x + cam.m_x) / tile_size;
	res.y = (rhs.y + cam.m_y) / tile_size;
	return res;
}

void load_from_file(void)
{
	tree.LoadFromFile("save_file.txt");
}

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	// Load assets:
	v2d a;
	a.x = Graphics::ScreenWidth / 2 - player.player_sprite.width() / 2;
	a.y = Graphics::ScreenHeight / 2 - player.player_sprite.height() / 2;
	v2d tmp = screen_to_world(a, cam);
	player.m_x = tmp.x;
	player.m_y = tmp.y;
	
	auto hndl = std::async(std::launch::async, load_from_file);

	for (int j = 0; j < tile_sheet_dirt.height / tile_size; ++j)
		for (int i = 0; i < tile_sheet_dirt.width / tile_size; ++i)
			tile_map_dirt.push_back(Surface(&tile_sheet_dirt, i * tile_size, j * tile_size, tile_size, tile_size));

	for (int j = 0; j < tile_sheet_dirt.height / tile_size; ++j)
		for (int i = 0; i < tile_sheet_dirt.width / tile_size; ++i)
			tile_map_stone.push_back(Surface(&tile_sheet_stone, i * tile_size, j * tile_size, tile_size, tile_size));

	hndl.get();


}

Game::~Game(void)
{
	tree.SaveToFile("save_file.txt");
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
	double speed = 7e0;
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
			Node * tmp = tree.access((int)x, (int)y);
			tmp->m_tile = tile_type::air;
		}
		if (wnd.mouse.RightIsPressed())
		{
			Node * tmp = tree.access((int)x, (int)y);
			tmp->m_tile = selected_tile;
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_DELETE))
	{
		tree.clear();
	}
}

void Game::UpdateModel()
{	
	int tmp = -200;
	for (int i = (int)((tmp + 0 + cam.m_x) / tile_size); i < (int)((1024 + cam.m_x - tmp) / tile_size); ++i)
	{
		for (int j = (int)((tmp + 0 + cam.m_y) / tile_size); j < (int)((1024 + cam.m_y - tmp) / tile_size); ++j)
		{
			double height_treshold = perlin::noise(/*i * dx * 0.1, */ perlin::noise(i * dx * 0.05)) * 100;
			double stone_threshold = perlin::noise(j * dx * 0.1) * 200;
			int x = i;
			int y = j;
			if (height_treshold > j)
			{
				tree.insert(Node(x, y, tile_type::air));
			}
			else if (height_treshold <= j && height_treshold > j - stone_threshold)
			{
				tree.insert(Node(x, y, tile_type::dirt));
			}
			else if (height_treshold <= j - stone_threshold)
			{
				double noise = perlin::noise(i * dx, j * dy);
				if (noise > 0.0)
					tree.insert(Node(x, y, tile_type::dirt));
				else
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
		int x = (int)(i->m_x * tile_size - cam.m_x);
		int y = (int)(i->m_y * tile_size - cam.m_y);

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
	v2d tmp;
	tmp.x = player.m_x;
	tmp.y = player.m_y;
	v2d player_pos = world_to_screen(tmp, cam);
	gfx.draw_surface_alpha((int)player_pos.x, (int)player_pos.y, player.player_sprite, Colors::White);

	// easy way out:
	gfx.draw_rect(0, 0, (int)tile_size, Graphics::ScreenHeight, Colors::White);
	gfx.draw_rect((int)Graphics::ScreenWidth - tile_size, 0, (int)tile_size, Graphics::ScreenHeight, Colors::White);
	gfx.draw_rect(0, 0, Graphics::ScreenWidth, (int)tile_size, Colors::White);
	gfx.draw_rect(0, (int)Graphics::ScreenHeight - tile_size, Graphics::ScreenWidth, (int)tile_size, Colors::White);

	gfx.draw_line(0, 0, Graphics::ScreenWidth - 1, Graphics::ScreenHeight - 1, Colors::Gray);
	gfx.draw_line(Graphics::ScreenWidth - 1, 0, 0, Graphics::ScreenHeight - 1, Colors::Gray);
}
