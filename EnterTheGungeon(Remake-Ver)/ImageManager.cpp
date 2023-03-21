#include "ImageManager.h"

map<uint16, unique_ptr<Image>> ImageManager::ImagesMap;

const Image& ImageManager::makeImage(POINT sp, FPOINT scale, POINT size)
{
	unique_ptr<Image> image = make_unique<Image>();
	image->init(move(sp), move(scale), move(size));
	return processToHoldImage(move(image));
}

const Image& ImageManager::makeImage(const char* fileName, POINT sp, FPOINT scale, POINT size, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	unique_ptr<Image> image = make_unique<Image>();
	image->init(fileName, move(sp), move(scale), move(size), isTrans, move(reverseData), transColor);
	return processToHoldImage(move(image));
}

const Image& ImageManager::makeImage(const char* fileName, POINT sp, FPOINT scale, POINT size, POINT keyFrame, bool isTrans, REVERSE_DATA reverseData, COLORREF transColor)
{
	unique_ptr<Image> image = make_unique<Image>();
	image->init(move(fileName), move(sp), move(scale), move(size), move(keyFrame), isTrans, move(reverseData), transColor);
	return processToHoldImage(move(image));
}

void ImageManager::findImageToDelete(uint16 imageIdentityNumber)
{
	const auto& iter = ImagesMap.find(imageIdentityNumber);
	if (iter != ImagesMap.end())
	{
		iter->second->release(imageIdentityNumber);
		iter->second.reset();

		ImagesMap.erase(iter);
	}
}

void ImageManager::allDelete()
{
	for (map<uint16, unique_ptr<Image>>::iterator iter = ImagesMap.begin(); iter != ImagesMap.end();)
	{
		iter->second->release(iter->first);
		iter->second.reset();
		iter = ImagesMap.erase(iter);
	}

	ImagesMap.clear();
}

const Image& ImageManager::processToHoldImage(unique_ptr<Image> image)
{
	const uint16 imgIdentityN = image->getIdentityNumber();
	ImagesMap.insert(make_pair(imgIdentityN, move(image)));
	return *ImagesMap[imgIdentityN];
}
