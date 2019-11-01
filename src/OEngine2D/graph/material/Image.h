#ifndef __IMAGE_H__
#define __IMAGE_H__
#include "util.h"
#include "FreeImage/FreeImage.h"

class Image {
public:
	Image() {}
	~Image();

	bool Load(const std::string& path);
	bool Load(const void * data, const int32_t size);
	bool Convert(VkFormat format);

	bool ReadFromFreeImage(FIBITMAP * image);

	void * GetData() const { return _data; }

	VkFormat GetColor() const { return _format; }

	size_t GetWidth() const { return _width; }
	size_t GetHeight() const { return _height; }
	size_t GetSize() const { return _size; }

private:
	uint8_t * _data = nullptr;

	VkFormat _format = VK_FORMAT_UNDEFINED;
	size_t _width = 0;
	size_t _height = 0;
	size_t _size = 0;
};

#endif //__IMAGE_H__

