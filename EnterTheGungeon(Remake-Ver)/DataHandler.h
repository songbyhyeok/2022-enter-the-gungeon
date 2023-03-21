#pragma once

#include "pch.h"

struct DATA_VALUE
{
	string		value;
	const char* name;
};

class Option;
class DataHandler
{
public:
	void									init(const char* iniSource, Option * option);
	void									writeIniFile() const;

private:
	void									setOption(Option* option) { _optionPtr = make_unique<Option*>(option); }

	bool									checkExistenceAboutIniFile() const;
	void									readIniFile() const;
	
private:
	unique_ptr<Option*>						_optionPtr;
	const char*								_iniFile;
};

