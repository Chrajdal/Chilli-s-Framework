#include "Game.h"

//-----------------------------------------------------------------------------
Random rnd;
quad_tree_f t;
bool keep_updating = true;
int vec_size = 100;
bool draw_rect_bool = false;
int g_n = 10;
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

	float alignment_weight = 0.001f;
	float separation_weight = 0.0001f;
	float cohesion_weight = 0.001f;

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

	n.m_velocity.Normalize();

	n.m_p.m_x += n.m_velocity.x;
	n.m_p.m_y += n.m_velocity.y;

	nt.insert(n);
}
void test_update_tree_at_once(quad_tree_f & t)
{
	g_n = t.size() / 20;
	if (g_n == 0)
		g_n++;
	quad_tree_f nt;
	if (t.m_root != NULL)
		test_update_node_at_once(t, *t.m_root, nt);
	t = nt;
}
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
	HandleInput();
	ComposeFrame();	
	
	cout << " FPS= " << setw(3) << setprecision(3) << 1000/timer.elapsed() << " time: " << timer.elapsed() << " ms\n";

	gfx.EndFrame();
}

void Game::UpdateModel()
{
	test_update_tree_at_once(t);
}

void Game::HandleInput()
{	
	if (wnd.mouse.LeftIsPressed())
	{
		node_f x(Tpoint<float>((float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY()));
		x.m_velocity = _Vec2<float>(rnd.next_float(-5.0f, 5.0f), rnd.next_float(-5.0f, 5.0f));
		t.insert(x);
	}
	
	if (wnd.mouse.RightIsPressed())
	{
		node_f x(Tpoint<float>((float)wnd.mouse.GetPosX(), (float)wnd.mouse.GetPosY()));
		x.m_velocity = _Vec2<float>(rnd.next_float(-5.0f, 5.0f), rnd.next_float(-5.0f, 5.0f));
		t.insert(x);
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		for (int i = 0; i < 10; ++i)
		{
			node_f x(Tpoint<float>(rnd.next_float(0.0f, (float)Graphics::ScreenWidth), rnd.next_float(0.0f, (float)Graphics::ScreenHeight)));
			x.m_velocity = _Vec2<float>(rnd.next_float(-5.0f, 5.0f), rnd.next_float(-5.0f, 5.0f));
			t.insert(x);
		}

		if (wnd.kbd.KeyIsPressed(VK_CONTROL))
		{
			for (int i = 0; i < 10000; ++i)
			{
				node_f x(Tpoint<float>(rnd.next_float(0.0f, (float)Graphics::ScreenWidth), rnd.next_float(0.0f, (float)Graphics::ScreenHeight)));
				x.m_velocity = _Vec2<float>(rnd.next_float(-5.0f, 5.0f), rnd.next_float(-5.0f, 5.0f));
				t.insert(x);
			}
		}
	}

	if (wnd.kbd.KeyIsPressed(VK_DELETE))
	{
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
}

void Game::ComposeFrame()
{
	cout << t.size() << " ";

	t.draw(gfx, draw_rect_bool);
}
//-----------------------------------------------------------------------------
