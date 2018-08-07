#pragma once

#include "utils.h"
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include <vector>

class Node
{
public:
	Node(int x, int y, Bitmap * s, const Trect<double> & boundary = {});
	Node(const Node & src);
	Node & operator = (const Node & src);
	~Node(void);

	bool insert(const Node & n);
	void Draw(Graphics & gfx, int camx, int camy) const;
	void range(vector<const Node *> & PointsInRange, Trect<double> & range) const;

	int m_x;
	int m_y;
	Node * m_nw, *m_ne, *m_sw, *m_se;
	mutable Trect<double> m_boundary;
	Bitmap * m_tile_data;
};