#pragma once

#include "utils.h"

class test_node
{
public:
	test_node(const Trect<float> & bound, const Tpoint<float> & p)
		: boundary(bound), point(p),
		northwest(NULL),
		northeast(NULL),
		southwest(NULL),
		southeast(NULL)
	{}

	~test_node(void)
	{
		delete northwest;
		delete northeast;
		delete southwest;
		delete southeast;
	}

	bool insert(const Tpoint<float> & p)
	{
		if (!boundary.contains(p))
			return false;
		if (point.m_x == p.m_x && point.m_y == p.m_y)
			return false;

		Tpoint<float> middle((boundary.m_upleft.m_x + boundary.m_downright.m_x) / 2, (boundary.m_upleft.m_y + boundary.m_downright.m_y) / 2);
		// inserting to WEST
		if (p.m_x < middle.m_x)
		{
			// inserting to NORTH
			if (p.m_y < middle.m_y)
			{
				if (northwest == NULL)
				{
					northwest = new test_node(Trect<float>(boundary.m_upleft, middle), p);
					return true;
				}
				else
					return northwest->insert(p);
			}
			// inserting to SOUTH
			else
			{
				if (southwest == NULL)
				{
					southwest = new test_node(Trect<float>({ boundary.m_upleft.m_x, middle.m_y }, { middle.m_x, boundary.m_downright.m_y }), p);
					return true;
				}
				else
					return southwest->insert(p);
			}
		}
		// inserting to EAST
		else
		{
			// inserting to NORTH
			if (p.m_y < middle.m_y)
			{
				if (northeast == NULL)
				{
					northeast = new test_node(Trect<float>({ middle.m_x, boundary.m_upleft.m_y }, { boundary.m_downright.m_x, middle.m_y }), p);
					return true;
				}
				else
					return northeast->insert(p);
			}
			// inserting to SOUTH
			else
			{
				if (southeast == NULL)
				{
					southeast = new test_node(Trect<float>(middle, boundary.m_downright), p);
					return true;
				}
				else
					return southeast->insert(p);
			}
		}
	}

	void draw(Graphics & gfx)
	{
		gfx.PutPixel(point.m_x, point.m_y, Colors::White);
		
		gfx.draw_line(boundary.m_upleft.m_x,    boundary.m_upleft.m_y,		boundary.m_upleft.m_x,    boundary.m_downright.m_y, Colors::Gray);
		gfx.draw_line(boundary.m_downright.m_x, boundary.m_upleft.m_y,		boundary.m_downright.m_x, boundary.m_downright.m_y, Colors::Gray);
		gfx.draw_line(boundary.m_upleft.m_x,	boundary.m_upleft.m_y,		boundary.m_downright.m_x, boundary.m_upleft.m_y,	Colors::Gray);
		gfx.draw_line(boundary.m_upleft.m_x,	boundary.m_downright.m_y,	boundary.m_downright.m_x, boundary.m_downright.m_y, Colors::Gray);
		
		if (northwest != NULL) northwest->draw(gfx);
		if (northeast != NULL) northeast->draw(gfx);
		if (southwest != NULL) southwest->draw(gfx);
		if (southeast != NULL) southeast->draw(gfx);
	}

private:
	Tpoint<float> point;
	Trect<float> boundary;

	test_node * northwest;
	test_node * northeast;
	test_node * southwest;
	test_node * southeast;
};




//		|
//   NW | NE
// -----+-----
//   SW | SE
//		|

class test_tree
{
public:
	test_tree(void)
		: root(NULL), m_size(0)
	{}

	~test_tree(void)
	{
		delete root;
	}
	void insert(const Tpoint<float> & p)
	{
		if (root == NULL)
		{
			Trect<float> screen({ 0,0, }, { Graphics::ScreenWidth - 1, Graphics::ScreenHeight - 1 });
			root = new test_node(screen, p);
			m_size = 1;
		}
		else
		{
			if (root->insert(p) == true)
				m_size++;
		}
	}

	void clear(void)
	{
		delete root;
		root = NULL;
		m_size = 0;
	}

	unsigned size(void) const
	{
		return m_size;
	}

	void draw(Graphics & gfx) const
	{
		if (root != NULL) root->draw(gfx);
	}
private:
	test_node * root;
	unsigned m_size;
};

