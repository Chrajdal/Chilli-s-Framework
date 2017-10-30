#include "Game.h"

#include "quad_tree.h"

using namespace std;
using namespace Colors;

bool keep_updating = true;
int vec_size = 100;
bool draw_rect_bool = false;
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
void draw_point(Graphics & gfx, const Tpoint<float> & p, const Color & c = Colors::White)
{
	if (p.m_x >= 0 && p.m_y >= 0 &&
		p.m_x < Graphics::ScreenWidth &&
		p.m_y < Graphics::ScreenHeight)
	{
		//gfx.PutPixel(p.m_x - 1, p.m_y - 1, c);
		//gfx.PutPixel(p.m_x - 1, p.m_y + 1, c);
		//gfx.PutPixel(p.m_x + 1, p.m_y - 1, c);
		//gfx.PutPixel(p.m_x + 1, p.m_y + 1, c);

		gfx.PutPixel((int)p.m_x, (int)p.m_y, c);
	}
}

void draw_node(Graphics & gfx, const node_f * n)
{
	if (n->m_ul != NULL) draw_node(gfx, n->m_ul);
	if (n->m_ur != NULL) draw_node(gfx, n->m_ur);
	if (n->m_dl != NULL) draw_node(gfx, n->m_dl);
	if (n->m_dr != NULL) draw_node(gfx, n->m_dr);

	//if (draw_rect_bool == true)
	//{
	//	gfx.draw_line(n->m_p.m_x, n->m_p.m_y, n->m_p.m_x, n->m_ul_r.m_upleft.m_y, Colors::Green);
	//	gfx.draw_line(n->m_p.m_x, n->m_p.m_y, n->m_p.m_x, n->m_dr_r.m_downright.m_y, Colors::Blue);
	//	gfx.draw_line(n->m_p.m_x, n->m_p.m_y, n->m_ul_r.m_upleft.m_x, n->m_p.m_y, Colors::Yellow);
	//	gfx.draw_line(n->m_p.m_x, n->m_p.m_y, n->m_dr_r.m_downright.m_x, n->m_p.m_y, Colors::Red);
	//}
	//draw_point(gfx, n->m_p, color_palette[(depth % color_palette.size())]);
	draw_point(gfx, n->m_p, Colors::Gray);
}

quad_tree_f t;

_Vec2<float> update_alignment(const vector<node_f> & closest_points, const node_f & n)
{
	_Vec2<float> v (0, 0);
	for (const auto & i : closest_points)
		v += i.m_velocity;
	v.x /= (float)(closest_points.size());
	v.y /= (float)(closest_points.size());
	v = v - n.m_velocity;

	return v;
}
_Vec2<float> update_separation(const vector<node_f> & closest_points, const node_f & n)
{
	_Vec2<float> v(0, 0);

	for (const auto & i : closest_points)
	{
		if (i.m_p != n.m_p)
		{
			v.x += i.m_p.m_x - n.m_p.m_x;
			v.y += i.m_p.m_y - n.m_p.m_y;
		}
	}

	v.x *= -1;
	v.y *= -1;

	return v;
}
_Vec2<float> update_cohesion(const vector<node_f> & closest_points, const node_f & n)
{
	_Vec2<float> v(0, 0);

	for (const auto & i : closest_points)
	{
		if (i.m_p != n.m_p)
		{
			v.x += i.m_p.m_x;
			v.y += i.m_p.m_y;
		}
	}

	v.x /= closest_points.size() - 1;
	v.y /= closest_points.size() - 1;

	return v;
}

//-----------------------------------------------------------------------------
void update_node_f(quad_tree_f & t, node_f & n, quad_tree_f & nt)
{
	vector<node_f> closest_points = t.find_n_closest_points(n.m_p, 5);
	_Vec2<float> alignment(0, 0), separation(0, 0), cohesion(0, 0);
	alignment = update_alignment(closest_points, n);
	separation = update_separation(closest_points, n);
	cohesion = update_cohesion(closest_points, n);

	float alignment_weight = 5.0f;
	float separation_weight = 5.0f;
	float cohesion_weight = 5.0f;

	n.m_velocity.x += alignment.x * alignment_weight + separation.x * separation_weight + cohesion.x * cohesion_weight;
	n.m_velocity.y += alignment.y * alignment_weight + separation.y * separation_weight + cohesion.y * cohesion_weight;
	if (n.m_velocity.x > 5.0f)
		n.m_velocity.x = 5.0f;
	if (n.m_velocity.y > 5.0f)
		n.m_velocity.y = 5.0f;
	
	n.m_p.m_x += n.m_velocity.x;
	n.m_p.m_y += n.m_velocity.y;

	if (n.m_p.m_x >= float(Graphics::ScreenWidth))
		n.m_p.m_x = float((int)(n.m_p.m_x) % Graphics::ScreenWidth);
	if (n.m_p.m_x < 0.0f)
		n.m_p.m_x += (float)Graphics::ScreenWidth;
	if (n.m_p.m_y >= float(Graphics::ScreenHeight))
		n.m_p.m_y = float((int)(n.m_p.m_y) % Graphics::ScreenHeight);
	if (n.m_p.m_y < 0.0f)
		n.m_p.m_y += (float)Graphics::ScreenHeight;

	nt.insert(n);

	if (n.m_ul != NULL)	update_node_f(t, *n.m_ul, nt);
	if (n.m_ur != NULL)	update_node_f(t, *n.m_ur, nt);
	if (n.m_dl != NULL)	update_node_f(t, *n.m_dl, nt);
	if (n.m_dr != NULL)	update_node_f(t, *n.m_dr, nt);
}
void update_quad_tree_f(quad_tree_f & t)
{
	quad_tree_f nt;
	if (t.m_root != NULL)
		update_node_f(t, *t.m_root, nt);
	//t.clear();
	t = nt;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void test_update_node(quad_tree_f & t, node_f & n, quad_tree_f & nt)
{
	vector<node_f> closest_points = t.find_n_closest_points(n.m_p, 5);

	_Vec2<float> alignment(0, 0);
	_Vec2<float> separation(0, 0);
	_Vec2<float> cohesion(0, 0);

	alignment = update_alignment(closest_points, n);
	separation = update_separation(closest_points, n);
	cohesion = update_cohesion(closest_points, n);

	float alignment_weight = 5.0f;
	float separation_weight = 5.0f;
	float cohesion_weight = 5.0f;

	n.m_velocity.x += alignment.x * alignment_weight + separation.x * separation_weight + cohesion.x * cohesion_weight;
	n.m_velocity.y += alignment.y * alignment_weight + separation.y * separation_weight + cohesion.y * cohesion_weight;

	float length = sqrt(n.m_velocity.x * n.m_velocity.x + n.m_velocity.y * n.m_velocity.y);
	if (length >= 5.0f)
	{
		n.m_velocity.x /= length;
		n.m_velocity.y /= length;
	}

	n.m_p.m_x += n.m_velocity.x;
	n.m_p.m_y += n.m_velocity.y;

	if (n.m_p.m_x >= float(Graphics::ScreenWidth))
		n.m_p.m_x = float((int)(n.m_p.m_x) % Graphics::ScreenWidth);
	if (n.m_p.m_x < 0.0f)
		n.m_p.m_x += (float)Graphics::ScreenWidth;
	if (n.m_p.m_y >= float(Graphics::ScreenHeight))
		n.m_p.m_y = float((int)(n.m_p.m_y) % Graphics::ScreenHeight);
	if (n.m_p.m_y < 0.0f)
		n.m_p.m_y += (float)Graphics::ScreenHeight;

	nt.insert(n);

	if (n.m_ul != NULL)	test_update_node(t, *n.m_ul, nt);
	if (n.m_ur != NULL)	test_update_node(t, *n.m_ur, nt);
	if (n.m_dl != NULL)	test_update_node(t, *n.m_dl, nt);
	if (n.m_dr != NULL)	test_update_node(t, *n.m_dr, nt);

}
void test_update_tree(quad_tree_f & t)
{
	quad_tree_f nt;
	if (t.m_root != NULL)
		test_update_node(t, *t.m_root, nt);
	t = nt;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void draw_quad_tree_f(Graphics & gfx, const quad_tree_f & t)
{
	if (t.m_root != NULL)
		draw_node(gfx, t.m_root);
}
//-----------------------------------------------------------------------------

//=============================================================================
quad_tree itree;
//=============================================================================

//-----------------------------------------------------------------------------
Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	//for (int i = 0; i < 1; ++i)
	//{
	//	node_f x(Tpoint<float>(random_between(0.0f, (float)Graphics::ScreenWidth - 1), random_between(0.0f, (float)Graphics::ScreenWidth - 1)));
	//	x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
	//	t.insert(x);
	//}
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
	if (wnd.mouse.RightIsPressed())
	{
		itree.insert({ wnd.mouse.GetPosX(), wnd.mouse.GetPosY() });
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		for (int i = 0; i < 1000; ++i)
			itree.insert(Tpoint<int>((int)random_between(0.0f, (float)Graphics::ScreenWidth - 1), (int)random_between(0.0f, (float)Graphics::ScreenWidth - 1)));
	}

	if (wnd.kbd.KeyIsPressed(VK_F4))
	{
		quad_tree tmp;
		itree = tmp;
	}

	//if (keep_updating == true)
	//	test_update_tree(t);
	//
	//if (wnd.mouse.LeftIsPressed())
	//{
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		node_f x(Tpoint<float>(wnd.mouse.GetPosX(), wnd.mouse.GetPosY()));
	//		x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
	//		t.insert(x);
	//	}
	//}
	//
	//if (wnd.mouse.RightIsPressed())
	//{
	//	node_f x(Tpoint<float>(wnd.mouse.GetPosX(), wnd.mouse.GetPosY()));
	//	x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
	//	t.insert(x);
	//}
	//
	//if (wnd.kbd.KeyIsPressed(VK_SPACE))
	//{
	//	quad_tree_f nt = t;
	//	
	//	for (int i = 0; i < 1000; ++i)
	//	{
	//		node_f x(Tpoint<float>(random_between(0.0f, (float)Graphics::ScreenWidth - 1), random_between(0.0f, (float)Graphics::ScreenWidth - 1)));
	//		x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
	//		nt.insert(x);
	//	}
	//	t = nt;
	//}
	//
	if (wnd.kbd.KeyIsPressed(VK_F5))
		itree.clear();
	if (wnd.kbd.KeyIsPressed(VK_UP))
		draw_rect_bool = true;
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
		draw_rect_bool = false;
	//if (wnd.kbd.KeyIsPressed(VK_LEFT))
	//	keep_updating = false;
	//if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	//	keep_updating = true;
}

void Game::ComposeFrame()
{
	itree.draw(gfx, draw_rect_bool);

	//draw_quad_tree_f(gfx, t);
}
//-----------------------------------------------------------------------------
