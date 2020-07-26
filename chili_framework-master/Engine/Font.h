#pragma once

#include "Graphics.h"
#include "Bitmap.h"
#include <ctype.h>
#include <string>

class Font
{
public:
	Font(void)
		: font_map("fontfixed.bmp")
	{
		font_map.leave_only_this_color(Colors::Black);
	}
	
	void DrawChar(Graphics & gfx, char c, int x, int y, int font_size, const Color & color) const
	{
		// -----------------------------------------------------------------------------------------------
		// abcdefghijklmnopqrstuvwxyz
		// ABCEDFGHIJKLMNOPQRSTUVWXYZ
		// 1234567890.:, '"(!?)+-*/=<>
		// -----------------------------------------------------------------------------------------------
		// !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~
		// -----------------------------------------------------------------------------------------------
		int char_width = 127;
		int char_height = 272;
		int correction_offset = -10;
		font_map.DrawPart(gfx, x, y, int(c - '!') * char_width - correction_offset, 0, int(c - '!' + 1) * char_width - correction_offset, char_height, color);

		//int char_width = 106;
		//int char_height = 200;
		//if (isalnum(c))
		//{
		//	if (isalpha(c))
		//	{
		//		if (islower(c))
		//		{
		//			font_map.DrawPart(gfx, x, y, int(c - 'a') * char_width, 0, int(c - 'a' + 1) * char_width, 180, color);
		//			return;
		//		}
		//		if (isupper(c))
		//		{
		//			font_map.DrawPart(gfx, x, y, int(c - 'A') * char_width, 240, int(c - 'A' + 1) * char_width, 420, color);
		//			return;
		//		}
		//	}			
		//	
		//	if (isdigit(c))
		//	{
		//		if(c == 0) font_map.DrawPart(gfx, x, y,                9 * char_width, 450,               10 * char_width, 630, color);
		//		else       font_map.DrawPart(gfx, x, y, int(c - '1' + 1) * char_width, 450, int(c - '1' + 2) * char_width, 630, color);
		//		return;
		//	}
		//}
		//else
		//{
		//
		//}		
	}

	void DrawString(Graphics & gfx, const std::string & str, int x, int y, int font_size, const Color & color) const
	{

	}

private:
	Bitmap font_map;
};