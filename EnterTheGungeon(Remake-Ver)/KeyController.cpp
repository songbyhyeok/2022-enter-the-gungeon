#include "KeyController.h"

/*
* GetAsyncKeyState() 함수 반환 값
0 (0x0000)
이전에 누른 적이 없고 호출 시점에서 안눌린 상태

0x8000 = -32768
이전에 누른 적이 없고 호출 시점에서 눌린 상태

0x8001 = -32767
이전에 누른 적이 있고 호출 시점에서 눌린 상태
-> (추가적 TMI) 해당 키를 GetAsyncKeyState로 연속 두 번 호출 시 0x8001을 반환한다.

1 (0x0001)
이전에 누른 적이 있고 호출 시점에서 안눌린 상태

summary
이전에 누른 적이 없고 호출 시점에서 눌린 상태, 즉 키가 눌려진 시점에서 0x8000을 반환합니다. 
만약 이전에도 누른 적이 있고 호출 시점에서도 눌렸으면 0x8001을 반환합니다. 
이전에 키를 눌렀으면 0x0001을 반환하고, 눌리지 않았다면 0x0000을 반환합니다. 
혹시, GetAsyncKeyState(VK_UP) & 0x8000 이런 코드를 보신적이 있으신가요? 
0x8000과 GetAsyncKeyState 반환값을 AND 연산하는 이유는 정확한 시점에서 키의 상태를 확인하기 위함입니다. 
만약에 키를 눌러 GetAsyncKeyState 함수가 0x8000을 반환한다면 이를 0x8000으로 AND 연산하여 키가 눌렸을때는 0x8000이 나옵니다. 
그러나 눌려진 적이 있으면 GetAsyncKeyState 함수는 0x0001을 반환한다고 했었습니다. 
0x0001에다 0x8000을 AND 연산하게되면 0이 나오게 됩니다. (만약에 AND 연산하지 않았을 경우에는 눌러진 적이 있는 상태도 참이 되어 동작을 수행할 것입니다.
*/

/*
* GetAsyncKeyState(key) 함수 설명
* input key를 눌렀다면 DEC -32767을 반환합니다. (bit 16자리)
* 누르지 않았다면 0을 반환합니다.
* 
* 비트 연산 & 설명
* key를 눌렀을 때 GetAsyncKeyState 리턴 값 -32767 & 0x8000(DEC 32768) = 1을 반환합니다.
*/

bitset<KeyMaxCount> KeyController::KeyUp;
bitset<KeyMaxCount> KeyController::KeyDown;

void KeyController::init()
{
    KeyUp.reset();
    KeyDown.reset();
}

bool KeyController::isOnceKeyDown(int16 key)
{
    switch (static_cast<bool>(GetAsyncKeyState(key) & 0x8000))
    {
    case true:
        if (KeyDown[key] == false) // 키가 이전에 눌리지 않은 상태
        {
            KeyDown.set(key, true); // 눌린 상태로 변환
            return true;
        }
        break;
    case false:
        KeyDown.set(key, false);
        break;
    }

    return false;
}

bool KeyController::isOnceKeyUp(int16 key)
{
    /*
    * 함수 호출 시 처음에는 keyUp = true으로 바꾸고 false로 return 한다.
    * 그 다음 한 턴 이후 재 호출 시 키를 누른 적이 없으므로 false로 가면서 keyUp이 true으로 눌렀던 적이 있다는 것을 알고 false 초기화 이후 true 리턴 합니다.
    * 
    * 한 턴 이후 반응 값을 얻고 싶다면 onceKeyUp을 쓴다.
    */

    switch (static_cast<bool>(GetAsyncKeyState(key) & 0x8000))
    {
    case true:
        KeyUp.set(key, true); // 키를 눌렀다면 처음에 keyUp 변수 값을 true으로 바꾸고 false 리턴합니다.
        break;
    case false:
        if (KeyUp[key] == true) // 다시 한 턴 돌아서 검사를 할 때 이전 값 keyUp이 true 즉 누른 적이 있다면
        {
            KeyUp.set(key, false); // false로 초기화를 하고 return true을 합니다.
            return true;
        }
        break;
    }

    return false;
}

bool KeyController::isStayKeyDown(int16 key)
{
    if (GetAsyncKeyState(key) & 0x8000)
        return true;

    return false;
}

bool KeyController::isToggleKey(int16 key)
{
    // 키를 눌렀다면 -32767을 반환합니다. 그리고 & 0x0001 연산 시 1을 반환합니다.
    // 키를 누르지 않았다면 0을 반환합니다. 그리고 & 0x0001 연산 시 0을 반환합니다.

    // 누르자마자 반응을 얻고 싶다면 toggleKey를 사용한다.

    // And& 
    // 0000 0000 0000 0001 이전에 키를 눌렀었음 0x0001 (지난 번 호출과 이번 호출 사이에 키가 눌려진 적이 있었다.)
    // 0000 0000 0000 0001 0x0001
    // 0000 0000 0000 0001
    return (GetAsyncKeyState(key) & 0x0001) == true ? true : false;
}
