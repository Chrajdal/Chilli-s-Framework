#pragma once

#include "utils.h"
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include "test.h"
#include "node.h"
#include <vector>

using namespace std;

//-----------------------------------------------------------------------------
class quad_tree
{
public:
	quad_tree(void);
	~quad_tree(void);
	quad_tree::quad_tree(const quad_tree & src);
	quad_tree & quad_tree::operator = (const quad_tree & src);
	void insert(int x, int y);
	void insert(const Tpoint<int> & p);
	void clear(void);
	Tpoint<int> find_closest_point(const Tpoint<int> & p) const;
	vector<Tpoint<int>> find_n_closest_points(const Tpoint<int> & p, int n);
	void draw(Graphics & gfx, bool draw_rect = false) const;
public:
	node * m_root;
};
//-----------------------------------------------------------------------------
