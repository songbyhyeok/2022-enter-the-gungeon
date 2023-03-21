#pragma once

#include "Image.h"

class CameraController
{
public:
	void			init();
	void			update();
	void			render() const;
	void			release();

	void			resetResolutionOfFrontBuffer(POINT startPosition, POINT printSize);

private:
	void			drawFrontBufferBy(const HDC& hdc) const;
	void			drawBackBuffer() const;

private:
	Image								_frontBufferImg;
	HDC									_backBufferHDC;
};

