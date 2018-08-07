#pragma once

#include "utils.h"
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include "node.h"
#include <vector>

class QuadTree
{
public:
	QuadTree(void);
	~QuadTree(void);

	void insert(const Node & n);
	vector<const Node *> range(Trect<double> & range) const;
	void clear(void);
	void Draw(Graphics & gfx, int camx, int camy) const;
	unsigned size(void) const;
	Trect<double> boundary(void) const;

private:
	Node * m_root;
	unsigned m_size;
};