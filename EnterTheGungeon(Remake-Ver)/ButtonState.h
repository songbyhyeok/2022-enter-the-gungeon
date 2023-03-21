#pragma once

#include "Button.h"
#include "SoundController.h"
#include "FunctionsStorage.h"

class ButtonState
{
public:
	void							init(Button* buttonUptr, SOUND_FILE soundFile, function<void(const shared_ptr<FunctionsStorage>&, const vector<VPointerData>&)> function, vector<VPointerData> vPtrDataVec);
	void							release();
	
	const string&					getSoundFileSource() const { return _soundFile.source; }
	void							setButtonStateWith(shared_ptr<ButtonState>& toBtnState, shared_ptr<ButtonState> fromBtnState) const { toBtnState = fromBtnState; }
	const char*						getTypeID() const { return _typeID; }

	bool							checkTransitionPossible(const shared_ptr<ButtonState>& buttonState);

	void							stopSound() const { SoundController::getInstance().stop(_soundFile); }
	void							playSound() const { SoundController::getInstance().play(_soundFile); }
	void							launchToFunction(const shared_ptr<FunctionsStorage>& launchPermission) const { if (_function) _function(launchPermission, _encryptedDatas); }
	
	static int16					toInt(const char* btnStTypeId);
		
	virtual							~ButtonState() = default;

protected:
	bool							isPass() const { return _isPass; }

protected:
	function<void(const shared_ptr<FunctionsStorage>&, const vector<VPointerData>&)>		_function;
	SOUND_FILE																				_soundFile;
	vector<VPointerData>																	_encryptedDatas;
	unique_ptr<Button*>																		_buttonUptr;
	const char*																				_typeID;
	bool																					_isPass;
};

class Disabled : public ButtonState
{
public:
	Disabled(bool isPass = false)
	{
		_typeID = move(typeid(Disabled).name());
		_isPass = isPass;
	}

	void toggleOff() const { (*_buttonUptr)->toggle(false); }
};

class Active : public ButtonState
{
public:
	Active(bool isPass = false)
	{
		_typeID = move(typeid(Active).name());
		_isPass = isPass;
	}

	void toggleOn() const { (*_buttonUptr)->toggle(true); }
};

class Pressed : public ButtonState
{
public:
	Pressed(bool isPass = false)
	{
		_typeID = move(typeid(Pressed).name());
		_isPass = isPass;
	}
};

class PressUp : public ButtonState
{
public:
	PressUp(bool isPass = false)
	{
		_typeID = move(typeid(PressUp).name());
		_isPass = isPass;
	}
};

