#include "quad_tree.h"

//-----------------------------------------------------------------------------
quad_tree::quad_tree(void)
	: m_root(NULL)
{}

quad_tree::~quad_tree(void)
{
	delete m_root;
}

quad_tree::quad_tree(const quad_tree & src)
{
	if (src.m_root == NULL)
		m_root = NULL;
	else
		m_root = new node(*src.m_root);
}

quad_tree & quad_tree::operator = (const quad_tree & src)
{
	if (this == &src)
		return *this;
	clear();
	if (src.m_root != NULL)
		m_root = new node(*src.m_root);
	return *this;
}

void quad_tree::insert(int x, int y)
{
	insert(Tpoint<int>(x, y));
}

void quad_tree::insert(const Tpoint<int> & p)
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

		m_root->m_ul_r = Trect<int>(Tpoint<int>(MIN_X, MIN_Y), Tpoint<int>(MID_X, MID_Y));
		m_root->m_ur_r = Trect<int>(Tpoint<int>(MID_X, MIN_Y), Tpoint<int>(MAX_X, MID_Y));
		m_root->m_dl_r = Trect<int>(Tpoint<int>(MIN_X, MID_Y), Tpoint<int>(MID_X, MAX_Y));
		m_root->m_dr_r = Trect<int>(Tpoint<int>(MID_X, MID_Y), Tpoint<int>(MAX_X, MAX_Y));
	}
	else
	{
		m_root->insert(p, m_root);
	}
}

void quad_tree::clear(void)
{
	delete m_root;
	m_root = NULL;
}

Tpoint<int> quad_tree::find_closest_point(const Tpoint<int> & p) const
{
	if (m_root == NULL)
		return Tpoint<int>();
	else
	{
		unsigned long long best_dist = sq_distance(p, m_root->m_p);
		Tpoint<int> closest(m_root->m_p);
		return m_root->find_closest_point(p, closest, best_dist);
	}
}

vector<Tpoint<int>> quad_tree::find_n_closest_points(const Tpoint<int> & p, int n)
{
	if (m_root == NULL)
		return vector<Tpoint<int>>();
	else
	{
		vector<Tpoint<int>> res;
		return m_root->find_n_closest_points(p, n, res);
	}
}

void quad_tree::draw(Graphics & gfx, bool draw_rect) const
{
	if (m_root != NULL) m_root->draw(gfx, draw_rect);
}
//-----------------------------------------------------------------------------
