#include "Image.h"
#include "ScreenManager.h"
#include "TrigonometricController.h"

#pragma comment(lib, "msimg32.lib")

void Image::init(POINT sp, FPOINT scale, POINT size)
{
	initDatas(move(sp), move(scale), size);

	processHDCAndDCs(nullptr, move(size));
}

void Image::init(const char* fileName, POINT sp, FPOINT scale, POINT size, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	_imageInfo.source = move(fileName);
	initDatas(move(scale), move(size), move(sp), isTrans, move(reverseData), transColor);

	processHDCAndDCs(move(fileName), move(size));
}

void Image::init(const char* fileName, POINT sp, FPOINT scale, POINT size, POINT keyFrame, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	_imageInfo.source = move(fileName);
	initDatas(move(scale), move(size), move(sp), move(keyFrame), isTrans, move(reverseData), transColor);
	processHDCAndDCs(move(fileName), move(size));
}

void Image::render(const HDC& hdc, const POINT& dest) const
{
	switch (_isTrans)
	{
	case true:
		/*switch (_imageInfo.size.x >= _imageInfo.printSize.x)
		{
		case true:
			GdiTransparentBlt ( hdc,
				dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hMemDC,
				0, 0,
				_imageInfo.size.x, _imageInfo.size.y,
				_transColor );
			break;
		case false:
			StretchBlt (_imageInfo.hBlendDC,
				0, 0,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hMemDC,
				0, 0,
				_imageInfo.size.x, _imageInfo.size.y,
				SRCCOPY );
			GdiTransparentBlt ( hdc,
				dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hBlendDC,
				0, 0,
				_imageInfo.printSize.x, _imageInfo.printSize.y,
				_transColor );
			break;
		}*/
		switch (_reverseData.x || _reverseData.y)
		{
		case false:
			GdiTransparentBlt(hdc,
				dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hMemDC,
				0, 0,
				_imageInfo.size.x, _imageInfo.size.y,
				_transColor);
			break;
		case true:
			POINT blendDest{ 0,0 };
			POINT blendSize = _imageInfo.size;
			if (_reverseData.x)
			{
				blendSize.x = -_imageInfo.size.x - 1;
				blendDest.x = _imageInfo.size.x;
			}
			if (_reverseData.y)
			{
				blendSize.y = -_imageInfo.size.y - 1;
				blendDest.y = _imageInfo.size.y;
			}

			StretchBlt(_imageInfo.hBlendDC,
				blendDest.x, blendDest.y,
				blendSize.x, blendSize.y,

				_imageInfo.hMemDC,
				0, 0,
				_imageInfo.size.x, _imageInfo.size.y,
				SRCCOPY);
			GdiTransparentBlt(hdc,
				dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hBlendDC,
				0, 0,
				_imageInfo.size.x, _imageInfo.size.y,
				_transColor);
			break;
		}
		
		break;
	case false:
		GdiTransparentBlt(hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hMemDC,
			0, 0,
			_imageInfo.size.x, _imageInfo.size.y,
			SRCCOPY);
		break;
	}
}

void Image::release(int16 identityNumber)
{
	assert(_identityNumber == identityNumber);

	SelectObject(_imageInfo.hMemDC, _imageInfo.hOldBit);
	DeleteObject(_imageInfo.hBitmap);
	DeleteDC(_imageInfo.hMemDC);

	if (isBigSize()) return;

	SelectObject(_imageInfo.hTempDC, _imageInfo.hTempOldBit);
	DeleteObject(_imageInfo.hTempBitmap);
	DeleteDC(_imageInfo.hTempDC);

	SelectObject(_imageInfo.hBlendDC, _imageInfo.hBlendOldBit);
	DeleteObject(_imageInfo.hBlendBitmap);
	DeleteDC(_imageInfo.hBlendDC);

	SelectObject(_imageInfo.hRotateDC, _imageInfo.hRotateOldBit);
	DeleteObject(_imageInfo.hRotateBitmap);
	DeleteDC(_imageInfo.hRotateDC);
}

void Image::initDatas(POINT sp, FPOINT scale, POINT size)
{
	_imageInfo.resID = 0;
	_imageInfo.scale = move(scale);
	_imageInfo.size = move(size);
	generateIdentityNumber(_identityNumber);
	setStartPosition(move(sp));
	setPrintSize();

	checkBigSize(BigSize.x, BigSize.y);
}

void Image::initDatas(FPOINT scale, POINT size, POINT sp, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	initDatas(move(sp), move(scale), move(size));

	setPrintSize();

	_reverseData = move(reverseData);
	_isTrans = isTrans;
	_transColor = transColor;
}

void Image::initDatas(FPOINT scale, POINT size, POINT sp, POINT keyFrame, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	_imageInfo.keyFrameSize = { size.x / keyFrame.x, size.y / keyFrame.y };
	initDatas(move(scale), move(size), move(sp), isTrans, move(reverseData), transColor);

	_imageInfo.curKeyFrame = { 0, 0 };
	_imageInfo.maxKeyFrame = { keyFrame.x - 1, keyFrame.y - 1 };
}

void Image::renderFrame(const HDC& hdc, const POINT& dest) const
{
	switch (_reverseData.x || _reverseData.y)
	{
	case false:
		switch (_isTrans)
		{
		case true:
			switch (_imageInfo.size.x >= _imageInfo.printSize.x)
			{
			case true:
				GdiTransparentBlt(hdc,
					dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
					_imageInfo.printSize.x, _imageInfo.printSize.y,

					_imageInfo.hMemDC,
					_imageInfo.keyFrameSize.x * _imageInfo.curKeyFrame.x, _imageInfo.keyFrameSize.y * _imageInfo.curKeyFrame.y,
					_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
					_transColor);
				break;
			case false:
				StretchBlt(_imageInfo.hBlendDC,
					0, 0,
					_imageInfo.printSize.x, _imageInfo.printSize.y,

					_imageInfo.hMemDC,
					_imageInfo.keyFrameSize.x * _imageInfo.curKeyFrame.x, _imageInfo.keyFrameSize.y * _imageInfo.curKeyFrame.y,
					_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
					SRCCOPY);
				GdiTransparentBlt(hdc,
					dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
					_imageInfo.printSize.x, _imageInfo.printSize.y,

					_imageInfo.hBlendDC,
					0, 0,
					_imageInfo.printSize.x, _imageInfo.printSize.y,
					_transColor);
				break;
			}
			break;
		case false:
			GdiTransparentBlt(hdc,
				dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hMemDC,
				_imageInfo.keyFrameSize.x * _imageInfo.curKeyFrame.x, _imageInfo.keyFrameSize.y * _imageInfo.curKeyFrame.y,
				_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
				SRCCOPY);
			break;
		}
		break;
	case true:
		POINT blendDest{ 0,0 };
		POINT blendSize = _imageInfo.keyFrameSize;
		if (_reverseData.x)
		{
			blendSize.x = -_imageInfo.keyFrameSize.x - 1;
			blendDest.x = _imageInfo.keyFrameSize.x;
		}
		if (_reverseData.y)
		{
			blendSize.y = -_imageInfo.keyFrameSize.y - 1;
			blendDest.y = _imageInfo.keyFrameSize.y;
		}

		StretchBlt(_imageInfo.hBlendDC,
			blendDest.x, blendDest.y,
			blendSize.x, blendSize.y,

			_imageInfo.hMemDC,
			_imageInfo.keyFrameSize.x * _imageInfo.curKeyFrame.x, _imageInfo.keyFrameSize.y * _imageInfo.curKeyFrame.y,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			SRCCOPY);
		GdiTransparentBlt(hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			_transColor);
		break;
	}
}

void Image::renderFrame(const HDC& hdc, const POINT& dest, const POINT& keyFrame) const
{
	switch (_isTrans == true)
	{
	case true:
		GdiTransparentBlt(hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hMemDC,
			_imageInfo.keyFrameSize.x * keyFrame.x, _imageInfo.keyFrameSize.y * keyFrame.y,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			_transColor);
		break;
	case false:
		GdiTransparentBlt(hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hMemDC,
			_imageInfo.keyFrameSize.x * keyFrame.x, _imageInfo.keyFrameSize.y * keyFrame.y,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			SRCCOPY);
		break;
	}
}

void Image::renderFrame(const HDC& hdc, const POINT& dest, const POINT& keyFrame, const POINT& size, const POINT& printSize) const
{
	switch (_isTrans)
	{
	case true:
		GdiTransparentBlt(hdc,
			dest.x, dest.y,
			printSize.x, printSize.y,

			_imageInfo.hMemDC,
			_imageInfo.keyFrameSize.x * keyFrame.x, _imageInfo.keyFrameSize.y * keyFrame.y,
			size.x, size.y,
			_transColor);
		break;
	case false:
		GdiTransparentBlt(hdc,
			dest.x, dest.y,
			printSize.x, printSize.y,

			_imageInfo.hMemDC,
			_imageInfo.keyFrameSize.x * keyFrame.x, _imageInfo.keyFrameSize.y * keyFrame.y,
			size.x, size.y,
			SRCCOPY);
		break;
	}
}

void Image::renderAnimation(const HDC& hdc, const POINT& dest, const POINT& sourcePoint) const
{
	switch (_reverseData.x || _reverseData.y)
	{
	case false:
		switch (_isTrans)
		{
		case true:
			GdiTransparentBlt(hdc,
				dest.x + _imageInfo.startPosition.x,
				dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hMemDC,
				sourcePoint.x, sourcePoint.y,
				_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
				_transColor);
			break;
		case false:
			GdiTransparentBlt(hdc,
				dest.x + _imageInfo.startPosition.x,
				dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hMemDC,
				sourcePoint.x, sourcePoint.y,
				_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
				SRCCOPY);
			break;
		}
		break;
	case true:
		POINT blendDest{ 0,0 };
		POINT blendSize = _imageInfo.keyFrameSize;
		if (_reverseData.x)
		{
			blendSize.x = -_imageInfo.keyFrameSize.x - 1;
			blendDest.x = _imageInfo.keyFrameSize.x;
		}
		if (_reverseData.y)
		{
			blendSize.y = -_imageInfo.keyFrameSize.y - 1;
			blendDest.y = _imageInfo.keyFrameSize.y;
		}

		StretchBlt(_imageInfo.hBlendDC,
			blendDest.x, blendDest.y,
			blendSize.x, blendSize.y,

			_imageInfo.hMemDC,
			sourcePoint.x, sourcePoint.y,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			SRCCOPY);
		GdiTransparentBlt(hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			_transColor);
		break;
	}
}

void Image::renderTransparentAnimation(const HDC& hdc, const POINT& dest, const POINT& sourcePoint) const
{
	switch (_reverseData.x || _reverseData.y)
	{
	case false:
		switch (_isTrans)
		{
		case true:
				// 여러 그림들을 동일한 DC에 계속해서 그려내는 것을 방지하기 위하여 청소하는 역할
			BitBlt(_imageInfo.hBlendDC,
				0, 0,
				_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,

				hdc,
				dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
				SRCCOPY);
			GdiTransparentBlt(_imageInfo.hBlendDC,
				0, 0,
				_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,

				_imageInfo.hMemDC,
				sourcePoint.x, sourcePoint.y,
				_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
				_transColor);
			AlphaBlend(hdc,
				dest.x + _imageInfo.startPosition.x,
				dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hBlendDC,
				0, 0,
				_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
				_blendFunc);
			break;
		case false:
			break;
		}
		break;
	case true:
		POINT blendDest{ 0,0 };
		POINT blendSize = _imageInfo.keyFrameSize;
		if (_reverseData.x)
		{
			blendSize.x = -_imageInfo.keyFrameSize.x - 1;
			blendDest.x = _imageInfo.keyFrameSize.x;
		}
		if (_reverseData.y)
		{
			blendSize.y = -_imageInfo.keyFrameSize.y - 1;
			blendDest.y = _imageInfo.keyFrameSize.y;
		}

		BitBlt(_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,

			hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			SRCCOPY);
		StretchBlt(_imageInfo.hTempDC,
			blendDest.x, blendDest.y,
			blendSize.x, blendSize.y,

			_imageInfo.hMemDC,
			sourcePoint.x, sourcePoint.y,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			SRCCOPY);
		GdiTransparentBlt(_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,

			_imageInfo.hTempDC,
			0, 0,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			_transColor);
		AlphaBlend(hdc,
			dest.x + _imageInfo.startPosition.x,
			dest.y + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			_blendFunc);
	}
}

void Image::renderTransparent(const HDC& hdc, const POINT& dest) const
{
	switch (_isTrans)
	{
	case true:
		GdiTransparentBlt(_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hMemDC,
			0, 0,
			_imageInfo.size.x, _imageInfo.size.y,
			_transColor);
		AlphaBlend(hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.printSize.x, _imageInfo.printSize.y,
			_blendFunc);
		break;
	case false:
		switch (_imageInfo.keyFrameSize.y != 0)
		{
		case true:
			AlphaBlend(hdc,
				dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hMemDC,
				_imageInfo.keyFrameSize.x * _imageInfo.curKeyFrame.x, _imageInfo.keyFrameSize.y * _imageInfo.curKeyFrame.y,
				_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
				_blendFunc);
			break;
		case false:
			AlphaBlend(hdc,
				dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
				_imageInfo.printSize.x, _imageInfo.printSize.y,

				_imageInfo.hMemDC,
				0, 0,
				_imageInfo.size.x, _imageInfo.size.y,
				_blendFunc);
			break;
		}		
		break;
	}
}

void Image::renderTransparentFrame(const HDC& hdc, const POINT& dest, const POINT& keyFrame) const
{
	const POINT selectedKeyFrame = (keyFrame.x == 0 && keyFrame.y == 0) ? _imageInfo.curKeyFrame : keyFrame;

	switch (_isTrans)
	{
	case true:
		// 여러 그림들을 동일한 DC에 계속해서 그려내는 것을 방지하기 위하여 청소하는 역할
		BitBlt(_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,

			hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			SRCCOPY);
		GdiTransparentBlt(_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,

			_imageInfo.hMemDC,
			_imageInfo.keyFrameSize.x * selectedKeyFrame.x, _imageInfo.keyFrameSize.y * selectedKeyFrame.y,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			_transColor);
		AlphaBlend(hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			_blendFunc);
		break;
	case false:
		AlphaBlend(hdc,
			dest.x + _imageInfo.startPosition.x, dest.y + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,

			_imageInfo.hMemDC,
			_imageInfo.keyFrameSize.x * selectedKeyFrame.x, _imageInfo.keyFrameSize.y * selectedKeyFrame.y,
			_imageInfo.keyFrameSize.x, _imageInfo.keyFrameSize.y,
			_blendFunc);
		break;
	}
}

void Image::renderTransparentFrame(const HDC& hdc, const POINT& dest, const POINT& keyFrame, const POINT& size, const POINT& printSize, const BYTE alpha)
{
	setTransparency(alpha);

	switch (_isTrans)
	{
	case true:
		// 여러 그림들을 동일한 DC에 계속해서 그려내는 것을 방지하기 위하여 청소하는 역할
		BitBlt(_imageInfo.hBlendDC,
			0, 0,
			printSize.x, printSize.y,
			hdc,
			dest.x, dest.y,
			SRCCOPY);
		GdiTransparentBlt
		(_imageInfo.hBlendDC,
			0, 0,
			printSize.x, printSize.y,

			_imageInfo.hMemDC,
			_imageInfo.keyFrameSize.x * keyFrame.x, _imageInfo.keyFrameSize.y * keyFrame.y,
			size.x, size.y,
			_transColor);
		AlphaBlend(hdc,
			dest.x, dest.y,
			printSize.x, printSize.y,

			_imageInfo.hBlendDC,
			0, 0,
			printSize.x, printSize.y,
			_blendFunc);
		break;
	case false:
		AlphaBlend(hdc,
			dest.x, dest.y,
			printSize.x, printSize.y,

			_imageInfo.hMemDC,
			_imageInfo.keyFrameSize.x * keyFrame.x, _imageInfo.keyFrameSize.y * keyFrame.y,
			size.x, size.y,
			_blendFunc);
		break;
	}
}

void Image::renderRotation(const HDC& hdc, const POINT& dest, uint16 rotationAngle) const
{
	POINT apt[3] = { 0 };
	float dblWidth = static_cast<float>(_imageInfo.printSize.x * _imageInfo.scale.x);
	float dblHeight = static_cast<float>(_imageInfo.printSize.y * _imageInfo.scale.y);
	float ixRotate = static_cast<float>((_imageInfo.printSize.x / 2) * _imageInfo.scale.x); // 크기가 변하는 것 고려
	float iyRotate = static_cast<float>((_imageInfo.printSize.y / 2) * _imageInfo.scale.y);
	float dblx, dbly, dblxDest, dblyDest;
	float dblRadian = TrigonometricController::getDegreeToRadian(rotationAngle);
	float cosVal = cos(dblRadian), sinVal = sin(dblRadian);

	// 1. 회전축을 기준으로 상대좌표를 구하고
	// 2. 회전후 위치좌표(상대좌표)를 얻은 후
	// 3. 얻은 값을 원래의 좌표에 적용.
	for (int i = 0; i < 3; i++)
	{
		if (i == 0) { dblx = -ixRotate, dbly = -iyRotate; }    // left up  꼭지점 부분
		else if (i == 1) { dblx = dblWidth - ixRotate, dbly = -iyRotate; }  // right up 꼭지점 부분
		else if (i == 2) { dblx = -ixRotate, dbly = dblHeight - iyRotate; } // left low 꼭지점 부분
		dblxDest = dblx * cosVal - dbly * sinVal;
		dblyDest = dblx * sinVal + dbly * cosVal;
		dblxDest += ixRotate, dblyDest += iyRotate;
		apt[i].x = dest.x - (long)ixRotate + (long)dblxDest;
		apt[i].y = dest.y - (long)iyRotate + (long)dblyDest;
	}

	PlgBlt(hdc, apt, _imageInfo.hMemDC, 
		0, 0,
		_imageInfo.printSize.x, _imageInfo.printSize.y, 
		nullptr, 0, 0);
}

void Image::renderRotatingFrame(const HDC& hdc, const POINT& dest, const POINT& keyFrame, int16 rotationAngle) const
{
	POINT apt[3] = { 0 };
	float dblWidth = static_cast<float>(_imageInfo.printSize.x * _imageInfo.scale.x);
	float dblHeight = static_cast<float>(_imageInfo.printSize.y * _imageInfo.scale.y);
	float ixRotate = static_cast<float>((_imageInfo.printSize.x / 2) * _imageInfo.scale.x); // 크기가 변하는 것 고려
	float iyRotate = static_cast<float>((_imageInfo.printSize.y / 2) * _imageInfo.scale.y);
	float dblx, dbly, dblxDest, dblyDest;
	float dblRadian = TrigonometricController::getRadianToDegree(rotationAngle);
	float cosVal = cos(dblRadian), sinVal = sin(dblRadian);

	// 1. 회전축을 기준으로 상대좌표를 구하고
	// 2. 회전후 위치좌표(상대좌표)를 얻은 후
	// 3. 얻은 값을 원래의 좌표에 적용.
	for (int i = 0; i < 3; i++)
	{
		if (i == 0) { dblx = -ixRotate, dbly = -iyRotate; }    // left up  꼭지점 부분
		else if (i == 1) { dblx = dblWidth - ixRotate, dbly = -iyRotate; }  // right up 꼭지점 부분
		else if (i == 2) { dblx = -ixRotate, dbly = dblHeight - iyRotate; } // left low 꼭지점 부분
		dblxDest = dblx * cosVal - dbly * sinVal;
		dblyDest = dblx * sinVal + dbly * cosVal;
		dblxDest += ixRotate, dblyDest += iyRotate;
		apt[i].x = (dest.x + _imageInfo.startPosition.x) - (long)ixRotate + (long)dblxDest;
		apt[i].y = (dest.y + _imageInfo.startPosition.y) - (long)iyRotate + (long)dblyDest;
	}

	PlgBlt(hdc, apt, _imageInfo.hMemDC,
		_imageInfo.keyFrameSize.x * keyFrame.x, _imageInfo.keyFrameSize.y * keyFrame.y,
		_imageInfo.printSize.x, _imageInfo.printSize.y,
		nullptr, 0, 0);
}

void Image::renderTransparentRotatingFrame(const HDC& hdc, const POINT& dest, const POINT& keyFrame, int16 rotationAngle) const
{
	switch (_isTrans)
	{
	case true:
	{
		// 1. 목적지 DC에 그려져 있는 내용을 blend DC에 복사
		BitBlt(_imageInfo.hBlendDC, 
			0, 0,
			_imageInfo.printSize.x, _imageInfo.printSize.y,
			hdc,
			dest.x + _imageInfo.startPosition.x,
			dest.y + _imageInfo.startPosition.y,
			SRCCOPY);

		// 2. 이미지를 회전시켜서 Rotate DC에 복사
		RECT rc = { 0, 0, _imageInfo.rotateMaxSize, _imageInfo.rotateMaxSize };
		HBRUSH hBrush = CreateSolidBrush(_transColor);
		FillRect(_imageInfo.hRotateDC, &rc, hBrush);
		DeleteObject(hBrush);

		renderRotatingFrame(_imageInfo.hRotateDC, {}, {}, rotationAngle);

		// 3. Rotate DC 이미지를 blend DC에 지정한 색상을 제외해서 복사
		GdiTransparentBlt(_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.printSize.x, _imageInfo.printSize.y,
			_imageInfo.hRotateDC,
			(_imageInfo.rotateMaxSize / 2) - (_imageInfo.printSize.x / 2),
			(_imageInfo.rotateMaxSize / 2) - (_imageInfo.printSize.y / 2),
			_imageInfo.printSize.x, _imageInfo.printSize.y,
			_transColor);

		// 4. blend DC에 내용을 목적지 DC에 복사
		AlphaBlend(hdc,
			dest.x + _imageInfo.startPosition.x,
			dest.x + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,
			_imageInfo.hBlendDC,
			0, 0,
			_imageInfo.printSize.x, _imageInfo.printSize.y,
			_blendFunc);
	}
		break;
	case false:
		AlphaBlend(hdc,
			dest.x + _imageInfo.startPosition.x,
			dest.x + _imageInfo.startPosition.y,
			_imageInfo.printSize.x, _imageInfo.printSize.y,
			_imageInfo.hMemDC, 
			0, 0, 
			_imageInfo.printSize.x, _imageInfo.printSize.y,
			_blendFunc);
		break;
	}
}

void Image::resetSize(POINT startPosition, POINT size, POINT printSize)
{
	_imageInfo.size = move(size);
	setStartPosition(move(startPosition));
	setPrintSize(move(printSize));
}

void Image::resetSpAndPrintSizeOfImage(POINT startPosition, POINT printSize)
{
	setStartPosition(move(startPosition));
	setPrintSize(move(printSize));
}

void Image::resetSpAndScaleOfImage(POINT startPosition, FPOINT scale)
{
	_imageInfo.scale = move(scale);
	setStartPosition(move(startPosition));
	setPrintSize();
}

void Image::setMemDC(const HDC& hdc, const char* fileName, const POINT& size)
{
	_imageInfo.hMemDC = CreateCompatibleDC(hdc); // 이 DC와 연결된 비트맵 객체에는 그림을 그림	
	switch (fileName == nullptr) // 위 DC를 사용하려면 여기서 비트맵 객체를 생성한다.
	{
	case true:
		_imageInfo.hBitmap = CreateCompatibleBitmap(hdc, size.x, size.y);
		break;
	case false:
		_imageInfo.hBitmap = static_cast<HBITMAP>(LoadImageA(G_hInstance, fileName, IMAGE_BITMAP, size.x, size.y, LR_LOADFROMFILE));
		break;
	}
	_imageInfo.hOldBit = static_cast<HBITMAP>(SelectObject(_imageInfo.hMemDC, _imageInfo.hBitmap)); // MemDC와 Bitmap과 연결한다. 
}

void Image::setTempDC(const HDC& hdc, const POINT& size)
{
	_imageInfo.hTempDC = CreateCompatibleDC(hdc);
	_imageInfo.hTempBitmap = CreateCompatibleBitmap(hdc, size.x, size.y);
	_imageInfo.hTempOldBit = static_cast<HBITMAP>(SelectObject(_imageInfo.hTempDC, _imageInfo.hTempBitmap));
}

void Image::setBlendDC(const HDC& hdc, const POINT& size)
{
	_imageInfo.hBlendDC = CreateCompatibleDC(hdc);
	_imageInfo.hBlendBitmap = CreateCompatibleBitmap(hdc, size.x, size.y);
	_imageInfo.hBlendOldBit = static_cast<HBITMAP>(SelectObject(_imageInfo.hBlendDC, _imageInfo.hBlendBitmap));
}

void Image::setRotateDC(const HDC& hdc, const POINT& size)
{
	const int16 maxSize = _imageInfo.printSize.x > _imageInfo.printSize.y ? _imageInfo.printSize.x : _imageInfo.printSize.y;
	_imageInfo.rotateMaxSize = maxSize * 2;
	_imageInfo.hRotateDC = CreateCompatibleDC(hdc);
	_imageInfo.hRotateBitmap = CreateCompatibleBitmap(hdc, maxSize * 2, maxSize * 2);
	_imageInfo.hRotateOldBit = static_cast<HBITMAP>(SelectObject(_imageInfo.hRotateDC, _imageInfo.hRotateBitmap));
}

void Image::setHDCs(const HDC& hdc, const char* fileName, const POINT& size)
{
	setMemDC(hdc, move(fileName), size);
	setRotateDC(hdc, size);
	if (isBigSize() && fileName != "Images/Scene/Maptool/Canvas.bmp") 
		return;

	POINT calculatedSize = { size.x, size.y };
	// 이렇게 하는 이유는 임시 복사 DC들은 원본 사이즈보다 커야 하는데, 가장 큰 사이즈가 밑에 계산한 스케일 값을 곱했을 때 크기 때문이다.
	if (calculatedSize.x < GetSystemMetrics(SM_CXSCREEN))
	{
		calculatedSize.x = 1.4f * calculatedSize.x;
		calculatedSize.y = 1.4f * calculatedSize.y;
	}

	setTempDC(hdc, calculatedSize);
	setBlendDC(hdc, calculatedSize);
}

void Image::setPrintSize()
{
	switch (_imageInfo.keyFrameSize.x == 0 && _imageInfo.keyFrameSize.y == 0)
	{
	case true:
		_imageInfo.printSize = { static_cast<LONG>(_imageInfo.scale.x * _imageInfo.size.x), static_cast<LONG>(_imageInfo.scale.y * _imageInfo.size.y) };
		break;
	case false:
		_imageInfo.printSize = { static_cast<LONG>(_imageInfo.scale.x * _imageInfo.keyFrameSize.x), static_cast<LONG>(_imageInfo.scale.y * _imageInfo.keyFrameSize.y) };
		break;
	}
}

void Image::processHDCAndDCs(const char* fileName, POINT size)
{
	HDC hdc = GetDC(G_hWnd); // 윈도우에 그림을 그리는 DC
	setHDCs(hdc, move(fileName), move(size));
	ReleaseDC(G_hWnd, hdc); // DC는 개수가 한정적!, 사용 후 반납
	checkNullptrOfDCs();
}

void Image::generateIdentityNumber(int16& identityN)
{
	static uint16 generateNumber = 0;
	identityN = generateNumber;
	++generateNumber;
}

void Image::checkNullptrOfDCs() const
{
	assert(_imageInfo.hBitmap);

	if (isBigSize()) return;

	assert(_imageInfo.hTempDC && _imageInfo.hBlendBitmap && _imageInfo.hRotateBitmap);
}

