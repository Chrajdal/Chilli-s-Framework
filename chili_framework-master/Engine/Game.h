#pragma once
#pragma optimize( "", off )
#include <iostream>
#include <iomanip>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <vector>
#include <deque>
#include <list>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdlib>
#include <chrono>
#include <cassert>
#include <sstream>
#include <ctime>
#include <numeric>
#include <thread>
#include <mutex>
#include <fstream>
#include <iostream>
#include "MainWindow.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Random.h"
#include "utils.h"

#include "node.h"
#include "node_f.h"

#include "quad_tree.h"
#include "quad_tree_f.h"

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
	void HandleInput();
	
private:
	MainWindow& wnd;
	Graphics gfx;
	CTimer m_timer;
};