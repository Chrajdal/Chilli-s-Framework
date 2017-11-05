#pragma once

#include "utils.h"
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include "node_f.h"
#include <vector>

//-----------------------------------------------------------------------------
class quad_tree_f
{
public:
	quad_tree_f(void);
	quad_tree_f(const quad_tree_f & src);
	quad_tree_f & operator = (const quad_tree_f & src);
	~quad_tree_f(void);

	void insert(float x, float y);
	void insert(const Tpoint<float> & p);
	void insert(const node_f & n);
	void clear(void);
	Tpoint<float> find_closest_point(const Tpoint<float> & p) const;
	void find_n_closest_points(const Tpoint<float> & p, int n, vector<node_f> & out);
	void draw(Graphics & gfx, bool draw_rect = false) const;

	/**/
	int size(void) const;
	/**/
public:
	node_f * m_root;
};
//-----------------------------------------------------------------------------
