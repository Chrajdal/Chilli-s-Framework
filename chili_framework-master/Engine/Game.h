#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

struct Tpoint
{
	Tpoint(int _x = 0, int _y = 0)
		: m_x(_x), m_y(_y)
	{}

	Tpoint(const Tpoint & src)
		: m_x(src.m_x), m_y(src.m_y)
	{}

	Tpoint & operator = (const Tpoint & src)
	{
		if (this == &src)
			return *this;
		m_x = src.m_x;
		m_y = src.m_y;
		return *this;
	}

	//friend ostream & operator << (ostream & os, const Tpoint & src)
	//{
	//	return os << src.m_x << ", " << src.m_y;
	//}

	int m_x;
	int m_y;
};

struct Trect
{
	Trect(const Tpoint & upleft = {}, const Tpoint & downright = {})
		: m_upleft(upleft), m_downright(downright)
	{}

	Trect(const Trect & src)
		: m_upleft(src.m_upleft), m_downright(src.m_downright)
	{}

	Trect & operator = (const Trect & src)
	{
		if (this == &src)
			return *this;
		m_upleft = src.m_upleft;
		m_downright = src.m_downright;
		return *this;
	}

	//friend ostream & operator << (ostream & os, const Trect & src)
	//{
	//	return os << "(" << src.m_upleft << ") -> (" << src.m_downright << ")";
	//}

	Tpoint m_upleft;
	Tpoint m_downright;
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
};