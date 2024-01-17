#include "../../Headers/ImageSurface.h"


ImageSurface::ImageSurface(unsigned int width, unsigned int height, std::vector<Color>& bufer) noexcept
	: m_buffer(bufer), m_width(width), m_height(height) {}

ImageSurface::ImageSurface(unsigned int width, unsigned int height) noexcept 
	: m_buffer(std::vector<Color>(width* height)), m_width(width), m_height(height) {}

ImageSurface::~ImageSurface() noexcept {}


void ImageSurface::ClearSurface(const Color& color) noexcept {
	memset(m_buffer.data(), color.color, m_width * m_height * sizeof(Color));
}

ImageSurface::Color& ImageSurface::GetPixel(unsigned int x, unsigned int y) const noexcept {
	Color ret = Color(m_buffer[0]);

	if (x >= 0 || x < m_width && y >= 0 && y < m_height) 
	{
		ret = Color(m_buffer[y * m_width + x]);
		return ret;
	}
	__ERROR(L"Uncorrected coordinates", __FILE__);
	return ret;
}

void ImageSurface::SetPixel(unsigned int x, unsigned int y, const Color color) noexcept {
	if (x >= 0 || x < m_width && y >= 0 && y < m_height) m_buffer[y * m_width + x] = color;
	else {
		__ERROR(L"Uncorrected coordinates", __FILE__);
	}
}


char* ImageSurface::GetBuffer() const noexcept {
	return (char*)m_buffer.data();
}
unsigned int ImageSurface::GetWidth() const noexcept {
	return m_width;
}
unsigned int ImageSurface::GetHeight() const noexcept {
	return m_height;
}


ImageSurface ImageSurface::FromFile(const std::string& name) noexcept {
	unsigned int width = 0;
	unsigned int height = 0;

	std::vector<unsigned char> image; //the raw pixels
	lodepng::decode(image, width, height, name);

	std::vector<Color> buffer(width * height);
	for (int i = 0; i < width * height; i++) {
		buffer[width * height - 1 - i].SetR(image[i * 4 + 3]);
		buffer[width * height - 1 - i].SetG(image[i * 4 + 2]);
		buffer[width * height - 1 - i].SetB(image[i * 4 + 1]);
		buffer[width * height - 1 - i].SetA(image[i * 4]);
	}

	return ImageSurface(width, height, buffer);
}

void ImageSurface::Copy(const ImageSurface& src) noexcept {
	m_width = src.m_width;
	m_height = src.m_height;
	m_buffer = std::vector<Color>(m_width * m_height);
	memcpy(m_buffer.data(), src.m_buffer.data(), m_width * m_height * sizeof(Color));
}