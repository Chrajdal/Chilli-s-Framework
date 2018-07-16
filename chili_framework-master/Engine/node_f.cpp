#include "node_f.h"

//-----------------------------------------------------------------------------
node_f::node_f(const Tpoint<float> & p,
	const Trect<float> upleft_rect,
	const Trect<float> upright_rect,
	const Trect<float> downleft_rect,
	const Trect<float> downright_rect)
	: m_p(p),
	m_ul_r(upleft_rect),
	m_ur_r(upright_rect),
	m_dl_r(downleft_rect),
	m_dr_r(downright_rect),
	m_ul(NULL), m_ur(NULL), m_dl(NULL), m_dr(NULL)
{}

node_f::node_f(const node_f & src)
	: m_p(src.m_p),
	m_ul_r(src.m_ul_r),
	m_ur_r(src.m_ur_r),
	m_dl_r(src.m_dl_r),
	m_dr_r(src.m_dr_r),
	m_velocity(src.m_velocity),
	m_ul(NULL), m_ur(NULL), m_dl(NULL), m_dr(NULL)
{
	if (src.m_ul != NULL) m_ul = new node_f(*src.m_ul);
	if (src.m_ur != NULL) m_ur = new node_f(*src.m_ur);
	if (src.m_dl != NULL) m_dl = new node_f(*src.m_dl);
	if (src.m_dr != NULL) m_dr = new node_f(*src.m_dr);
}

node_f & node_f::operator = (const node_f & src)
{
	if (this == &src)
		return *this;

	m_p = src.m_p;
	m_ul_r = src.m_ul_r;
	m_ur_r = src.m_ur_r;
	m_dl_r = src.m_dl_r;
	m_dr_r = src.m_dr_r;
	m_velocity = src.m_velocity;

	delete m_ul; m_ul = NULL;
	delete m_ur; m_ur = NULL;
	delete m_dl; m_dl = NULL;
	delete m_dr; m_dr = NULL;

	if (src.m_ul != NULL) m_ul = new node_f(*src.m_ul);
	if (src.m_ur != NULL) m_ur = new node_f(*src.m_ur);
	if (src.m_dl != NULL) m_dl = new node_f(*src.m_dl);
	if (src.m_dr != NULL) m_dr = new node_f(*src.m_dr);

	return *this;
}

node_f::node_f(const node_f && src)
	:m_p(move(src.m_p)),
	m_ul_r(move(src.m_ul_r)),
	m_ur_r(move(src.m_ur_r)),
	m_dl_r(move(src.m_dl_r)),
	m_dr_r(move(src.m_dr_r)),
	m_velocity(move(src.m_velocity)),
	m_ul(src.m_ul), m_ur(src.m_ur), m_dl(src.m_dl), m_dr(src.m_dr)
{
}

node_f & node_f::operator = (const node_f && src)
{
	if (this == &src)
		return *this;

	m_p = move(src.m_p);
	m_ul_r = move(src.m_ul_r);
	m_ur_r = move(src.m_ur_r);
	m_dl_r = move(src.m_dl_r);
	m_dr_r = move(src.m_dr_r);
	m_velocity = move(src.m_velocity);

	delete m_ul; m_ul = NULL;
	delete m_ur; m_ur = NULL;
	delete m_dl; m_dl = NULL;
	delete m_dr; m_dr = NULL;

	m_ul = src.m_ul;
	m_ur = src.m_ur;
	m_dl = src.m_dl;
	m_dr = src.m_dr;

	return *this;
}

node_f::~node_f(void)
{
	delete m_ul; m_ul = NULL;
	delete m_ur; m_ur = NULL;
	delete m_dl; m_dl = NULL;
	delete m_dr; m_dr = NULL;
}

void node_f::insert(const Tpoint<float> & p, const node_f * parent)
{
	if (m_p.m_x == p.m_x && m_p.m_y == p.m_y)
		return;

	if (p.m_y < m_p.m_y) // UP
	{
		if (p.m_x < m_p.m_x) // UP-LEFT
		{
			if (m_ul == NULL)
			{
				m_ul = new node_f(p);
				float MIN_X = parent->m_ul_r.m_upleft.m_x;
				float MID_X = p.m_x;
				float MAX_X = parent->m_ul_r.m_downright.m_x;
				float MIN_Y = parent->m_ul_r.m_upleft.m_y;
				float MID_Y = p.m_y;
				float MAX_Y = parent->m_ul_r.m_downright.m_y;

				m_ul->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
				m_ul->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
				m_ul->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
				m_ul->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
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
				m_ur = new node_f(p);
				float MIN_X = parent->m_ur_r.m_upleft.m_x;
				float MID_X = p.m_x;
				float MAX_X = parent->m_ur_r.m_downright.m_x;
				float MIN_Y = parent->m_ur_r.m_upleft.m_y;
				float MID_Y = p.m_y;
				float MAX_Y = parent->m_ur_r.m_downright.m_y;

				m_ur->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
				m_ur->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
				m_ur->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
				m_ur->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
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
				m_dl = new node_f(p);
				float MIN_X = parent->m_dl_r.m_upleft.m_x;
				float MID_X = p.m_x;
				float MAX_X = parent->m_dl_r.m_downright.m_x;
				float MIN_Y = parent->m_dl_r.m_upleft.m_y;
				float MID_Y = p.m_y;
				float MAX_Y = parent->m_dl_r.m_downright.m_y;

				m_dl->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
				m_dl->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
				m_dl->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
				m_dl->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
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
				m_dr = new node_f(p);
				float MIN_X = parent->m_dr_r.m_upleft.m_x;
				float MID_X = p.m_x;
				float MAX_X = parent->m_dr_r.m_downright.m_x;
				float MIN_Y = parent->m_dr_r.m_upleft.m_y;
				float MID_Y = p.m_y;
				float MAX_Y = parent->m_dr_r.m_downright.m_y;

				m_dr->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
				m_dr->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
				m_dr->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
				m_dr->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
			}
			else
			{
				m_dr->insert(p, m_dr);
			}
		}
	}
}

void node_f::insert(const node_f & n, const node_f * parent)
{
	if (m_p.m_x == n.m_p.m_x && m_p.m_y == n.m_p.m_y)
		return;

	if (n.m_p.m_y < m_p.m_y) // UP
	{
		if (n.m_p.m_x < m_p.m_x) // UP-LEFT
		{
			if (m_ul == NULL)
			{
				m_ul = new node_f(n.m_p);
				m_ul->m_velocity = n.m_velocity;

				float MIN_X = parent->m_ul_r.m_upleft.m_x;
				float MID_X = n.m_p.m_x;
				float MAX_X = parent->m_ul_r.m_downright.m_x;
				float MIN_Y = parent->m_ul_r.m_upleft.m_y;
				float MID_Y = n.m_p.m_y;
				float MAX_Y = parent->m_ul_r.m_downright.m_y;

				m_ul->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
				m_ul->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
				m_ul->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
				m_ul->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
			}
			else
			{
				m_ul->insert(n, m_ul);
			}
		}
		else // (p.m_x >= m_p.m_x) // UP-RIGHT
		{
			if (m_ur == NULL)
			{
				m_ur = new node_f(n.m_p);
				m_ur->m_velocity = n.m_velocity;

				float MIN_X = parent->m_ur_r.m_upleft.m_x;
				float MID_X = n.m_p.m_x;
				float MAX_X = parent->m_ur_r.m_downright.m_x;
				float MIN_Y = parent->m_ur_r.m_upleft.m_y;
				float MID_Y = n.m_p.m_y;
				float MAX_Y = parent->m_ur_r.m_downright.m_y;

				m_ur->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
				m_ur->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
				m_ur->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
				m_ur->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
			}
			else
			{
				m_ur->insert(n, m_ur);
			}
		}
	}
	else // DOWN
	{
		if (n.m_p.m_x < m_p.m_x) // DOWN-LEFT
		{
			if (m_dl == NULL)
			{
				m_dl = new node_f(n.m_p);
				m_dl->m_velocity = n.m_velocity;

				float MIN_X = parent->m_dl_r.m_upleft.m_x;
				float MID_X = n.m_p.m_x;
				float MAX_X = parent->m_dl_r.m_downright.m_x;
				float MIN_Y = parent->m_dl_r.m_upleft.m_y;
				float MID_Y = n.m_p.m_y;
				float MAX_Y = parent->m_dl_r.m_downright.m_y;

				m_dl->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
				m_dl->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
				m_dl->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
				m_dl->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
			}
			else
			{
				m_dl->insert(n, m_dl);
			}
		}
		else // DOWN-RIGHT
		{
			if (m_dr == NULL)
			{
				m_dr = new node_f(n.m_p);
				m_dr->m_velocity = n.m_velocity;

				float MIN_X = parent->m_dr_r.m_upleft.m_x;
				float MID_X = n.m_p.m_x;
				float MAX_X = parent->m_dr_r.m_downright.m_x;
				float MIN_Y = parent->m_dr_r.m_upleft.m_y;
				float MID_Y = n.m_p.m_y;
				float MAX_Y = parent->m_dr_r.m_downright.m_y;

				m_dr->m_ul_r = {{MIN_X, MIN_Y}, {MID_X, MID_Y}};
				m_dr->m_ur_r = {{MID_X, MIN_Y}, {MAX_X, MID_Y}};
				m_dr->m_dl_r = {{MIN_X, MID_Y}, {MID_X, MAX_Y}};
				m_dr->m_dr_r = {{MID_X, MID_Y}, {MAX_X, MAX_Y}};
			}
			else
			{
				m_dr->insert(n, m_dr);
			}
		}
	}
}


Tpoint<float> get_closest_p(const Trect<float> & rect, const Tpoint<float> & p)
{
	//  1 | 2 | 3
	// ---+---+---
	//  4 | 5 | 6
	// ---+---+---
	//  7 | 8 | 9

	if (p.m_x < rect.m_upleft.m_x)
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 1
			return Tpoint<float>(rect.m_upleft);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 7
			return Tpoint<float>(rect.m_upleft.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 4
			return Tpoint<float>(rect.m_upleft.m_x, p.m_y);
		}
	}
	else if (p.m_x > rect.m_downright.m_x)
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 3
			return Tpoint<float>(rect.m_downright.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 9
			return Tpoint<float>(rect.m_downright);
		}
		else
		{	// no.: 6
			return Tpoint<float>(rect.m_downright.m_x, p.m_y);
		}
	}
	else
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 2
			return Tpoint<float>(p.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 8
			return Tpoint<float>(p.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 5
			return p;
		}
	}
}

Tpoint<float> node_f::find_closest_point(const Tpoint<float> & p, Tpoint<float> & closest, double & best_dist) const
{
	double dist = sq_distance(p, m_p);
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

void node_f::find_n_closest_points(const Tpoint<float> & p, int n, vector<node_f> & found) const
{
	if (found.size() < n)
	{
		if (found.empty())
			found.push_back(m_p);
		else
		{
			auto it = lower_bound(found.begin(), found.end(), m_p,
				[p](const node_f & a, const node_f & b)
			{ return sq_distance(a.m_p, p) <= sq_distance(b.m_p, p); });
			found.insert(it, m_p);
		}
	}
	else
	{
		double curr_dist = sq_distance(p, m_p);
		for (auto it = found.begin(); it != found.end(); ++it)
		{
			if (curr_dist <= sq_distance(p, it->m_p))
			{
				found.insert(it, m_p);
				if (found.size() >= n)
					found.pop_back();
				break;
			}
		}
	}

	if (m_ul != NULL)
		if (sq_distance(get_closest_p(m_ul_r, p), p) <= sq_distance(p, found.back().m_p))
			m_ul->find_n_closest_points(p, n, found);
	if (m_ur != NULL)
		if (sq_distance(get_closest_p(m_ur_r, p), p) <= sq_distance(p, found.back().m_p))
			m_ur->find_n_closest_points(p, n, found);
	if (m_dl != NULL)
		if (sq_distance(get_closest_p(m_dl_r, p), p) <= sq_distance(p, found.back().m_p))
			m_dl->find_n_closest_points(p, n, found);
	if (m_dr != NULL)
		if (sq_distance(get_closest_p(m_dr_r, p), p) <= sq_distance(p, found.back().m_p))
			m_dr->find_n_closest_points(p, n, found);
}

void node_f::draw(Graphics & gfx, bool draw_rect) const
{
	//gfx.PutPixel(m_p.m_x, m_p.m_y, Colors::Gray);
	if (m_p.m_x >= 10 && m_p.m_y >= 10 &&
		m_p.m_x < Graphics::ScreenWidth - 10 &&
		m_p.m_y < Graphics::ScreenHeight - 10)
		gfx.draw_circle((int)m_p.m_x, (int)m_p.m_y, 5, Colors::Gray);
	if (draw_rect == true)
	{
		gfx.draw_line((int)m_p.m_x, (int)m_p.m_y, (int)m_p.m_x               , (int)m_ul_r.m_upleft.m_y, Colors::Green);
		gfx.draw_line((int)m_p.m_x, (int)m_p.m_y, (int)m_p.m_x               , (int)m_dr_r.m_downright.m_y, Colors::Blue);
		gfx.draw_line((int)m_p.m_x, (int)m_p.m_y, (int)m_ul_r.m_upleft.m_x   , (int)m_p.m_y, Colors::Yellow);
		gfx.draw_line((int)m_p.m_x, (int)m_p.m_y, (int)m_dr_r.m_downright.m_x, (int)m_p.m_y, Colors::Red);
	}

	if (m_ul != NULL) m_ul->draw(gfx, draw_rect);
	if (m_ur != NULL) m_ur->draw(gfx, draw_rect);
	if (m_dl != NULL) m_dl->draw(gfx, draw_rect);
	if (m_dr != NULL) m_dr->draw(gfx, draw_rect);
}

int node_f::size(int & sz) const
{
	sz++;
	if (m_ul != NULL) m_ul->size(sz);
	if (m_ur != NULL) m_ur->size(sz);
	if (m_dl != NULL) m_dl->size(sz);
	if (m_dr != NULL) m_dr->size(sz);
	return sz;
}
//-----------------------------------------------------------------------------
