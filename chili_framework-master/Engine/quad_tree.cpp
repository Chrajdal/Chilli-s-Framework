#include "quad_tree.h"

//-----------------------------------------------------------------------------
int random_int(int min, int max)
{
	int range = max - min + 1;
	return (rand() % range) + min;
}
float random_between(const float & min, const float & max)
{
	return (max - min) * ((float)rand() / (float)RAND_MAX) + min;
}
double random_between(const double & min, const double & max)
{
	return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

long sq_distance(const Tpoint<int> & a, const Tpoint<int> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
long long sq_distance(const Tpoint <long> & a, const Tpoint<long> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
double sq_distance(const Tpoint<float> & a, const Tpoint<float> & b)
{
	return (double)((a.m_x - b.m_x) * (double)(a.m_x - b.m_x))
		+ (double)((a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
double sq_distance(const Tpoint<double> & a, const Tpoint<double> & b)
{
	return (double)((a.m_x - b.m_x) * (double)(a.m_x - b.m_x))
		+ (double)((a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}

template<typename T>
Tpoint<T> get_closest_p(const Trect<T> & rect, const Tpoint<T> & p)
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
			return Tpoint<T>(rect.m_upleft);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 7
			return Tpoint<T>(rect.m_upleft.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 4
			return Tpoint<T>(rect.m_upleft.m_x, p.m_y);
		}
	}
	else if (p.m_x > rect.m_downright.m_x)
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 3
			return Tpoint<T>(rect.m_downright.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 9
			return Tpoint<T>(rect.m_downright);
		}
		else
		{	// no.: 6
			return Tpoint<T>(rect.m_downright.m_x, p.m_y);
		}
	}
	else
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 2
			return Tpoint<T>(p.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 8
			return Tpoint<T>(p.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 5
			return p;
		}
	}
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
node::node(const Tpoint<int> & p,
	const Trect<int> upleft_rect,
	const Trect<int> upright_rect,
	const Trect<int> downleft_rect,
	const Trect<int> downright_rect)
	: m_p(p),
	m_ul_r(upleft_rect),
	m_ur_r(upright_rect),
	m_dl_r(downleft_rect),
	m_dr_r(downright_rect),
	m_ul(NULL), m_ur(NULL), m_dl(NULL), m_dr(NULL)
{}

node::~node(void)
{
	delete m_ul;
	delete m_ur;
	delete m_dl;
	delete m_dr;
}

node::node(const node & src)
	: m_p(src.m_p),
	m_ul_r(src.m_ul_r),
	m_ur_r(src.m_ur_r),
	m_dl_r(src.m_dl_r),
	m_dr_r(src.m_dr_r),
	m_ul(NULL), m_ur(NULL), m_dl(NULL), m_dr(NULL)
{
	if (src.m_ul != NULL) m_ul = new node(*src.m_ul);
	if (src.m_ur != NULL) m_ur = new node(*src.m_ur);
	if (src.m_dl != NULL) m_dl = new node(*src.m_dl);
	if (src.m_dr != NULL) m_dr = new node(*src.m_dr);
}

node & node::operator = (const node & src)
{
	if (this == &src)
		return *this;

	m_p = src.m_p;
	m_ul_r = src.m_ul_r;
	m_ur_r = src.m_ur_r;
	m_dl_r = src.m_dl_r;
	m_dr_r = src.m_dr_r;

	delete m_ul; m_ul = NULL;
	delete m_ur; m_ur = NULL;
	delete m_dl; m_dl = NULL;
	delete m_dr; m_dr = NULL;

	if (src.m_ul != NULL) m_ul = new node(*src.m_ul);
	if (src.m_ur != NULL) m_ur = new node(*src.m_ur);
	if (src.m_dl != NULL) m_dl = new node(*src.m_dl);
	if (src.m_dr != NULL) m_dr = new node(*src.m_dr);

	return *this;
}

void node::insert(const Tpoint<int> & p, const node * parent)
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

				m_ul->m_ul_r = Trect<int>(Tpoint<int>(MIN_X, MIN_Y), Tpoint<int>(MID_X, MID_Y));
				m_ul->m_ur_r = Trect<int>(Tpoint<int>(MID_X, MIN_Y), Tpoint<int>(MAX_X, MID_Y));
				m_ul->m_dl_r = Trect<int>(Tpoint<int>(MIN_X, MID_Y), Tpoint<int>(MID_X, MAX_Y));
				m_ul->m_dr_r = Trect<int>(Tpoint<int>(MID_X, MID_Y), Tpoint<int>(MAX_X, MAX_Y));
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

				m_ur->m_ul_r = Trect<int>(Tpoint<int>(MIN_X, MIN_Y), Tpoint<int>(MID_X, MID_Y));
				m_ur->m_ur_r = Trect<int>(Tpoint<int>(MID_X, MIN_Y), Tpoint<int>(MAX_X, MID_Y));
				m_ur->m_dl_r = Trect<int>(Tpoint<int>(MIN_X, MID_Y), Tpoint<int>(MID_X, MAX_Y));
				m_ur->m_dr_r = Trect<int>(Tpoint<int>(MID_X, MID_Y), Tpoint<int>(MAX_X, MAX_Y));
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

				m_dl->m_ul_r = Trect<int>(Tpoint<int>(MIN_X, MIN_Y), Tpoint<int>(MID_X, MID_Y));
				m_dl->m_ur_r = Trect<int>(Tpoint<int>(MID_X, MIN_Y), Tpoint<int>(MAX_X, MID_Y));
				m_dl->m_dl_r = Trect<int>(Tpoint<int>(MIN_X, MID_Y), Tpoint<int>(MID_X, MAX_Y));
				m_dl->m_dr_r = Trect<int>(Tpoint<int>(MID_X, MID_Y), Tpoint<int>(MAX_X, MAX_Y));
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

				m_dr->m_ul_r = Trect<int>(Tpoint<int>(MIN_X, MIN_Y), Tpoint<int>(MID_X, MID_Y));
				m_dr->m_ur_r = Trect<int>(Tpoint<int>(MID_X, MIN_Y), Tpoint<int>(MAX_X, MID_Y));
				m_dr->m_dl_r = Trect<int>(Tpoint<int>(MIN_X, MID_Y), Tpoint<int>(MID_X, MAX_Y));
				m_dr->m_dr_r = Trect<int>(Tpoint<int>(MID_X, MID_Y), Tpoint<int>(MAX_X, MAX_Y));
			}
			else
			{
				m_dr->insert(p, m_dr);
			}
		}
	}
}

Tpoint<int> node::find_closest_point(const Tpoint<int> & p, Tpoint<int> & closest, unsigned long long & best_dist) const
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

vector<Tpoint<int>> node::find_n_closest_points(const Tpoint<int> & p, int n, vector<Tpoint<int>> & found) const
{
	if (found.size() < n)
	{
		if (found.empty())
			found.push_back(m_p);
		else
		{
			auto it = lower_bound(found.begin(), found.end(), m_p, [p](const Tpoint<int> & a, const Tpoint<int> & b) { return sq_distance(a, p) <= sq_distance(b, p); });
			found.insert(it, m_p);
		}
	}
	else
	{
		unsigned long long curr_dist = sq_distance(p, m_p);
		for (auto it = found.begin(); it != found.end(); ++it)
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
		if (sq_distance(get_closest_p(m_ul_r, p), p) <= sq_distance(p, found.back()))
			m_ul->find_n_closest_points(p, n, found);
	if (m_ur != NULL)
		if (sq_distance(get_closest_p(m_ur_r, p), p) <= sq_distance(p, found.back()))
			m_ur->find_n_closest_points(p, n, found);
	if (m_dl != NULL)
		if (sq_distance(get_closest_p(m_dl_r, p), p) <= sq_distance(p, found.back()))
			m_dl->find_n_closest_points(p, n, found);
	if (m_dr != NULL)
		if (sq_distance(get_closest_p(m_dr_r, p), p) <= sq_distance(p, found.back()))
			m_dr->find_n_closest_points(p, n, found);
	return found;
}
void node::draw(Graphics & gfx, bool draw_rect) const
{
	gfx.PutPixel(m_p.m_x, m_p.m_y, Colors::Gray);
	if (draw_rect == true)
	{
		gfx.draw_line(m_p.m_x, m_p.m_y, m_p.m_x, m_ul_r.m_upleft.m_y, Colors::Green);
		gfx.draw_line(m_p.m_x, m_p.m_y, m_p.m_x, m_dr_r.m_downright.m_y, Colors::Blue);
		gfx.draw_line(m_p.m_x, m_p.m_y, m_ul_r.m_upleft.m_x, m_p.m_y, Colors::Yellow);
		gfx.draw_line(m_p.m_x, m_p.m_y, m_dr_r.m_downright.m_x, m_p.m_y, Colors::Red);
	}

	if (m_ul != NULL) m_ul->draw(gfx, draw_rect);
	if (m_ur != NULL) m_ur->draw(gfx, draw_rect);
	if (m_dl != NULL) m_dl->draw(gfx, draw_rect);
	if (m_dr != NULL) m_dr->draw(gfx, draw_rect);
}
//-----------------------------------------------------------------------------

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
	m_velocity(src.m_velocity)
{
	m_ul = m_ur = m_dl = m_dr = NULL;
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

				m_ul->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
				m_ul->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
				m_ul->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
				m_ul->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
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

				m_ur->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
				m_ur->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
				m_ur->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
				m_ur->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
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

				m_dl->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
				m_dl->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
				m_dl->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
				m_dl->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
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

				m_dr->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
				m_dr->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
				m_dr->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
				m_dr->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
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

				m_ul->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
				m_ul->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
				m_ul->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
				m_ul->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
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

				m_ur->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
				m_ur->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
				m_ur->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
				m_ur->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
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

				m_dl->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
				m_dl->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
				m_dl->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
				m_dl->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
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

				m_dr->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
				m_dr->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
				m_dr->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
				m_dr->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
			}
			else
			{
				m_dr->insert(n, m_dr);
			}
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

vector<node_f> node_f::find_n_closest_points(const Tpoint<float> & p, int n, vector<node_f> & found) const
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
	return found;
}
//-----------------------------------------------------------------------------

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
	if (p.m_x < GMINX || p.m_x >= GMAXX || p.m_y < GMINY || p.m_y >= GMAXY)
		return;

	if (m_root == NULL)
	{
		m_root = new node_f(p);
		float MIN_X = GMINX;
		float MID_X = p.m_x;
		float MAX_X = GMAXX;
		float MIN_Y = GMINY;
		float MID_Y = p.m_y;
		float MAX_Y = GMAXY;

		m_root->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
		m_root->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
		m_root->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
		m_root->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
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

		m_root->m_ul_r = Trect<float>(Tpoint<float>(MIN_X, MIN_Y), Tpoint<float>(MID_X, MID_Y));
		m_root->m_ur_r = Trect<float>(Tpoint<float>(MID_X, MIN_Y), Tpoint<float>(MAX_X, MID_Y));
		m_root->m_dl_r = Trect<float>(Tpoint<float>(MIN_X, MID_Y), Tpoint<float>(MID_X, MAX_Y));
		m_root->m_dr_r = Trect<float>(Tpoint<float>(MID_X, MID_Y), Tpoint<float>(MAX_X, MAX_Y));
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

vector<node_f> quad_tree_f::find_n_closest_points(const Tpoint<float> & p, int n)
{
	if (m_root == NULL)
		return vector<node_f>();
	else
	{
		vector<node_f> res;
		return m_root->find_n_closest_points(p, n, res);
	}
}
//-----------------------------------------------------------------------------
