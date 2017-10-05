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

//#define GMIN numeric_limits<int>::min()
//#define GMAX numeric_limits<int>::max()

//#define GMIN -100
//#define GMAX 100

#define GMIN 0
#define GMAX (Graphics::ScreenHeight - 1)

double my_distance(const Tpoint & a, const Tpoint & b)
{
	return sqrt((double)(a.m_x - b.m_x) * (double)(a.m_x - b.m_x)
		      + (double)(a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
int sq_distance(const Tpoint & a, const Tpoint & b)
{
	return (a.m_x - b.m_x) * (a.m_x - b.m_x)
		 + (a.m_y - b.m_y) * (a.m_y - b.m_y);
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

class odd_node;
class even_node
{
public:
	even_node(const Tpoint & p = {})
		: m_p(p), m_top(NULL), m_bot(NULL)
	{}
	~even_node(void);
	void insert(const Tpoint & p, const Trect & lrect, const Trect & rrect);
	Tpoint find_closest_point(const Tpoint & p, Tpoint & closest, int & best_dist) const;

	Tpoint m_p;
	Trect m_trect;
	Trect m_brect;
	odd_node * m_top;
	odd_node * m_bot;
};

class odd_node
{
public:
	odd_node(const Tpoint & p = {})
		: m_p(p), m_left(NULL), m_right(NULL)
	{}
	~odd_node(void)
	{
		delete m_left;
		delete m_right;
	}

	void insert(const Tpoint & p, const Trect & lrect, const Trect & rrect);

	Tpoint find_closest_point(const Tpoint & p, Tpoint & closest, int & best_dist) const
	{
		int dist = sq_distance(p, m_p);
		if (dist < best_dist)
		{
			best_dist = dist;
			closest = m_p;
		}

		if (best_dist > sq_distance(get_closest_p(m_lrect, p), p) && m_left != NULL)
			m_left->find_closest_point(p, closest, best_dist);
		if (best_dist > sq_distance(get_closest_p(m_rrect, p), p) && m_right != NULL)
			m_right->find_closest_point(p, closest, best_dist);
		return closest;
	}

	Tpoint m_p;
	Trect m_lrect;
	Trect m_rrect;
	even_node * m_left;
	even_node * m_right;
};

even_node::~even_node(void)
{
	delete m_top;
	delete m_bot;
}

void odd_node::insert(const Tpoint & p, const Trect & lrect, const Trect & rrect)
{
	if (p.m_x == m_p.m_x && p.m_y == m_p.m_y)
		return;

	if (p.m_x < m_p.m_x)
	{
		if (m_left == NULL)
		{
			m_left = new even_node(p);

			Tpoint ta(lrect.m_upleft);
			Tpoint tb(lrect.m_downright.m_x, p.m_y);
			Tpoint ba(lrect.m_upleft.m_x, p.m_y);
			Tpoint bb(lrect.m_downright);

			Trect trect(ta, tb);
			Trect brect(ba, bb);
			m_left->m_trect = trect;
			m_left->m_brect = brect;
		}
		else
		{
			m_left->insert(p, m_left->m_trect, m_left->m_brect);
		}
	}
	else
	{
		if (m_right == NULL)
		{
			m_right = new even_node(p);

			Tpoint ta(rrect.m_upleft);
			Tpoint tb(rrect.m_downright.m_x, p.m_y);
			Tpoint ba(rrect.m_upleft.m_x, p.m_y);
			Tpoint bb(rrect.m_downright);

			Trect trect(ta, tb);
			Trect brect(ba, bb);
			m_right->m_trect = trect;
			m_right->m_brect = brect;
		}
		else
		{
			m_right->insert(p, m_right->m_trect, m_right->m_brect);
		}
	}
}

void even_node::insert(const Tpoint & p, const Trect & trect, const Trect & brect)
{
	if (p.m_x == m_p.m_x && p.m_y == m_p.m_y)
		return;
	if (p.m_y > m_p.m_y)
	{
		if (m_bot == NULL)
		{
			m_bot = new odd_node(p);

			Tpoint la(brect.m_upleft);
			Tpoint lb(p.m_x, brect.m_downright.m_y);
			Tpoint ra(p.m_x, brect.m_upleft.m_y);
			Tpoint rb(brect.m_downright);

			Trect lrect(la, lb);
			Trect rrect(ra, rb);
			m_bot->m_lrect = lrect;
			m_bot->m_rrect = rrect;
		}
		else
		{
			m_bot->insert(p, m_bot->m_lrect, m_bot->m_rrect);
		}
	}
	else
	{
		if (m_top == NULL)
		{
			m_top = new odd_node(p);

			Tpoint la(trect.m_upleft);
			Tpoint lb(p.m_x, trect.m_downright.m_y);
			Tpoint ra(p.m_x, trect.m_upleft.m_y);
			Tpoint rb(trect.m_downright);

			Trect lrect(la, lb);
			Trect rrect(ra, rb);
			m_top->m_lrect = lrect;
			m_top->m_rrect = rrect;
		}
		else
		{
			m_top->insert(p, m_top->m_lrect, m_top->m_rrect);
		}
	}
}

Tpoint even_node::find_closest_point(const Tpoint & p, Tpoint & closest, int & best_dist) const
{
	int dist = sq_distance(p, m_p);
	if (dist < best_dist)
	{
		best_dist = dist;
		closest = m_p;
	}

	if (best_dist > sq_distance(get_closest_p(m_brect, p), p) && m_bot != NULL)
		m_bot->find_closest_point(p, closest, best_dist);
	if (best_dist > sq_distance(get_closest_p(m_trect, p), p) && m_top != NULL)
		m_top->find_closest_point(p, closest, best_dist);
	
	return closest;
}

class kdtree
{
public:
	kdtree(void)
		: m_root(NULL)
	{}
	~kdtree(void)
	{
		delete m_root;
	}

	void insert(int x, int y)
	{
		insert(Tpoint(x, y));
	}

	void insert(const Tpoint & p)
	{
		if (m_root == NULL)
		{
			Tpoint la(0, 0);
			Tpoint lb(p.m_x, Graphics::ScreenHeight - 1);
			Tpoint ra(p.m_x, 0);
			Tpoint rb(Graphics::ScreenWidth - 1, Graphics::ScreenHeight - 1);
			Trect lrect(la, lb);
			Trect rrect(ra, rb);

			if (p.m_x < lrect.m_upleft.m_x ||
				p.m_x > rrect.m_downright.m_x ||
				p.m_y < lrect.m_upleft.m_y ||
				p.m_y > rrect.m_downright.m_y)
				return;

			m_root = new odd_node(p);

			m_root->m_lrect = lrect;
			m_root->m_rrect = rrect;
		}
		else
		{
			if (p.m_x < m_root->m_lrect.m_upleft.m_x ||
				p.m_x > m_root->m_rrect.m_downright.m_x ||
				p.m_y < m_root->m_lrect.m_upleft.m_y ||
				p.m_y > m_root->m_rrect.m_downright.m_y)
				return;
			m_root->insert(p, m_root->m_lrect, m_root->m_rrect);
		}
	}

	void erase(const Tpoint & p)
	{
		if (m_root == NULL)
			return;
		else
		{
			return;
		}
	}

	Tpoint find_closest_point(const Tpoint & p) const
	{
		if (m_root == NULL)
			return Tpoint();
		else
		{
			Tpoint closest = m_root->m_p;
			int best_dist = sq_distance(p, m_root->m_p);
			m_root->find_closest_point(p, closest, best_dist);
			return closest;
		}
	}

	odd_node * m_root;
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

void draw_odd_node(Graphics & gfx, const odd_node * n);
void draw_even_node(Graphics & gfx, const even_node * n)
{
	if (n->m_bot != NULL)
		draw_odd_node(gfx, n->m_bot);
	if (n->m_top != NULL)
		draw_odd_node(gfx, n->m_top);

	if (draw_rect_bool == true)
	{
		draw_rect(gfx, n->m_brect);
		draw_rect(gfx, n->m_trect);
	}
	draw_point(gfx, n->m_p, Colors::Red);
}

void draw_odd_node(Graphics & gfx, const odd_node * n)
{
	if (n->m_left != NULL)
		draw_even_node(gfx, n->m_left);
	if (n->m_right != NULL)
		draw_even_node(gfx, n->m_right);

	if (draw_rect_bool == true)
	{
		draw_rect(gfx, n->m_lrect);
		draw_rect(gfx, n->m_rrect);
	}
	draw_point(gfx, n->m_p, Colors::Blue);
}

void draw_kdtree(Graphics & gfx, const kdtree & t)
{
	if (t.m_root == NULL)
		return;
	else
	{
		draw_odd_node(gfx, t.m_root);
	}
}

int random_int(int min, int max)
{
	int range = max - min + 1;
	return (rand() % range) + min;
}

kdtree t;
Tpoint p;
Tpoint closest;

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	srand(unsigned(time(0)));

	for (int i = 0; i < 100; ++i)
		t.insert(Tpoint(random_int(0, Graphics::ScreenWidth - 1), random_int(0, Graphics::ScreenHeight - 1)));

	p = Tpoint(random_int(0, Graphics::ScreenWidth - 1), random_int(0, Graphics::ScreenHeight - 1));
	closest = t.find_closest_point(p);
}

void Game::Go()
{
	if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
		exit(0);
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
	draw_kdtree(gfx, t);
	
	draw_point(gfx, p, Colors::Green);
	draw_point(gfx, closest, Colors::White);

	//draw_rect(gfx, mr);
	//Tpoint p(wnd.mouse.GetPosX(), wnd.mouse.GetPosY());
	//cl = get_closest_p(mr, p);
	//draw_point(gfx, p, Colors::Red);
	//draw_point(gfx, cl, Colors::Blue);
	
}
