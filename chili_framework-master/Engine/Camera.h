#pragma once

#include "Graphics.h"

class Camera
{
public:
	Camera(void)
	{
		m_x = 0;
		m_y = 0;
		m_width = Graphics::ScreenWidth;
		m_height = Graphics::ScreenHeight;
	}

public:
	double m_x; // current position of the camera
	double m_y; // current position of the camera
private:
	int m_width;  // viewport size = Graphics::ScreenWidth
	int m_height; // viewport size = Graphics::ScreenHeight
};
