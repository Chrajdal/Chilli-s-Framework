#include "quad_tree_f.h"

//-----------------------------------------------------------------------------
quad_tree_f::quad_tree_f(void)
	: m_root(NULL)
{}

quad_tree_f::quad_tree_f(const quad_tree_f & src)
{
	if (src.m_root == NULL)
		m_root = NULL;
	else
		m_root = new node_f(*src.m_root);
}

quad_tree_f & quad_tree_f::operator = (const quad_tree_f & src)
{
	if (this == &src)
		return *this;
	clear();
	if (src.m_root != NULL)
		m_root = new node_f(*src.m_root);
	return *this;
}

quad_tree_f & quad_tree_f::operator = (quad_tree_f && src)
{
	if (this == &src)
		return *this;
	clear();
	if (src.m_root == NULL)
	{
		m_root = NULL;
		return *this;
	}
	else
	{
		m_root = src.m_root;
		src.m_root = NULL;
		return *this;
	}
	return *this;
}


quad_tree_f::~quad_tree_f(void)
{
	delete m_root;
}

void quad_tree_f::insert(float x, float y)
{
	insert(Tpoint<float>(x, y));
}

void quad_tree_f::insert(const Tpoint<float> & p)
{
	//if (p.m_x < GMINX || p.m_x >= GMAXX || p.m_y < GMINY || p.m_y >= GMAXY)
	//	return;

	if (m_root == NULL)
	{
		m_root = new node_f(p);
		float MIN_X = GMINX;
		float MID_X = p.m_x;
		float MAX_X = GMAXX;
		float MIN_Y = GMINY;
		float MID_Y = p.m_y;
		float MAX_Y = GMAXY;

		m_root->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
		m_root->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
		m_root->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
		m_root->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
	}
	else
	{
		m_root->insert(p, m_root);
	}
}

void quad_tree_f::insert(const node_f & n)
{
	if (n.m_p.m_x < GMINX || n.m_p.m_x >= GMAXX || n.m_p.m_y < GMINY || n.m_p.m_y >= GMAXY)
		return;

	if (m_root == NULL)
	{
		m_root = new node_f(n.m_p);
		m_root->m_velocity = n.m_velocity;

		float MIN_X = GMINX;
		float MID_X = n.m_p.m_x;
		float MAX_X = GMAXX;
		float MIN_Y = GMINY;
		float MID_Y = n.m_p.m_y;
		float MAX_Y = GMAXY;

		m_root->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
		m_root->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
		m_root->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
		m_root->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
	}
	else
	{
		m_root->insert(n, m_root);
	}
}

void quad_tree_f::clear(void)
{
	delete m_root;
	m_root = NULL;
}

Tpoint<float> quad_tree_f::find_closest_point(const Tpoint<float> & p) const
{
	if (m_root == NULL)
		return Tpoint<float>();
	else
	{
		double best_dist = sq_distance(p, m_root->m_p);
		Tpoint<float> closest(m_root->m_p);
		return m_root->find_closest_point(p, closest, best_dist);
	}
}

void quad_tree_f::find_n_closest_points(const Tpoint<float> & p, int n, vector<node_f> & out)
{
	if (m_root != NULL)
		return m_root->find_n_closest_points(p, n, out);
}

void quad_tree_f::draw(Graphics & gfx, bool draw_rect) const
{
	if (m_root != NULL) m_root->draw(gfx, draw_rect);
}

int quad_tree_f::size(void) const
{
	if (m_root == NULL)
		return 0;
	else
	{
		int sz = 1;
		return m_root->size(sz);
	}
}
//-----------------------------------------------------------------------------
