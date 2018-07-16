#include "Game.h"

//constexpr float GMINY = 0;
//constexpr float GMAXY = (Graphics::ScreenHeight - 1);
//constexpr float GMINX = 0;
//constexpr float GMAXX = (Graphics::ScreenWidth - 1);

bool keep_updating = true;
int vec_size = 100;
bool draw_rect_bool = false;

template<typename T>
void draw_rect(Graphics & gfx, const Trect<T> & rect, const Color & c = Colors::Cyan)
{
	for (int i = rect.m_upleft.m_x; i <= rect.m_downright.m_x; ++i)
	{
		if (i >= 0 && i < Graphics::ScreenWidth && rect.m_upleft.m_y >= 0 && rect.m_downright.m_y < Graphics::ScreenHeight)
		{
			gfx.PutPixel((int)i, (int)rect.m_upleft.m_y, c);
			gfx.PutPixel((int)i, (int)rect.m_downright.m_y, c);
		}
	}
	for (int i = rect.m_upleft.m_y; i <= rect.m_downright.m_y; ++i)
	{
		if (i >= 0 && i < Graphics::ScreenWidth && rect.m_upleft.m_y >= 0 && rect.m_downright.m_y < Graphics::ScreenHeight)
		{
			gfx.PutPixel((int)rect.m_upleft.m_x, (int)i, c);
			gfx.PutPixel((int)rect.m_downright.m_x, (int)i, c);
		}
	}
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
	vector<node_f> closest_points;
	closest_points.reserve(5);
	t.find_n_closest_points(n.m_p, 5, closest_points);

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
int g_n = 100;
float g_alignment_weight = 3.0f;
float g_separation_weight = 3.0f;
float g_cohesion_weight = 3.0f;

//-----------------------------------------------------------------------------
void update_node_at_once(const vector<node_f> & closest_points, node_f & n)
{
	_Vec2<float> alignment(0, 0);
	_Vec2<float> separation(0, 0);
	_Vec2<float> cohesion(0, 0);

	for (const auto & i : closest_points)
	{
		if (i.m_p != n.m_p)
		{
			alignment += i.m_velocity;

			separation.x += i.m_p.m_x - n.m_p.m_x;
			separation.y += i.m_p.m_y - n.m_p.m_y;

			cohesion.x += i.m_p.m_x;
			cohesion.y += i.m_p.m_y;
		}
	}

	alignment.x /= (float)closest_points.size();
	alignment.y /= (float)closest_points.size();
	alignment = alignment - n.m_velocity;

	separation.x *= -1;
	separation.y *= -1;

	cohesion.x /= (float)closest_points.size();
	cohesion.y /= (float)closest_points.size();

	float alignment_weight = 1.0f;
	float separation_weight = 1.0f;
	float cohesion_weight = 1.0f;

	n.m_velocity.x += alignment.x * alignment_weight + separation.x * separation_weight + cohesion.x * cohesion_weight;
	n.m_velocity.y += alignment.y * alignment_weight + separation.y * separation_weight + cohesion.y * cohesion_weight;
}
void test_update_node_at_once(quad_tree_f & t, node_f n, quad_tree_f & nt)
{
	if (n.m_ul != NULL)	test_update_node_at_once(t, *n.m_ul, nt);
	if (n.m_ur != NULL)	test_update_node_at_once(t, *n.m_ur, nt);
	if (n.m_dl != NULL)	test_update_node_at_once(t, *n.m_dl, nt);
	if (n.m_dr != NULL)	test_update_node_at_once(t, *n.m_dr, nt);

	int n_closest_points = g_n;
	vector<node_f> closest_points;
	closest_points.reserve(n_closest_points);
	t.find_n_closest_points(n.m_p, n_closest_points, closest_points);

	update_node_at_once(closest_points, n);

	//float length = sqrt(n.m_velocity.x * n.m_velocity.x + n.m_velocity.y * n.m_velocity.y);
	//if (length >= 5.0f)
	//{
	//	n.m_velocity.x /= length;
	//	n.m_velocity.y /= length;
	//}

	//if (n.m_p.m_x >= float(Graphics::ScreenWidth))
	//	n.m_p.m_x = float((int)(n.m_p.m_x) % Graphics::ScreenWidth);
	//if (n.m_p.m_x < 0.0f)
	//	n.m_p.m_x += (float)Graphics::ScreenWidth;
	//if (n.m_p.m_y >= float(Graphics::ScreenHeight))
	//	n.m_p.m_y = float((int)(n.m_p.m_y) % Graphics::ScreenHeight);
	//if (n.m_p.m_y < 0.0f)
	//	n.m_p.m_y += (float)Graphics::ScreenHeight;

	n.m_p.m_x += n.m_velocity.x;
	n.m_p.m_y += n.m_velocity.y;

	//if (n.m_p.m_x > GMAXX) n.m_p.m_x = GMINX + 5;
	//if (n.m_p.m_x < GMINX) n.m_p.m_x = GMAXX - 5;
	//if (n.m_p.m_y > GMAXY) n.m_p.m_y = GMINY + 5;
	//if (n.m_p.m_y < GMINY) n.m_p.m_y = GMAXY - 5;

	nt.insert(move(n));
}
void test_update_tree_at_once(quad_tree_f & t)
{
	g_n = t.size() / 10;
	if (g_n == 0)
		g_n++;
	quad_tree_f nt;
	if (t.m_root != NULL)
		test_update_node_at_once(t, *t.m_root, nt);
	t = nt;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void test_update_node(quad_tree_f & t, node_f & n, quad_tree_f & nt)
{
	vector<node_f> closest_points;
	closest_points.reserve(100);
	t.find_n_closest_points(n.m_p, 100, closest_points);

	_Vec2<float> alignment(0, 0);
	_Vec2<float> separation(0, 0);
	_Vec2<float> cohesion(0, 0);

	alignment = update_alignment(closest_points, n);
	separation = update_separation(closest_points, n);
	cohesion = update_cohesion(closest_points, n);

	float alignment_weight = 15.0f;
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

	if (n.m_p.m_x < float(Graphics::ScreenWidth))
	{
		n.m_p.m_x = float((int)(n.m_p.m_x) % Graphics::ScreenWidth);
	}
	else if (n.m_p.m_x > 0.0f)
	{
		n.m_p.m_x += (float)Graphics::ScreenWidth;
	}
	else if (n.m_p.m_y < float(Graphics::ScreenHeight))
	{
		n.m_p.m_y = float((int)(n.m_p.m_y) % Graphics::ScreenHeight);
	}
	else if (n.m_p.m_y > 0.0f)
	{
		n.m_p.m_y += (float)Graphics::ScreenHeight;
	}
	
	

	if (n.m_ul != NULL)	test_update_node(t, *n.m_ul, nt);
	if (n.m_ur != NULL)	test_update_node(t, *n.m_ur, nt);
	if (n.m_dl != NULL)	test_update_node(t, *n.m_dl, nt);
	if (n.m_dr != NULL)	test_update_node(t, *n.m_dr, nt);


	nt.insert(n);
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
Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));
}

inline ostream & nl(ostream & os) { return os << "\n"; }

void Game::Go()
{
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		wnd.Kill();

	CTimer timer;

	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	HandleInput();
	
	

	/* ----------------------------------- */
	// GUAD_TREE_F size drawing
	cout << t.size() << " ";
	/* ----------------------------------- */

	/* ----------------------------------- */
	cout << "time: " << timer.elapsed() << " ms\n";
		/* ----------------------------------- */

	gfx.EndFrame();




}

void Game::UpdateModel()
{
	test_update_tree(t);
}

void Game::HandleInput()
{	
	if (wnd.mouse.LeftIsPressed())
	{
		node_f x(Tpoint<float>((float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY()));
		x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
		t.insert(x);
	}
	
	if (wnd.mouse.RightIsPressed())
	{
		node_f x(Tpoint<float>((float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY()));
		x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
		t.insert(x);
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		for (int i = 0; i < 10; ++i)
		{
			node_f x(Tpoint<float>(random_between(0.0f, (float)Graphics::ScreenWidth), random_between(0.0f, (float)Graphics::ScreenHeight)));
			x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
			t.insert(x);
		}

		if (wnd.kbd.KeyIsPressed(VK_CONTROL))
		{
			for (int i = 0; i < 10000; ++i)
			{
				node_f x(Tpoint<float>(random_between(0.0f, (float)Graphics::ScreenWidth), random_between(0.0f, (float)Graphics::ScreenHeight)));
				x.m_velocity = _Vec2<float>(random_between(-5.0f, 5.0f), random_between(-5.0f, 5.0f));
				t.insert(x);
			}
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_DELETE))
	{
		g_alignment_weight = 5.0f;
		g_separation_weight = 5.0f;
		g_cohesion_weight = 5.0f;
		t.clear();
	}
	if (wnd.kbd.KeyIsPressed(VK_UP))
		draw_rect_bool = true;
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
		draw_rect_bool = false;
	if (wnd.kbd.KeyIsPressed(VK_LEFT))
		keep_updating = false;
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
		keep_updating = true;
	
	if (wnd.kbd.KeyIsPressed(VK_F1))
		g_alignment_weight += 0.1f;
	if (wnd.kbd.KeyIsPressed(VK_F2))
		g_alignment_weight -= 0.1f;

	if (wnd.kbd.KeyIsPressed(VK_F3))
		g_separation_weight += 0.1f;
	if (wnd.kbd.KeyIsPressed(VK_F4))
		g_separation_weight -= 0.1f;

	if (wnd.kbd.KeyIsPressed(VK_F5))
		g_cohesion_weight += 0.1f;
	if (wnd.kbd.KeyIsPressed(VK_F6))
		g_cohesion_weight -= 0.1f;
}

void Game::ComposeFrame()
{
	t.draw(gfx, draw_rect_bool);
}
//-----------------------------------------------------------------------------
