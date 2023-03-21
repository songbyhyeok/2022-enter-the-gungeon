#include "DataHandler.h"
#include "Option.h"

void DataHandler::init(const char* iniSource, Option* option)
{
	_iniFile = move(iniSource);
	setOption(option);
	(checkExistenceAboutIniFile() == true) ? readIniFile() : writeIniFile();
}

bool DataHandler::checkExistenceAboutIniFile() const
{
	char strReadIni[20];
	::GetPrivateProfileStringA("OptionDatas", "Existence", "False", strReadIni, 20, _iniFile);
	return (strcmp(strReadIni, "True") == false) ? true : false;
}

void DataHandler::readIniFile() const
{
	(*_optionPtr)->readAndWriteIniFile(_iniFile);
}

void DataHandler::writeIniFile() const
{
	(*_optionPtr)->writeIniFile(_iniFile);
}
