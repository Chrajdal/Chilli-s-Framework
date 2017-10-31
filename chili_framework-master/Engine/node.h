#pragma once

#include "utils.h"
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include <vector>


Tpoint<int> get_closest_p(const Trect<int> & rect, const Tpoint<int> & p);

//-----------------------------------------------------------------------------
class node
{
public:
	node(const Tpoint<int> & p = {},
		const Trect<int> upleft_rect = {},
		const Trect<int> m_upright_rect = {},
		const Trect<int> m_downleft_rect = {},
		const Trect<int> m_downright_rect = {});
	~node(void);
	node::node(const node & src);
	node & node::operator = (const node & src);
	void insert(const Tpoint<int> & p, const node * parent);
	Tpoint<int> find_closest_point(const Tpoint<int> & p, Tpoint<int> & closest, unsigned long long & best_dist) const;
	vector<Tpoint<int>> find_n_closest_points(const Tpoint<int> & p, int n, vector<Tpoint<int>> & found) const;
	void draw(Graphics & gfx, bool draw_rect = false) const;
public:
	Tpoint<int> m_p;
	_Vec2<float> velocity;
	Trect<int> m_ul_r, m_ur_r, m_dl_r, m_dr_r;
	node * m_ul, *m_ur, *m_dl, *m_dr;
};
//-----------------------------------------------------------------------------
