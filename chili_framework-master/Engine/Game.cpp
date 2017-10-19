#include "Game.h"

#include "quad_tree.h"

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
	if (n.m_p.m_x + n.m_velocity.x > 0 && n.m_p.m_x + n.m_velocity.x < Graphics::ScreenWidth - 1 &&
		n.m_p.m_y + n.m_velocity.y > 0 && n.m_p.m_y + n.m_velocity.y < Graphics::ScreenHeight - 1)
		gfx.draw_line((int)n.m_p.m_x, (int)n.m_p.m_y, (int)n.m_p.m_x + (int)n.m_velocity.x, (int)n.m_p.m_y + (int)n.m_velocity.y, Colors::White);
}

void update_node_f(node_f & n)
{
	n.m_velocity *= 1.01;
	n.m_p.m_x += n.m_velocity.x;
	n.m_p.m_y += n.m_velocity.y;
	if (n.m_p.m_x - fabs(n.m_velocity.x) < 0)
		n.m_velocity.x = fabs(n.m_velocity.x);
	if (n.m_p.m_x + fabs(n.m_velocity.x) >= Graphics::ScreenWidth - 1)
		n.m_velocity.x = -fabs(n.m_velocity.x);
	if (n.m_p.m_y - fabs(n.m_velocity.y) < 0)
		n.m_velocity.y = fabs(n.m_velocity.y);
	if (n.m_p.m_y + fabs(n.m_velocity.y) >= Graphics::ScreenHeight - 1)
		n.m_velocity.y = -fabs(n.m_velocity.y);
}

vector<node_f> vec;
//          ^ point       ^ velocity

void update_alignment(const vector<node_f> & closest_points, node_f & n)
{
	_Vec2<float> sum(0, 0);
	for (const auto & i : closest_points)
		sum += i.m_velocity;

	sum /= (float)closest_points.size();
	sum.Normalize();

	_Vec2<float> steer = sum - n.m_velocity;
	float steer_weight = 1.01f;
	n.m_velocity = steer * steer_weight;

	float magnitude = n.m_velocity.LenSq();
	if (magnitude > 100) {
		n.m_velocity = n.m_velocity / magnitude;
	}
	n.m_p.m_x += n.m_velocity.x;
	n.m_p.m_y += n.m_velocity.y;
}

void update_separation(const vector<node_f> & closest_points, node_f & n)
{

}

void update_cohesion(const vector<node_f> & closest_points, node_f & n)
{

}

void update_vec(vector<node_f> & v)
{
	for (vector<node_f>::iterator i = v.begin(); i != v.end(); ++i)
	{
		vector<node_f> w = v;
		Tpoint<float> p = i->m_p;
		sort(w.begin(), w.end(), [p](const node_f & a, node_f & b) { return sq_distance(a.m_p, p) <= sq_distance(b.m_p, p); });
		vector<node_f> closest_points = vector<node_f>(w.begin(), w.begin() + w.size() / 50 + 1);

		// alignment
		update_alignment(closest_points, *i);

		// separation
		update_separation(closest_points, *i);

		// cohesion
		update_cohesion(closest_points, *i);


		if (i->m_p.m_x >= Graphics::ScreenWidth)
			i->m_p.m_x = float((int)(i->m_p.m_x) % Graphics::ScreenWidth);

		if (i->m_p.m_x < 0)
			i->m_p.m_x += (float)Graphics::ScreenWidth;

		if (i->m_p.m_y >= Graphics::ScreenHeight)
			i->m_p.m_y = float((int)(i->m_p.m_y) % Graphics::ScreenHeight);

		if (i->m_p.m_y < 0)
			i->m_p.m_y += (float)Graphics::ScreenHeight;
	}
}

void draw_vec(Graphics & gfx, const vector<node_f> & v)
{
	for (const auto & n : v)
	{
		if (n.m_p.m_x + n.m_velocity.x > 0 && n.m_p.m_x + n.m_velocity.x < Graphics::ScreenWidth - 1 &&
			n.m_p.m_y + n.m_velocity.y > 0 && n.m_p.m_y + n.m_velocity.y < Graphics::ScreenHeight - 1)
			//gfx.draw_line((int)n.m_p.m_x, (int)n.m_p.m_y, (int)n.m_p.m_x + (int)n.m_velocity.x, (int)n.m_p.m_y + (int)n.m_velocity.y, Colors::White);
			draw_point(gfx, n.m_p);
	}
}

int vec_size = 200;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	for (int i = 0; i < vec_size; ++i)
	{
		node_f x(Tpoint<float>(random_between(0.0f, (float)Graphics::ScreenWidth - 1), random_between(0.0f, (float)Graphics::ScreenWidth - 1)));
		x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
		vec.push_back(x);
	}
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
	update_vec(vec);

	if (wnd.kbd.KeyIsPressed(VK_F5))
	{
		vec.clear();
		for (int i = 0; i < vec_size; ++i)
		{
			node_f x(Tpoint<float>(random_between(0.0f, (float)Graphics::ScreenWidth - 1), random_between(0.0f, (float)Graphics::ScreenWidth - 1)));
			x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
			vec.push_back(x);
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		vec_size += 10;
		vec.clear();
		for (int i = 0; i < vec_size; ++i)
		{
			node_f x(Tpoint<float>(random_between(0.0f, (float)Graphics::ScreenWidth - 1), random_between(0.0f, (float)Graphics::ScreenWidth - 1)));
			x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
			vec.push_back(x);
		}
	}
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		vec_size -= 10;
		vec.clear();
		for (int i = 0; i < vec_size; ++i)
		{
			node_f x(Tpoint<float>(random_between(0.0f, (float)Graphics::ScreenWidth - 1), random_between(0.0f, (float)Graphics::ScreenWidth - 1)));
			x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
			vec.push_back(x);
		}
	}
}

void Game::ComposeFrame()
{
	draw_vec(gfx, vec);
}
