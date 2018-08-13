#pragma once

#include "utils.h"
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include <vector>
#include "Surface.h"
#include "tile.h"

class Node
{
public:
	Node(int x, int y, const tile_type & ttile, const Trect<double> & boundary = {});
	Node(const Node & src);
	Node & operator = (const Node & src);
	~Node(void);

	bool insert(const Node & n);
	void range(vector<const Node *> & PointsInRange, Trect<double> & range) const;
	const Node * at(int x, int y, const Node * n) const;
	Node * access(int x, int y);

	friend ostream & operator << (ostream & os, const Node & src)
	{
		if (src.m_nw != NULL) os << *src.m_nw;
		if (src.m_ne != NULL) os << *src.m_ne;
		if (src.m_sw != NULL) os << *src.m_sw;
		if (src.m_se != NULL) os << *src.m_se;

		return os << src.m_x << " " << src.m_y << " " << src.m_tile << " # ";
	}

	int m_x;
	int m_y;
	Node * m_nw, *m_ne, *m_sw, *m_se;
	mutable Trect<double> m_boundary;
	tile_type m_tile;
};