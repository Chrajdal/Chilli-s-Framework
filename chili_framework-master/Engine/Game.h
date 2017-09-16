#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"


struct Tpoint
{
	Tpoint(int _x = 0, int _y = 0)
		: x(_x), y(_y) {}
	int x;
	int y;

	friend ostream & operator << (ostream & os, const Tpoint & src)
	{
		return os << src.x << ", " << src.y;
	}
};

double distance(const Tpoint & a, const Tpoint & b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

class node
{
public:
	node(int x = 0, int y = 0, int data = 0)
		: m_x(x),
		m_y(y),
		m_data(data),
		m_upleft(NULL),
		m_upright(NULL),
		m_downleft(NULL),
		m_downright(NULL)
	{}
	~node(void)
	{
		delete m_upleft; delete m_upright; delete m_downleft; delete m_downright;
	}

	void insert(int x, int y, int data)
	{
		if (m_x == x && m_y == y)
			return;

		if (m_x < x && m_y < y)
		{
			// going down right
			if (m_downright == NULL)
				m_downright = new node(x, y, data);
			else
				m_downright->insert(x, y, data);
		}
		else if (m_x < x && m_y >= y)
		{
			// going down left
			if (m_downleft == NULL)
				m_downleft = new node(x, y, data);
			else
				m_downleft->insert(x, y, data);
		}
		else if (m_x >= x && m_y < y)
		{
			// going up right
			if (m_upright == NULL)
				m_upright = new node(x, y, data);
			else
				m_upright->insert(x, y, data);
		}
		else// if(m_x > x && m_y > y)
		{
			// going up left
			if (m_upleft == NULL)
				m_upleft = new node(x, y, data);
			else
				m_upleft->insert(x, y, data);
		}
	}

	inline Tpoint point(void)const
	{
		return Tpoint(m_x, m_y);
	}

	Tpoint find_closest_point(const node * curr_best, double best_dist, const Tpoint & p) const
	{
		if (m_x == p.x && m_y == p.y)
			return p;

		double curr_dist = distance(this->point(), p);

		cout << this->point() << " / " << curr_best->point() << " --- " << curr_dist << " / " << best_dist << endl;

		if (curr_dist < best_dist)
		{
			curr_best = this;
			best_dist = curr_dist;
		}

		if (m_x < p.x && m_y < p.y)
		{ // GO DOWNRIGHT
			if (m_downright != NULL)
			{
				cout << "going DOWNRIGHT" << endl;
				return m_downright->find_closest_point(curr_best, best_dist, p);
			}
		}
		else if (m_x < p.x && m_y >= p.y)
		{ // GO UPRIGHT
			if (m_upright != NULL)
			{
				cout << "going UPRIGHT" << endl;
				return m_upright->find_closest_point(curr_best, best_dist, p);
			}
		}
		else if (m_x >= p.x && m_y < p.y)
		{ // GO DOWNLEFT
			if (m_downleft != NULL)
			{
				cout << "going DOWNLEFT" << endl;
				return m_downleft->find_closest_point(curr_best, best_dist, p);
			}
		}
		else if (m_x >= p.x && m_y >= p.y)
		{ // GO UPLEFT
			if (m_upleft != NULL)
			{
				cout << "going UPLEFT" << endl;
				return m_upleft->find_closest_point(curr_best, best_dist, p);
			}
		}

		return curr_best->point();
	}

	vector<Tpoint> find_n_closest_points(const Tpoint & p, int n) const
	{
		return{};
	}

	friend ostream & operator << (ostream & os, const node & src)
	{
		if (src.m_upleft != NULL)
			os << "\"" << src.m_x << "," << src.m_y << "\" -> " << *src.m_upleft;
		if (src.m_upright != NULL)
			os << "\"" << src.m_x << "," << src.m_y << "\" -> " << *src.m_upright;
		if (src.m_downleft != NULL)
			os << "\"" << src.m_x << "," << src.m_y << "\" -> " << *src.m_downleft;
		if (src.m_downright != NULL)
			os << "\"" << src.m_x << "," << src.m_y << "\" -> " << *src.m_downright;

		if (src.m_upleft == NULL &&
			src.m_upright == NULL &&
			src.m_downleft == NULL &&
			src.m_downright == NULL)
			os << "\"" << src.m_x << "," << src.m_y << "\"" << ";\n";
		return os;
	}

private:
	int m_x;
	int m_y;
	int m_data;
	node * m_upleft;
	node * m_upright;
	node * m_downleft;
	node * m_downright;
};

class map
{
public:
	map(void)
		: m_root(new node())
	{
	}

	~map(void)
	{
		delete m_root;
	}

	void insert(int x = 0, int y = 0, int data = 0)
	{
		if (m_root == NULL)
			m_root = new node(data);
		else
			m_root->insert(x, y, data);
	}

	Tpoint find_closest_point(const Tpoint & p) const
	{
		if (m_root == NULL)
			return Tpoint(0, 0);
		else
			return m_root->find_closest_point(m_root, distance(m_root->point(), p), p);
	}

	vector<Tpoint> find_n_closest_points(const Tpoint & p, int n) const
	{
		if (m_root == NULL)
			return vector<Tpoint>();
		else
			return m_root->find_n_closest_points(p, n);
	}


	friend ostream & operator << (ostream & os, const map & src)
	{
		if (src.m_root == NULL)
			return os;
		else
			return os << *src.m_root;
	}
private:
	node * m_root;
};

class CTimer
{
public:
	CTimer(void)
	{
		start = chrono::system_clock::now();
	}
	double elapsed(void) const
	{
		return (double)(chrono::duration_cast<chrono::milliseconds>
			(chrono::system_clock::now() - start)).count();
	}
private:
	chrono::time_point<chrono::system_clock> start;
};


class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	
private:
	MainWindow& wnd;
	Graphics gfx;
	map m;

};