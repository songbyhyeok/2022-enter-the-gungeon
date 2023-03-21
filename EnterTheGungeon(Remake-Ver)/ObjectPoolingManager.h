#pragma once

#include "pch.h"

template<typename T>
class ObjectPoolingManager
{
public:
	ObjectPoolingManager() = default;
	ObjectPoolingManager(T object, const uint16 count)
	{
		init(move(object), count);
	}

	void init(T& object, const uint16 count)
	{
		_selectedObject = move(object);

		_objectsVector.reserve(count);
		for (int i = 0; i < count; i++)
		{
			_objectsVector.emplace_back(new T(_selectedObject));
		}
	}

	void release()
	{
		while (!_objectsVector.empty())
		{
			delete _objectsVector[_objectsVector.size() - 1];
			_objectsVector.pop_back();
		}
	}

	T* getObjectFromPools()
	{
		if (_objectsVector.empty())
		{
			expand();
		}

		T* selectedObject = _objectsVector[_objectsVector.size() - 1];
		_objectsVector.pop_back();
		return selectedObject;
	}

	void returnObject(T* object) { _objectsVector.emplace_back(object); }
	const uint16	getObjectsVecSize() const { return _objectsVector.size(); }

private:	
	void expand()
	{
		for (int i = 0; i < _objectsVector.capacity(); i++)
		{
			_objectsVector.emplace_back(new T(_selectedObject));
		}
	}

private:
	vector<T*>						_objectsVector;
	T								_selectedObject;
};
