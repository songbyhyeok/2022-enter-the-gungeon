#pragma once

#include "pch.h"

constexpr int16 VK_Akey = 0x41;
constexpr int16 VK_Ckey = 0x43;
constexpr int16 VK_Dkey = 0x44;
constexpr int16 VK_Ekey = 0x45;
constexpr int16 VK_Ikey = 0x49;
constexpr int16 VK_Pkey = 0x50;
constexpr int16 VK_Qkey = 0x51;
constexpr int16 VK_Rkey = 0x52;
constexpr int16 VK_Skey = 0x53;
constexpr int16 VK_Wkey = 0x57;
constexpr int16 VK_Xkey = 0x58;
constexpr int16 VK_Ykey = 0x59;
constexpr int16 VK_Zkey = 0x5A;
constexpr int16 KeyMaxCount = 166;
class KeyController
{
public:
	static void init();

	static bool isOnceKeyDown(int16 key);
	static bool isOnceKeyUp(int16 key);
	static bool isStayKeyDown(int16 key);
	static bool isToggleKey(int16 key);
	
private:
	static bitset<KeyMaxCount> KeyUp;
	static bitset<KeyMaxCount> KeyDown;
};

