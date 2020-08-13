#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "utils.h"

#define GMINY 0
#define GMAXY (Graphics::ScreenHeight - 1)
#define GMINX 0
#define GMAXX (Graphics::ScreenWidth - 1)

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,const Color & c );
	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth  = 1024;
	static constexpr int ScreenHeight = 1024;
	
	// safer but slower render functions
	void PutPixel_s(int x, int y, const Color & c);

	void draw_line_s(float2 s, float2 e, const Color& c);
	void draw_line_s(int x1, int y1, int x2, int y2, const Color & c);
	void draw_circle_s(int _x, int _y, int radius, const Color & c);

	void draw_circle_filled(int x, int y, int r, const Color & c);
	void draw_circle_filled_s(int x, int y, int r, const Color & c);

	// Bresenham's line algorithm
	void draw_line(int x1, int y1, int x2, int y2, const Color & c);

	//Midpoint circle algorithm
	void draw_circle(int _x, int _y, int radius, const Color & c);


	void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, const Color & col)
	{
		draw_line(x1, y1, x2, y2, col);
		draw_line(x2, y2, x3, y3, col);
		draw_line(x3, y3, x1, y1, col);
	}

	void draw_triangle_s(int x1, int y1, int x2, int y2, int x3, int y3, const Color& col)
	{
		draw_line_s(x1, y1, x2, y2, col);
		draw_line_s(x2, y2, x3, y3, col);
		draw_line_s(x3, y3, x1, y1, col);
	}

	// https://www.avrfreaks.net/sites/default/files/triangles.c
	void fill_triangle_s(int x1, int y1, int x2, int y2, int x3, int y3, const Color & col)
	{
		//auto SWAP = [](int& x, int& y) { int t = x; x = y; y = t; };
		auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) PutPixel_s(i, ny, col); };

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;
		bool changed1 = false;
		bool changed2 = false;
		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;
		// Sort vertices
		if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
		if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
		if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

		t1x = t2x = x1; y = y1;   // Starting points
		dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y2 - y1);

		dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
		else signx2 = 1;
		dy2 = (int)(y3 - y1);

		if (dy1 > dx1) {   // swap values
			std::swap(dx1, dy1);
			changed1 = true;
		}
		if (dy2 > dx2) {   // swap values
			std::swap(dy2, dx2);
			changed2 = true;
		}

		e2 = (int)(dx2 >> 1);
		// Flat top, just process the second half
		if (y1 == y2) goto next;
		e1 = (int)(dx1 >> 1);

		for (int i = 0; i < dx1;) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i < dx1) {
				i++;
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) t1xp = signx1;//t1x += signx1;
					else          goto next1;
				}
				if (changed1) break;
				else t1x += signx1;
			}
			// Move line
		next1:
			// process second line until y value is about to change
			while (1) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;//t2x += signx2;
					else          goto next2;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next2:
			if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);    // Draw line from min to max points found on the y
										 // Now increase y
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y == y2) break;

		}
	next:
		// Second half
		dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
		else signx1 = 1;
		dy1 = (int)(y3 - y2);
		t1x = x2;

		if (dy1 > dx1) {   // swap values
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else changed1 = false;

		e1 = (int)(dx1 >> 1);

		for (int i = 0; i <= dx1; i++) {
			t1xp = 0; t2xp = 0;
			if (t1x < t2x) { minx = t1x; maxx = t2x; }
			else { minx = t2x; maxx = t1x; }
			// process first line until y value is about to change
			while (i < dx1) {
				e1 += dy1;
				while (e1 >= dx1) {
					e1 -= dx1;
					if (changed1) { t1xp = signx1; break; }//t1x += signx1;
					else          goto next3;
				}
				if (changed1) break;
				else   	   	  t1x += signx1;
				if (i < dx1) i++;
			}
		next3:
			// process second line until y value is about to change
			while (t2x != x3) {
				e2 += dy2;
				while (e2 >= dx2) {
					e2 -= dx2;
					if (changed2) t2xp = signx2;
					else          goto next4;
				}
				if (changed2)     break;
				else              t2x += signx2;
			}
		next4:

			if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
			drawline(minx, maxx, y);
			if (!changed1) t1x += signx1;
			t1x += t1xp;
			if (!changed2) t2x += signx2;
			t2x += t2xp;
			y += 1;
			if (y > y3) return;
		}
	}

	void draw_elipse_s(float2 origin, int width, int height, const Color & col)
	{
		int hh = height * height;
		int ww = width * width;
		int hhww = hh * ww;
		int x0 = width;
		int dx = 0;

		// do the horizontal diameter
		for (int x = -width; x <= width; x++)
			PutPixel_s(origin.x + x, origin.y, col);

		// now do both halves at the same time, away from the diameter
		for (int y = 1; y <= height; y++)
		{
			int x1 = x0 - (dx - 1);  // try slopes of dx - 1 or more
			for (; x1 > 0; x1--)
				if (x1 * x1 * hh + y * y * ww <= hhww)
					break;
			dx = x0 - x1;  // current approximation of the slope
			x0 = x1;

			for (int x = -x0; x <= x0; x++)
			{
				PutPixel_s(origin.x + x, origin.y - y, col);
				PutPixel_s(origin.x + x, origin.y + y, col);
			}
		}
	}
	
	void draw_rect_filled(int x, int y, int w, int h, const Color & col)
	{
		for (int i = y; i < y + h; ++i)
		{
			std::fill_n(pSysBuffer + i * Graphics::ScreenWidth + x, w, col);
		}		
	}
	void draw_rect(int x, int y, int w, int h, const Color& col)
	{
		draw_line(x    , y    , x + w, y    , col);		// ---
		draw_line(x    , y    , x    , y + h, col);		// |
		draw_line(x + w, y    , x + w, y + h, col);     //   |
		draw_line(x    , y + h, x + w, y + h, col);     // ___ 
	}

	void draw_rect_s(int x, int y, int w, int h, const Color& col)
	{
		draw_line_s(x, y, x + w, y, col);				// ---
		draw_line_s(x, y, x, y + h, col);				// |
		draw_line_s(x + w, y, x + w, y + h, col);		//   |
		draw_line_s(x, y + h, x + w, y + h, col);		// ___ 
	}

};
