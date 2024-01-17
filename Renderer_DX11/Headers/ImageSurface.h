#pragma once

#include "DX11_ErrorMacro.h"
#include "../Dependencies/PNGLoader/lodepng.h"

#include <vector>
#include <string>

/// <summary>
/// Used as a conteiner for storing images.
/// </summary>
class ImageSurface {
public:
	struct Color {
		unsigned int color;
		Color() noexcept : color(0) {}
		Color(const Color& color) noexcept : color(color.color) {}
		Color(unsigned int color) noexcept : color(color) {}
		typedef unsigned char UCHAR;
		Color(UCHAR r, UCHAR g, UCHAR b, UCHAR a)noexcept : color(((r << 24u) | (g << 16u) | (b << 8u) | a)) {}
		UCHAR GetR() const { return color >> 24u; }
		UCHAR GetG() const { return color >> 16u; }
		UCHAR GetB() const { return color >> 8u; }
		UCHAR GetA() const { return color; }
		void SetR(const UCHAR r) noexcept { (color &= 0x00FFFFFF) |= (r << 24u); }
		void SetG(const UCHAR g) noexcept { (color &= 0xFF00FFFF) |= (g << 16u); }
		void SetB(const UCHAR b) noexcept { (color &= 0xFFFF00FF) |= (b << 8u); }
		void SetA(const UCHAR a) noexcept { (color &= 0xFFFFFF00) |= a; }
	};

	ImageSurface(unsigned int width, unsigned int height, std::vector<Color>& bufer) noexcept;
	ImageSurface(unsigned int width, unsigned int height) noexcept;
	ImageSurface(const ImageSurface& surface) = delete;
	~ImageSurface() noexcept;

	void ClearSurface(const Color& color) noexcept;

	Color& GetPixel(unsigned int x, unsigned int y) const noexcept;
	void SetPixel(unsigned int x, unsigned int y, const Color color) noexcept;

	char* GetBuffer() const noexcept;
	unsigned int GetWidth() const noexcept;
	unsigned int GetHeight() const noexcept;

	static ImageSurface FromFile(const std::string& name) noexcept;
	void Copy(const ImageSurface& src) noexcept;

private:
	std::vector<Color> m_buffer;
	unsigned int m_width;
	unsigned int m_height;
};