#include "ButtonState.h"
#include "Button.h"

void ButtonState::init(Button* buttonUptr, SOUND_FILE soundFile, function<void(const shared_ptr<FunctionsStorage>&, const vector<VPointerData>&)> function, vector<VPointerData> vPtrDataVec)
{
	_buttonUptr = make_unique<Button*>(buttonUptr);
	_soundFile = move(soundFile);
	_function = move(function);
	_encryptedDatas = move(vPtrDataVec);
}

void ButtonState::release()
{
	for (const auto & voidPtrData : _encryptedDatas)
	{
		if (voidPtrData.second)
			delete voidPtrData.first;
	}
}

bool ButtonState::checkTransitionPossible(const shared_ptr<ButtonState>& buttonState)
{
	if (isPass() || !(*_buttonUptr)->compareButtonStates(buttonState))
	{
		return true;
	}

	return false;
}

int16 ButtonState::toInt(const char* btnStTypeId)
{
	if (btnStTypeId == typeid(Disabled).name())
		return 0;
	else if (btnStTypeId == typeid(Active).name())
		return 1;
	else if (btnStTypeId == typeid(Pressed).name())
		return 2;
	else if (btnStTypeId == typeid(PressUp).name())
		return 3;
}
