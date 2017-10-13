#include <iostream>
#include <iomanip>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <vector>
#include <deque>
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

#define GMIN 0
#define GMAX (Graphics::ScreenHeight - 1)

double my_distance(const Tpoint & a, const Tpoint & b)
{
	return sqrt((double)(a.m_x - b.m_x) * (double)(a.m_x - b.m_x)
		      + (double)(a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
unsigned long long sq_distance(const Tpoint & a, const Tpoint & b)
{
	return (unsigned long long)(a.m_x - b.m_x) * (unsigned long long)(a.m_x - b.m_x)
		 + (unsigned long long)(a.m_y - b.m_y) * (unsigned long long)(a.m_y - b.m_y);
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
		m_upleft_rect(),
		m_upright_rect(),
		m_downleft_rect(),
		m_downright_rect(),
		m_upleft(NULL), m_upright(NULL), m_downleft(NULL), m_downright(NULL)
	{}

	~node(void)
	{
		delete m_upleft;
		delete m_upright;
		delete m_downleft;
		delete m_downright;
	}

	void insert(const Tpoint & p, const node * parent)
	{
		if (m_p.m_x == p.m_x && m_p.m_y == p.m_y)
			return;

		if (p.m_y < m_p.m_y) // UP
		{
			if (p.m_x < m_p.m_x) // UP-LEFT
			{
				if (m_upleft == NULL)
				{
					m_upleft = new node(p);
					int MIN_X = parent->m_upleft_rect.m_upleft.m_x;
					int MID_X = p.m_x;
					int MAX_X = parent->m_upleft_rect.m_downright.m_x;
					int MIN_Y = parent->m_upleft_rect.m_upleft.m_y;
					int MID_Y = p.m_y;
					int MAX_Y = parent->m_upleft_rect.m_downright.m_y;

					m_upleft->m_upleft_rect = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
					m_upleft->m_upright_rect = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
					m_upleft->m_downleft_rect = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
					m_upleft->m_downright_rect = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
				}
				else
				{
					m_upleft->insert(p, m_upleft);
				}
			}
			else // (p.m_x >= m_p.m_x) // UP-RIGHT
			{
				if (m_upright == NULL)
				{
					m_upright = new node(p);
					int MIN_X = parent->m_upright_rect.m_upleft.m_x;
					int MID_X = p.m_x;
					int MAX_X = parent->m_upright_rect.m_downright.m_x;
					int MIN_Y = parent->m_upright_rect.m_upleft.m_y;
					int MID_Y = p.m_y;
					int MAX_Y = parent->m_upright_rect.m_downright.m_y;

					m_upright->m_upleft_rect = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
					m_upright->m_upright_rect = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
					m_upright->m_downleft_rect = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
					m_upright->m_downright_rect = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
				}
				else
				{
					m_upright->insert(p, m_upright);
				}
			}
		}
		else // DOWN
		{
			if (p.m_x < m_p.m_x) // DOWN-LEFT
			{
				if (m_downleft == NULL)
				{
					m_downleft = new node(p);
					int MIN_X = parent->m_downleft_rect.m_upleft.m_x;
					int MID_X = p.m_x;
					int MAX_X = parent->m_downleft_rect.m_downright.m_x;
					int MIN_Y = parent->m_downleft_rect.m_upleft.m_y;
					int MID_Y = p.m_y;
					int MAX_Y = parent->m_downleft_rect.m_downright.m_y;

					m_downleft->m_upleft_rect = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
					m_downleft->m_upright_rect = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
					m_downleft->m_downleft_rect = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
					m_downleft->m_downright_rect = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
				}
				else
				{
					m_downleft->insert(p, m_downleft);
				}
			}
			else // DOWN-RIGHT
			{
				if (m_downright == NULL)
				{
					m_downright = new node(p);
					int MIN_X = parent->m_downright_rect.m_upleft.m_x;
					int MID_X = p.m_x;
					int MAX_X = parent->m_downright_rect.m_downright.m_x;
					int MIN_Y = parent->m_downright_rect.m_upleft.m_y;
					int MID_Y = p.m_y;
					int MAX_Y = parent->m_downright_rect.m_downright.m_y;

					m_downright->m_upleft_rect = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
					m_downright->m_upright_rect = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
					m_downright->m_downleft_rect = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
					m_downright->m_downright_rect = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
				}
				else
				{
					m_downright->insert(p, m_downright);
				}
			}
		}
	}

	Tpoint find_closest_point(const Tpoint & p, Tpoint & closest, unsigned long long & best_dist) const
	{
		int dist = sq_distance(p, m_p);
		if (dist <= best_dist)
		{
			best_dist = dist;
			closest = m_p;
		}

		if (best_dist > sq_distance(get_closest_p(m_upleft_rect, p), p) && m_upleft != NULL)
			m_upleft->find_closest_point(p, closest, best_dist);
		if (best_dist > sq_distance(get_closest_p(m_upright_rect, p), p) && m_upright != NULL)
			m_upright->find_closest_point(p, closest, best_dist);
		if (best_dist > sq_distance(get_closest_p(m_downleft_rect, p), p) && m_downleft != NULL)
			m_downleft->find_closest_point(p, closest, best_dist);
		if (best_dist > sq_distance(get_closest_p(m_downright_rect, p), p) && m_downright != NULL)
			m_downright->find_closest_point(p, closest, best_dist);

		return closest;
	}

	/*
	friend ostream & operator <<(ostream & os, const node & src)
	{
		os << src.m_p << " ----- " << src.m_upleft_rect << " / " << src.m_upright_rect << " / " <<
			src.m_downleft_rect << " / " << src.m_downright_rect << endl;
		if (src.m_upleft != NULL)
			os << *src.m_upleft;
		if (src.m_upright != NULL)
			os << *src.m_upright;
		if (src.m_downleft != NULL)
			os << *src.m_downleft;
		if (src.m_downright != NULL)
			os << *src.m_downright;
		return os;
	}
	*/
	Tpoint m_p;
	Trect m_upleft_rect, m_upright_rect, m_downleft_rect, m_downright_rect;
	node * m_upleft;
	node * m_upright;
	node * m_downleft;
	node * m_downright;
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
		  if (p.m_x < GMIN || p.m_x > GMAX || p.m_y < GMIN || p.m_y > GMAX)
			  return;

		  if (m_root == NULL)
		  {
			  m_root = new node(p);
			  int MIN_X = GMIN;
			  int MID_X = p.m_x;
			  int MAX_X = GMAX;
			  int MIN_Y = GMIN;
			  int MID_Y = p.m_y;
			  int MAX_Y = GMAX;

			  m_root->m_upleft_rect = Trect(Tpoint(MIN_X, MIN_Y), Tpoint(MID_X, MID_Y));
			  m_root->m_upright_rect = Trect(Tpoint(MID_X, MIN_Y), Tpoint(MAX_X, MID_Y));
			  m_root->m_downleft_rect = Trect(Tpoint(MIN_X, MID_Y), Tpoint(MID_X, MAX_Y));
			  m_root->m_downright_rect = Trect(Tpoint(MID_X, MID_Y), Tpoint(MAX_X, MAX_Y));
		  }
		  else
		  {
			  m_root->insert(p, m_root);
		  }
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

	  /*
	  friend ostream & operator << (ostream & os, const quad_tree & src)
	  {
		  if (src.m_root != NULL)
			  os << *src.m_root;
		  return os;
	  }
	  */
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

void draw_point(Graphics & gfx, const Tpoint & p, const Color & c)
{
	if (p.m_x > 1 && p.m_y > 1 &&
		p.m_x < Graphics::ScreenWidth - 1 &&
		p.m_y < Graphics::ScreenHeight - 1)
	{
		gfx.PutPixel(p.m_x - 1, p.m_y - 1, c);
		gfx.PutPixel(p.m_x - 1, p.m_y + 1, c);
		gfx.PutPixel(p.m_x + 1, p.m_y - 1, c);
		gfx.PutPixel(p.m_x + 1, p.m_y + 1, c);

		gfx.PutPixel(p.m_x, p.m_y, c);
	}
}

bool draw_rect_bool = false;

void draw_node(Graphics & gfx, const node const * n)
{
	if (n->m_upleft != NULL)
		draw_node(gfx, n->m_upleft);
	if (n->m_upright != NULL)
		draw_node(gfx, n->m_upright);
	if (n->m_downleft != NULL)
		draw_node(gfx, n->m_downleft);
	if (n->m_downright != NULL)
		draw_node(gfx, n->m_downright);

	if (draw_rect_bool == true)
	{
		draw_rect(gfx, n->m_upleft_rect, Colors::Red);
		draw_rect(gfx, n->m_upright_rect, Colors::Green);
		draw_rect(gfx, n->m_downleft_rect, Colors::Blue);
		draw_rect(gfx, n->m_downright_rect, Colors::Yellow);
	}
	draw_point(gfx, n->m_p, Colors::White);
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

Game::Game(MainWindow& wnd)
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
		closest = t.find_closest_point(p);
	}

	if (wnd.mouse.RightIsPressed())
	{
		t.insert(Tpoint(wnd.mouse.GetPosX(), wnd.mouse.GetPosY()));
	}

	if (wnd.kbd.KeyIsPressed(VK_SPACE))
	{
		for (int i = 0; i < 100; ++i)
			t.insert(Tpoint(random_int(0, Graphics::ScreenWidth), random_int(0, Graphics::ScreenHeight)));
	}

	if (wnd.kbd.KeyIsPressed(VK_UP))
		draw_rect_bool = true;

	if (wnd.kbd.KeyIsPressed(VK_DOWN))
		draw_rect_bool = false;
}

Trect mr(Tpoint(100, 100), Tpoint(300, 300));
Tpoint cl;

void Game::ComposeFrame()
{
	draw_quad_tree(gfx, t);
	
	draw_point(gfx, p, Colors::Green);
	draw_point(gfx, closest, Colors::Red);

	//draw_rect(gfx, mr);
	//Tpoint p(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
	//cl = get_closest_p(mr, p);
	//draw_point(gfx, p, Colors::Red);
	//draw_point(gfx, cl, Colors::Blue);
	
}
