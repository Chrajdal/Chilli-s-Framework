#pragma once

#include "utils.h"
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include <vector>

Tpoint<float> get_closest_p(const Trect<float> & rect, const Tpoint<float> & p);

//-----------------------------------------------------------------------------
class node_f
{
public:
	node_f(const Tpoint<float> & p = {},
		const Trect<float> upleft_rect = {},
		const Trect<float> m_upright_rect = {},
		const Trect<float> m_downleft_rect = {},
		const Trect<float> m_downright_rect = {});
	node_f(const node_f & src);
	node_f & operator=(const node_f & src);
	~node_f(void);
	void insert(const Tpoint<float> & p, const node_f * parent);
	void insert(const node_f & p, const node_f * parent);
	Tpoint<float> find_closest_point(const Tpoint<float> & p, Tpoint<float> & closest, double & best_dist) const;
	vector<node_f> find_n_closest_points(const Tpoint<float> & p, int n, vector<node_f> & found) const;
	void draw(Graphics & gfx, bool draw_rect = false) const;
public:
	Tpoint<float> m_p;
	_Vec2<float> m_velocity;
	Trect<float> m_ul_r, m_ur_r, m_dl_r, m_dr_r;
	node_f * m_ul, *m_ur, *m_dl, *m_dr;
};
//-----------------------------------------------------------------------------
