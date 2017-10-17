#include <iostream>
#include <iomanip>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdlib>
#include <chrono>
#include <cassert>
#include <sstream>
#include <ctime>
#include <numeric>
#include <thread>
#include <mutex>
#include <fstream>
#include "MainWindow.h"
#include "Game.h"

using namespace std;
using namespace Colors;

#define GMINY 0
#define GMAXY (Graphics::ScreenHeight - 1)
#define GMINX 0
#define GMAXX (Graphics::ScreenWidth - 1)

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
	Color( 69, 206, 103),
	Color( 34, 204, 170),
	Color( 26, 196, 177),
	Color( 18, 189, 185),
	Color( 18, 180, 186),
	Color( 17, 170, 187),
	Color( 42, 119, 204),
	Color( 68,  68, 221),
	Color( 59,  42, 204),
	Color( 51,  17, 187),
	Color( 55,  14, 188),
	Color( 59,  12, 189),
	Color( 63,  23, 171),
	Color( 68,  34, 153)
};

double my_distance(const Tpoint & a, const Tpoint & b)
{
	return sqrt((double)(a.m_x - b.m_x) * (double)(a.m_x - b.m_x)
		      + (double)(a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
unsigned long long sq_distance(const Tpoint & a, const Tpoint & b)
{
	return (unsigned long long)((a.m_x - b.m_x) * (a.m_x - b.m_x))
		 + (unsigned long long)((a.m_y - b.m_y) * (a.m_y - b.m_y));
}
Tpoint get_closest_p(const Trect & rect, const Tpoint & p)
{
	/* 1 | 2 | 3
	  ---+---+---
	   4 | 5 | 6
	  ---+---+---
	   7 | 8 | 9 */

	if (p.m_x < rect.m_upleft.m_x)
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 1
			return Tpoint(rect.m_upleft);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 7
			return Tpoint(rect.m_upleft.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 4
			return Tpoint(rect.m_upleft.m_x, p.m_y);
		}
	}
	else if (p.m_x > rect.m_downright.m_x)
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 3
			return Tpoint(rect.m_downright.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 9
			return Tpoint(rect.m_downright);
		}
		else
		{	// no.: 6
			return Tpoint(rect.m_downright.m_x, p.m_y);
		}
	}
	else
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 2
			return Tpoint(p.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 8
			return Tpoint(p.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 5
			return p;
		}
	}
}

class node
{
public:
	node(const Tpoint & p = {},
		const Trect upleft_rect = {},
		const Trect m_upright_rect = {},
		const Trect m_downleft_rect = {},
		const Trect m_downright_rect = {})
		: m_p(p),
		m_ul_r(),
		m_ur_r(),
		m_dl_r(),
		m_dr_r(),
		m_ul(NULL), m_ur(NULL), m_dl(NULL), m_dr(NULL)
	{}

	~node(void)
	{
		delete m_ul;
		delete m_ur;
		delete m_dl;
		delete m_dr;
	}

	void insert(const Tpoint & p, const node * parent)
	{
		if (m_p.m_x == p.m_x && m_p.m_y == p.m_y)
			return;

		if (p.m_y < m_p.m_y) // UP
		{
			if (p.m_x < m_p.m_x) // UP-LEFT
			{
				if (m_ul == NULL)
				{
					m_ul = new node(p);
					int MIN_X = parent->m_ul_r.m_upleft.m_x;
					int MID_X = p.m_x;
					int MAX_X = parent->m_ul_r.m_downright.m_x;
					int MIN_Y = parent->m_ul_r.m_upleft.m_y;
					int MID_Y = p.m_y;
					int MAX_Y = parent->m_ul_r.m_downright.m_y;

					m_ul->m_ul_r = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
					m_ul->m_ur_r = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
					m_ul->m_dl_r = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
					m_ul->m_dr_r = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
				}
				else
				{
					m_ul->insert(p, m_ul);
				}
			}
			else // (p.m_x >= m_p.m_x) // UP-RIGHT
			{
				if (m_ur == NULL)
				{
					m_ur = new node(p);
					int MIN_X = parent->m_ur_r.m_upleft.m_x;
					int MID_X = p.m_x;
					int MAX_X = parent->m_ur_r.m_downright.m_x;
					int MIN_Y = parent->m_ur_r.m_upleft.m_y;
					int MID_Y = p.m_y;
					int MAX_Y = parent->m_ur_r.m_downright.m_y;

					m_ur->m_ul_r = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
					m_ur->m_ur_r = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
					m_ur->m_dl_r = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
					m_ur->m_dr_r = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
				}
				else
				{
					m_ur->insert(p, m_ur);
				}
			}
		}
		else // DOWN
		{
			if (p.m_x < m_p.m_x) // DOWN-LEFT
			{
				if (m_dl == NULL)
				{
					m_dl = new node(p);
					int MIN_X = parent->m_dl_r.m_upleft.m_x;
					int MID_X = p.m_x;
					int MAX_X = parent->m_dl_r.m_downright.m_x;
					int MIN_Y = parent->m_dl_r.m_upleft.m_y;
					int MID_Y = p.m_y;
					int MAX_Y = parent->m_dl_r.m_downright.m_y;

					m_dl->m_ul_r = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
					m_dl->m_ur_r = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
					m_dl->m_dl_r = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
					m_dl->m_dr_r = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
				}
				else
				{
					m_dl->insert(p, m_dl);
				}
			}
			else // DOWN-RIGHT
			{
				if (m_dr == NULL)
				{
					m_dr = new node(p);
					int MIN_X = parent->m_dr_r.m_upleft.m_x;
					int MID_X = p.m_x;
					int MAX_X = parent->m_dr_r.m_downright.m_x;
					int MIN_Y = parent->m_dr_r.m_upleft.m_y;
					int MID_Y = p.m_y;
					int MAX_Y = parent->m_dr_r.m_downright.m_y;

					m_dr->m_ul_r = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
					m_dr->m_ur_r = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
					m_dr->m_dl_r = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
					m_dr->m_dr_r = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
				}
				else
				{
					m_dr->insert(p, m_dr);
				}
			}
		}
	}

	Tpoint find_closest_point(const Tpoint & p, Tpoint & closest, unsigned long long & best_dist) const
	{
		unsigned long long dist = sq_distance(p, m_p);
		if (dist < best_dist)
		{
			best_dist = dist;
			closest = m_p;
		}

		if (best_dist > sq_distance(get_closest_p(m_ul_r, p), p) && m_ul != NULL)
			m_ul->find_closest_point(p, closest, best_dist);
		if (best_dist > sq_distance(get_closest_p(m_ur_r, p), p) && m_ur != NULL)
			m_ur->find_closest_point(p, closest, best_dist);
		if (best_dist > sq_distance(get_closest_p(m_dl_r, p), p) && m_dl != NULL)
			m_dl->find_closest_point(p, closest, best_dist);
		if (best_dist > sq_distance(get_closest_p(m_dr_r, p), p) && m_dr != NULL)
			m_dr->find_closest_point(p, closest, best_dist);

		return closest;
	}

	vector<Tpoint> find_n_closest_points(const Tpoint & p, int n, vector<Tpoint> & found) const
	{
		if (found.size() < n)
		{
			found.push_back(m_p);
			sort(found.begin(), found.end(), [p](const Tpoint & a, const Tpoint & b) { return sq_distance(a, p) < sq_distance(b, p); });
		}
		else
		{
			unsigned long long curr_dist = sq_distance(p, m_p);
			for (vector<Tpoint>::const_iterator it = found.begin(); it != found.end(); ++it)
			{
				if (curr_dist <= sq_distance(p, *it))
				{
					found.insert(it, m_p);
					if (found.size() >= n)
						found.pop_back();
					break;
				}
			}
		}

		if (m_ul != NULL)
		{
			if (sq_distance(get_closest_p(m_ul_r, p), p) < sq_distance(p, found.back()))
				m_ul->find_n_closest_points(p, n, found);
		}
		if (m_ur != NULL)
		{
			if (sq_distance(get_closest_p(m_ur_r, p), p) < sq_distance(p, found.back()))
				m_ur->find_n_closest_points(p, n, found);
		}
		if (m_dl != NULL)
		{
			if (sq_distance(get_closest_p(m_dl_r, p), p) < sq_distance(p, found.back()))
				m_dl->find_n_closest_points(p, n, found);
		}
		if (m_dr != NULL)
		{
			if (sq_distance(get_closest_p(m_dr_r, p), p) < sq_distance(p, found.back()))
				m_dr->find_n_closest_points(p, n, found);
		}

		return found;
	}

	Tpoint m_p;
	Trect m_ul_r, m_ur_r, m_dl_r, m_dr_r;
	node * m_ul, * m_ur, * m_dl,* m_dr;
};

class quad_tree
{
  public:
	  quad_tree()
		  : m_root(NULL)
	  {}
	  ~quad_tree(void)
	  {
		  delete m_root;
	  }

	  void insert(int x, int y)
	  {
		  insert(Tpoint(x, y));
	  }

	  void insert(const Tpoint & p)
	  {
		  if (p.m_x < GMINX || p.m_x >= GMAXX || p.m_y < GMINY || p.m_y >= GMAXY)
			  return;

		  if (m_root == NULL)
		  {
			  m_root = new node(p);
			  int MIN_X = GMINX;
			  int MID_X = p.m_x;
			  int MAX_X = GMAXX;
			  int MIN_Y = GMINY;
			  int MID_Y = p.m_y;
			  int MAX_Y = GMAXY;

			  m_root->m_ul_r = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
			  m_root->m_ur_r = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
			  m_root->m_dl_r = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
			  m_root->m_dr_r = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
		  }
		  else
		  {
			  m_root->insert(p, m_root);
		  }
	  }

	  void clear(void)
	  {
		  delete m_root;
		  m_root = NULL;
	  }

	  Tpoint find_closest_point(const Tpoint & p) const
	  {
		  if (m_root == NULL)
			  return Tpoint();
		  else
		  {
			  unsigned long long best_dist = sq_distance(p, m_root->m_p);
			  Tpoint closest(m_root->m_p);
			  return m_root->find_closest_point(p, closest, best_dist);
		  }
	  }

	  vector<Tpoint> find_n_closest_points(const Tpoint & p, int n)
	  {
		  if (m_root == NULL)
			  return vector<Tpoint>();
		  else
		  {
			  vector<Tpoint> res;
			  return m_root->find_n_closest_points(p, n, res);
		  }
	  }
public:
	node * m_root;
};

class CTimer
{
public:
	CTimer(void)
	{
		start = chrono::system_clock::now();
	}
	double elapsed(void) const
	{
		return (double)(chrono::duration_cast<chrono::milliseconds>
			(chrono::system_clock::now() - start)).count();
	}
private:
	chrono::time_point<chrono::system_clock> start;
};

void draw_rect(Graphics & gfx, const Trect & rect, const Color & c = Colors::Cyan)
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

void draw_point(Graphics & gfx, const Tpoint & p, const Color & c = Colors::White)
{
	if (p.m_x > 1 && p.m_y > 1 &&
		p.m_x < Graphics::ScreenWidth - 1 &&
		p.m_y < Graphics::ScreenHeight - 1)
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
	if (n->m_ul != NULL) draw_node(gfx, n->m_ul, depth + 1);
	if (n->m_ur != NULL) draw_node(gfx, n->m_ur, depth + 1);
	if (n->m_dl != NULL) draw_node(gfx, n->m_dl, depth + 1);
	if (n->m_dr != NULL) draw_node(gfx, n->m_dr, depth + 1);

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

int random_int(int min, int max)
{
	int range = max - min + 1;
	return (rand() % range) + min;
}

quad_tree t;
Tpoint p;
Tpoint closest;
vector<Tpoint> closest_points;

Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	//for (int i = 0; i < 100; ++i)
	//	t.insert(Tpoint(random_int(0, Graphics::ScreenWidth - 1), random_int(0, Graphics::ScreenHeight - 1)));

	p = Tpoint(random_int(0, Graphics::ScreenWidth - 1), random_int(0, Graphics::ScreenHeight - 1));
	closest = t.find_closest_point(p);
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

bool space_pressed = true;

void Game::UpdateModel()
{
	if (wnd.mouse.LeftIsPressed())
	{
		//p = Tpoint(random_int(0, Graphics::ScreenWidth - 1), random_int(0, Graphics::ScreenHeight - 1));
		p = Tpoint(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
		//closest = t.find_closest_point(p);
		closest_points = t.find_n_closest_points(p, 50);
	}

	if (wnd.mouse.RightIsPressed())
	{
		t.insert(Tpoint(wnd.mouse.GetPosX(), wnd.mouse.GetPosY()));
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		//for (int i = 0; i < 100; ++i)
		//	t.insert(Tpoint(random_int(0, Graphics::ScreenWidth), random_int(0, Graphics::ScreenHeight)));

		for (int i = 0; i < 100; ++i)
			t.insert(Tpoint(random_int(0, GMAXX), random_int(0, GMAXY)));
	}

	if (wnd.kbd.KeyIsPressed(VK_CONTROL))
	{
		for (int i = 0; i < 10000; ++i)
			t.insert(Tpoint(random_int(0, GMAXX), random_int(0, GMAXY)));
	}

	if (wnd.kbd.KeyIsPressed(VK_UP))
		draw_rect_bool = true;

	if (wnd.kbd.KeyIsPressed(VK_DOWN))
		draw_rect_bool = false;

	if (wnd.kbd.KeyIsPressed(VK_F5))
	{
		t.clear();
		closest_points.clear();
	}
}

void Game::ComposeFrame()
{
	draw_quad_tree(gfx, t);
	
	draw_point(gfx, p, Colors::Green);
	//draw_point(gfx, closest, Colors::White);
	for (const auto & i : closest_points)
		draw_point(gfx, i, Colors::Cyan);
}
