/*
 * settingsManager.cpp
 *
 *  Created on: Jul 17, 2012
 *      Author: Cipri
 */

#include <maapi.h>
#include <MAUtil/String.h>
#include <MAUtil/util.h>
#include <MAFS/File.h>
#include <conprint.h>
#include <matime.h>

#include "settingsManager.h"

namespace Logical
{
	SettingsManager::SettingsManager() : _coin("EUR"), _showAll(true), _showFromDate(false), _showMonthly(false), _debtValue(0.0)
	{
		_settingsFile = maFileOpen(SETTINGS_FILE, MA_ACCESS_READ_WRITE);

		if(maFileExists(_settingsFile))
		{
			LoadSettings();
			maFileClose(_settingsFile);
		}
		else
		{
			maFileCreate(_settingsFile);
			maFileClose(_settingsFile);
			_date._day = 1;
			_date._mounth = 1;
			_date._year = 1601;
			ApplySettings();
		}
	}

	SettingsManager::~SettingsManager()
	{

	}

	void SettingsManager::LoadSettings()
	{
		_readSettings();
	}

	void SettingsManager::ApplySettings()
	{
		_writeSettings();
	}

	MAUtil::String& SettingsManager::getCoin()
	{
		return _coin;
	}

	Model::DateStruct& SettingsManager::getDate()
	{
		return _date;
	}

	double& SettingsManager::getDebtValue()
	{
		return _debtValue;
	}

	bool SettingsManager::isShowAll()
	{
		return _showAll;
	}

	bool SettingsManager::isShowMonthly()
	{
		return _showMonthly;
	}

	bool SettingsManager::isShowFromDate()
	{
		return _showFromDate;
	}

	void SettingsManager::_readSettings()
	{
		char settingsFileContent[BUFF_SIZE];
		MAUtil::String content;

		maFileRead(_settingsFile, settingsFileContent, BUFF_SIZE);

		content.append(settingsFileContent, strlen(settingsFileContent));

		int offset = 0;
		int position = content.find("|", offset);
		_coin = content.substr(offset, position); //read the coin

		offset = position + 1;
		position = content.find("|", offset);

		int day = MAUtil::stringToInteger(content.substr(offset, position - offset), 10); //read the reset day

		offset = position + 1;
		position = content.find("|", offset);

		MAUtil::String dateString = content.substr(offset, position - offset); //read the dateString

		offset = position + 1;
		position = content.find("|", offset);

		MAUtil::String binaryMask = content.substr(offset, position - offset); //read the binary mask
		offset = position + 1;
		_debtValue = MAUtil::stringToDouble(content.substr(offset, content.length() - offset));
		lprintfln("%s", binaryMask.c_str());

		if(binaryMask ==  "100")
		{
			_showAll = true;
			_showMonthly = false;
			_showFromDate = false;
		}
		else if(binaryMask == "010")
		{
			_showAll = false;
			_showMonthly = true;
			_showFromDate = false;
		}
		else if(binaryMask == "001")
		{
			_showAll = false;
			_showMonthly = false;
			_showFromDate = true;
		}

		if(_showMonthly)
		{
			_date._day = day;

			struct tm * dateTime = new tm;
			split_time(maTime(), dateTime);

			_date._mounth = dateTime->tm_mon + 1;
			_date._year = dateTime->tm_year + 1900;

			delete dateTime;
		}
		else if(_showFromDate)
		{
			offset = 0;
			position = dateString.find("-", offset);

			_date._year = MAUtil::stringToInteger(dateString.substr(offset, position), 10);

			offset = position + 1;
			position = dateString.find("-", offset);

			_date._mounth = MAUtil::stringToInteger(dateString.substr(offset, position - offset), 10);

			offset = position + 1;
			lprintfln("%d %d", dateString.length(), offset);
			_date._day = MAUtil::stringToInteger(dateString.substr(offset, dateString.length() - offset), 10);
		}
		else if(_showAll)
		{
			_date._day = 1;
			_date._mounth = 1;
			_date._year = 1601;
		}
	}

	void SettingsManager::_writeSettings()
	{
		_settingsFile = maFileOpen(SETTINGS_FILE, MA_ACCESS_READ_WRITE);

		maFileTruncate(_settingsFile, 0);

		char buffer[BUFF_SIZE];
		MAUtil::String binaryMask;

		if(_showAll) binaryMask = "100";
		else if(_showMonthly) binaryMask = "010";
		else if(_showFromDate) binaryMask = "001";

		sprintf(buffer, "%s|%d|%s|%s|%.2f", _coin.c_str(), _date._day, Model::DateStructToString(_date).c_str(), binaryMask.c_str(), _debtValue);
		lprintfln("%s", buffer);

		maFileWrite(_settingsFile, buffer, strlen(buffer));
		maFileClose(_settingsFile);
	}

	void SettingsManager::setIsShowAll(bool arg)
	{
		_showAll = arg;
	}

	void SettingsManager::setIsShowMontly(bool arg)
	{
		_showMonthly  = arg;
	}

	void SettingsManager::setIsShowFromDate(bool arg)
	{
		_showFromDate = arg;
	}

	void SettingsManager::setDateFrom(const Model::DateStruct& arg)
	{
		_date._day = arg._day;
		_date._mounth = arg._mounth;
		_date._year = arg._year;
	}

	void SettingsManager::setCoin(const MAUtil::String& arg)
	{
		_coin = arg;
	}

	void SettingsManager::setDebtValue(const double& arg)
	{
		_debtValue = arg;
	}
}

