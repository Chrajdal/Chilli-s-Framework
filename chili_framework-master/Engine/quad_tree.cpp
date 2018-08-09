#include "quad_tree.h"

QuadTree::QuadTree(void)
	: m_root(NULL), m_size(0)
{}

QuadTree::~QuadTree(void)
{
	delete m_root;
}

void QuadTree::insert(const Node & n)
{
	if (m_root == NULL)
	{
		Trect<double> boundary{ { -100000, -100000 },{ 100000, 100000 } };
		if (boundary.contains(n.m_x, n.m_y))
		{
			m_root = new Node(n.m_x, n.m_y, n.m_tile, boundary);
			m_size = 1;
		}
	}
	else
	{
		if (m_root->insert(n))
			m_size++;
	}
}

vector<const Node *> QuadTree::range(Trect<double> & range) const
{
	vector<const Node *> res;
	if (m_root != NULL)
		m_root->range(res, range);
	return res;
}

const Node * QuadTree::at(int x, int y) const
{
	if (m_root == NULL)
		return NULL;
	else
		return m_root->at(x, y);
}

void QuadTree::clear(void)
{
	delete m_root;
	m_root = NULL;
	m_size = 0;
}

unsigned QuadTree::size(void) const
{
	return m_size;
}

Trect<double> QuadTree::boundary(void) const
{
	if (m_root == NULL)
		return Trect<double>();
	return m_root->m_boundary;
}
