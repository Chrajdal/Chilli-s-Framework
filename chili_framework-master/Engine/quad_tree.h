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
	const Node * at(int x, int y) const;

	Node * access(int x, int y);

	void clear(void);
	unsigned size(void) const;
	Trect<double> boundary(void) const;

private:
	Node * m_root;
	unsigned m_size;
};