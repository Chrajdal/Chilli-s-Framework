#include "Game.h"

using namespace std;
using namespace Colors;

vector<Color> color_palette =
{
	Color(248,  12,  18),
	Color(248,  15,  10),
	Color(238,  17,   0),
	Color(246,  34,   8),
	Color(255,  51,  17),
	Color(255,  60,  25),
	Color(255,  68,  34),
	Color(255,  85,  51),
	Color(255, 102,  68),
	Color(255, 127,  59),
	Color(255, 153,  51),
	Color(254, 163,  48),
	Color(254, 174,  45),
	Color(229, 180,  48),
	Color(204, 187,  51),
	Color(204, 191,  33),
	Color(206, 195,  16),
	Color(188, 199,  25),
	Color(170, 204,  34),
	Color(137, 206,  35),
	Color(105, 208,  37),
	Color(69, 206, 103),
	Color(34, 204, 170),
	Color(26, 196, 177),
	Color(18, 189, 185),
	Color(18, 180, 186),
	Color(17, 170, 187),
	Color(42, 119, 204),
	Color(68,  68, 221),
	Color(59,  42, 204),
	Color(51,  17, 187),
	Color(55,  14, 188),
	Color(59,  12, 189),
	Color(63,  23, 171),
	Color(68,  34, 153)
};


void draw_rect(Graphics & gfx, const Trect<int> & rect, const Color & c = Colors::Cyan)
{
	for (int i = rect.m_upleft.m_x; i <= rect.m_downright.m_x; ++i)
	{
		if (i >= 0 && i < Graphics::ScreenWidth && rect.m_upleft.m_y >= 0 && rect.m_downright.m_y < Graphics::ScreenHeight)
		{
			gfx.PutPixel(i, rect.m_upleft.m_y, c);
			gfx.PutPixel(i, rect.m_downright.m_y, c);
		}
	}
	for (int i = rect.m_upleft.m_y; i <= rect.m_downright.m_y; ++i)
	{
		if (i >= 0 && i < Graphics::ScreenWidth && rect.m_upleft.m_y >= 0 && rect.m_downright.m_y < Graphics::ScreenHeight)
		{
			gfx.PutPixel(rect.m_upleft.m_x, i, c);
			gfx.PutPixel(rect.m_downright.m_x, i, c);
		}
	}
}

void draw_point(Graphics & gfx, const Tpoint<int> & p, const Color & c = Colors::White)
{
	if (p.m_x >= 0 && p.m_y >= 0 &&
		p.m_x < Graphics::ScreenWidth &&
		p.m_y < Graphics::ScreenHeight)
	{
		//gfx.PutPixel(p.m_x - 1, p.m_y - 1, c);
		//gfx.PutPixel(p.m_x - 1, p.m_y + 1, c);
		//gfx.PutPixel(p.m_x + 1, p.m_y - 1, c);
		//gfx.PutPixel(p.m_x + 1, p.m_y + 1, c);
		
		gfx.PutPixel(p.m_x, p.m_y, c);
	}
}

bool draw_rect_bool = false;

void draw_node(Graphics & gfx, const node * n, int depth = 0)
{
	if (n->m_ul != NULL)
		draw_node(gfx, n->m_ul, depth + 1);
	if (n->m_ur != NULL)
		draw_node(gfx, n->m_ur, depth + 1);
	if (n->m_dl != NULL)
		draw_node(gfx, n->m_dl, depth + 1);
	if (n->m_dr != NULL)
		draw_node(gfx, n->m_dr, depth + 1);

	if (draw_rect_bool == true)
	{
		gfx.draw_line(n->m_p.m_x, n->m_p.m_y, n->m_p.m_x,                n->m_ul_r.m_upleft.m_y,    Colors::Green);
		gfx.draw_line(n->m_p.m_x, n->m_p.m_y, n->m_p.m_x,                n->m_dr_r.m_downright.m_y, Colors::Blue);
		gfx.draw_line(n->m_p.m_x, n->m_p.m_y, n->m_ul_r.m_upleft.m_x,    n->m_p.m_y,                Colors::Yellow);
		gfx.draw_line(n->m_p.m_x, n->m_p.m_y, n->m_dr_r.m_downright.m_x, n->m_p.m_y,                Colors::Red);
	}
	draw_point(gfx, n->m_p, color_palette[(depth % color_palette.size())]);
	//draw_point(gfx, n->m_p, Colors::Gray);
}

void draw_quad_tree(Graphics & gfx, const quad_tree & t)
{
	if (t.m_root != NULL)
		draw_node(gfx, t.m_root);
}

void draw_node(Graphics & gfx, const node_f & n)
{
	if (n.m_ul != NULL)
		draw_node(gfx, *n.m_ul);
	if (n.m_ur != NULL)
		draw_node(gfx, *n.m_ur);
	if (n.m_dl != NULL)
		draw_node(gfx, *n.m_dl);
	if (n.m_dr != NULL)
		draw_node(gfx, *n.m_dr);

	//if (n.m_p.m_x + n.velocity.x > 0 && n.m_p.m_x + n.velocity.x < Graphics::ScreenWidth - 1 &&
	//	n.m_p.m_y + n.velocity.y > 0 && n.m_p.m_y + n.velocity.y < Graphics::ScreenHeight - 1)
		gfx.draw_line((int)n.m_p.m_x, (int)n.m_p.m_y, (int)n.m_p.m_x + (int)n.velocity.x, (int)n.m_p.m_y + (int)n.velocity.y, Colors::White);
}

void update_node_f(node_f & n)
{
	n.velocity *= 1.01;

	n.m_p.m_x += n.velocity.x;
	n.m_p.m_y += n.velocity.y;

	if (n.m_p.m_x - fabs(n.velocity.x) < 0)
		n.velocity.x = fabs(n.velocity.x);
	if (n.m_p.m_x + fabs(n.velocity.x) >= Graphics::ScreenWidth - 1)
		n.velocity.x = -fabs(n.velocity.x);
	if (n.m_p.m_y - fabs(n.velocity.y) < 0)
		n.velocity.y = fabs(n.velocity.y);
	if (n.m_p.m_y + fabs(n.velocity.y) >= Graphics::ScreenHeight - 1)
		n.velocity.y = -fabs(n.velocity.y);
}

quad_tree t;
Tpoint<int> p;
Tpoint<int> closest;
vector<Tpoint<int>> closest_points;

Tpoint<float> mypoint(Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2);
node_f mynode(mypoint);

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	p = Tpoint<int>(random_int(0, Graphics::ScreenWidth - 1), random_int(0, Graphics::ScreenHeight - 1));
	closest = t.find_closest_point(p);

	float MIN_X = GMINX;
	float MID_X = (float)p.m_x;
	float MAX_X = GMAXX;
	float MIN_Y = GMINY;
	float MID_Y = (float)p.m_y;
	float MAX_Y = GMAXY;

	mynode.m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
	mynode.m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
	mynode.m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
	mynode.m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));

	mynode.velocity = _Vec2<float>(random_between(-10.0f, 10.0f), random_between(-10.0f, 10.0f));
	//mynode.velocity.Normalize();
}

void Game::Go()
{
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		wnd.Kill();
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	update_node_f(mynode);

	if (wnd.mouse.LeftIsPressed())
	{
		p = Tpoint<int>(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
		//mynode.m_p = Tpoint<float>((float)wnd.mouse.GetPosX(), ((float)wnd.mouse.GetPosY()));
		closest_points = t.find_n_closest_points(p, 500);

		mynode.velocity = _Vec2<float>(random_between(-10.0f, 10.0f), random_between(-10.0f, 10.0f));
	}

	if (wnd.mouse.RightIsPressed())
	{
		t.insert(Tpoint<int>(wnd.mouse.GetPosX(), wnd.mouse.GetPosY()));
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		for (int i = 0; i < 100; ++i)
			t.insert(Tpoint<int>(random_int(0, GMAXX), random_int(0, GMAXY)));
	}

	if (wnd.kbd.KeyIsPressed(VK_CONTROL))
	{
		for (int i = 0; i < 10000; ++i)
			t.insert(Tpoint<int>(random_int(0, GMAXX), random_int(0, GMAXY)));
	}

	if (wnd.kbd.KeyIsPressed(VK_UP))
		draw_rect_bool = true;

	if (wnd.kbd.KeyIsPressed(VK_DOWN))
		draw_rect_bool = false;

	if (wnd.kbd.KeyIsPressed(VK_F5))
	{
		t.clear();
		closest_points.clear();
		p = Tpoint<int>(-1, -1);
	}
}

void Game::ComposeFrame()
{
	draw_quad_tree(gfx, t);
	draw_point(gfx, p, Colors::Green);
	for (const auto & i : closest_points)
		draw_point(gfx, i, Colors::Magenta);

	//draw_node(gfx, mynode);
}
