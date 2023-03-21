#pragma once

#include "Image.h"

class ImageManager
{
public:
	static void			renderRectBy(const HDC& hdc, const RECT& rect) { Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom); }

	static void			setPrint(Image& image, POINT printSize) { image.setPrintSize(move(printSize)); }

	static const Image& makeImage(POINT sp, FPOINT scale, POINT size);
	static const Image& makeImage(const char* fileName, POINT sp, FPOINT scale, POINT size, bool isTrans = false, REVERSE_DATA reverseData = {false, false}, COLORREF transColor = 0);
	static const Image& makeImage(const char* fileName, POINT sp, FPOINT scale, POINT size, POINT keyFrame, bool isTrans = false, REVERSE_DATA reverseData = { false, false }, COLORREF transColor = 0);
	static void			resetImage(Image& image, POINT startPosition, POINT size, POINT printSize) { image.resetSize(move(startPosition), move(size), move(printSize)); }
	static void			resetImage(Image& image, POINT startPosition, FPOINT scale) { image.resetSpAndScaleOfImage(move(startPosition), move(scale)); }
	static void			resetImage(Image& image, POINT startPosition, POINT printSize) { image.resetSpAndPrintSizeOfImage(move(startPosition), move(printSize)); }	
	static void			findImageToDelete(uint16 imageIdentityNumber);
	static void			allDelete();

private:
	static const Image& processToHoldImage(unique_ptr<Image> image);

private:
	static map<uint16, unique_ptr<Image>> ImagesMap;
};
