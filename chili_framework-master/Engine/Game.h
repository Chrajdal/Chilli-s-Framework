#pragma once
//#pragma optimize( "", off )
#include <iostream>
#include <iomanip>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <vector>
#include <deque>
#include <future>
#include <array>
#include <list>
#include <algorithm>
#include <functional>
#include <cassert>
#include <bitset>
#include <cstdlib>
#include <chrono>
#include <cassert>
#include <sstream>
#include <ctime>
#include <numeric>
#include <thread>
#include <mutex>
#include <fstream>
#include "MainWindow.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "utils.h"
#include "Random.h"
#include "Camera.h"
#include "geometry.h"

#include "quad_tree.h"

class Game
{
public:
	Game(class MainWindow& wnd);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	~Game(void);
	void Go();
private:
	void HandleInput();
	void ComposeFrame();
	void UpdateModel();

private:
	MainWindow & wnd;
	Graphics gfx;
	CTimer timer;
};