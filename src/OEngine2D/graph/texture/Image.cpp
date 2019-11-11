#include "Image.h"

Image::~Image() {
	if (_data) {
		free(_data);
		_data = nullptr;
	}
}

bool Image::Load(const std::string& path) {
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path.c_str());
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(path.c_str());

	CHECK(fif != FIF_UNKNOWN, "this is not a image :" << path);
	CHECK(FreeImage_FIFSupportsReading(fif), "not support this format :" << path);

	FIBITMAP* image = FreeImage_Load(fif, path.c_str());
	CHECK(image != nullptr, "load image failed :" << path);

	if (!ReadFromFreeImage(image)) {
		FreeImage_Unload(image);
		return false;
	}

	FreeImage_Unload(image);
	return true;
}

bool Image::Load(const void * data, const int32_t size) {
	FIMEMORY * mem = FreeImage_OpenMemory((uint8_t*)data, size);
	if (!mem)
		return false;

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(mem);
	if (fif == FIF_UNKNOWN) {
		OASSERT(false, "this is not a image");
		FreeImage_CloseMemory(mem);
		return false;
	}

	if (!FreeImage_FIFSupportsReading(fif)) {
		OASSERT(false, "not support this format");
		FreeImage_CloseMemory(mem);
		return false;
	}

	FIBITMAP * image = FreeImage_LoadFromMemory(fif, mem);
	if (!image) {
		OASSERT(false, "load image failed");
		FreeImage_CloseMemory(mem);
		return false;
	}

	if (!ReadFromFreeImage(image)) {
		FreeImage_Unload(image);
		FreeImage_CloseMemory(mem);
		return false;
	}

	FreeImage_Unload(image);
	FreeImage_CloseMemory(mem);
	return true;
}

bool Image::Convert(VkFormat format) {
	return true;
}

bool Image::ReadFromFreeImage(FIBITMAP * image) {
	int32_t i, j, k;
	int32_t pitch = FreeImage_GetPitch(image);
	uint8_t* bits = FreeImage_GetBits(image);
	int32_t bpp = FreeImage_GetBPP(image);
	RGBQUAD *palette = NULL;

	_width = FreeImage_GetWidth(image);
	_height = FreeImage_GetHeight(image);

	switch (bpp) {
	case 8:
		if (!(palette = FreeImage_GetPalette(image))) 
			return NULL;
		_data = (uint8_t*)malloc(_width * _height * 3);
		_format = VK_FORMAT_R8G8B8_UNORM;
		_size = 3 * _width * _height;
		for (i = 0; i < _height; ++i) {
			for (j = 0; j < _width; ++j) {
				k = bits[i * pitch + j];
				_data[(i * _width + j) * 3 + 0] = palette[k].rgbRed;
				_data[(i * _width + j) * 3 + 1] = palette[k].rgbGreen;
				_data[(i * _width + j) * 3 + 2] = palette[k].rgbBlue;
			}
		}
		break;
	case 24:
		_data = (uint8_t*)malloc(_width * _height * 3);
		_format = VK_FORMAT_R8G8B8_UNORM;
		_size = 3 * _width * _height;
		for (i = 0; i < _height; ++i) {
			for (j = 0; j < _width; ++j) {
				_data[(i * _width + j) * 3 + 0] = bits[i*pitch + j * 3 + 2];
				_data[(i * _width + j) * 3 + 1] = bits[i*pitch + j * 3 + 1];
				_data[(i * _width + j) * 3 + 2] = bits[i*pitch + j * 3 + 0];
			}
		}
		break;
	case 32:
		_data = (uint8_t*)malloc(_width * _height * 4);
		_format = VK_FORMAT_R8G8B8A8_UNORM;
		_size = 4 * _width * _height;
		for (i = 0; i < _height; ++i) {
			for (j = 0; j < _width; ++j) {
				_data[(i * _width + j) * 4 + 0] = bits[i*pitch + j * 3 + 2];
				_data[(i * _width + j) * 4 + 1] = bits[i*pitch + j * 3 + 1];
				_data[(i * _width + j) * 4 + 2] = bits[i*pitch + j * 3 + 0];
				_data[(i * _width + j) * 4 + 3] = bits[i*pitch + j * 3 + 3];
			}
		}
		break;
	default: return nullptr;
	}
	return true;
}
