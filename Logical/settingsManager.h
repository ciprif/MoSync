/*
 * settingsManager.h
 *
 *  Created on: Jul 17, 2012
 *      Author: Cipri
 */

#ifndef SETTINGSMANAGER_H_
#define SETTINGSMANAGER_H_

#include "../Model/util.h"

#define SETTINGS_FILE "settings.txt"
#define BUFF_SIZE 1024

namespace Logical
{
	const int DEFAULT_DAY = 1;
	const int DEFAULT_MONTH = 1;
	const int DEFAULT_YEAR = 2012;

	class SettingsManager
	{
	public:
		SettingsManager();
		~SettingsManager();

		void LoadSettings();
		void ApplySettings();

		MAUtil::String& getCoin();
		Model::DateStruct& getDate();
		double& getDebtValue();

		bool isShowAll();
		bool isShowMonthly();
		bool isShowFromDate();

		void setIsShowAll(bool);
		void setIsShowMontly(bool);
		void setIsShowFromDate(bool);
		void setDateFrom(const Model::DateStruct&);
		void setCoin(const MAUtil::String&);
		void setDebtValue(const double&);
	private:
		void _readSettings();
		void _writeSettings();

		MAHandle _settingsFile;

		MAUtil::String _coin;
		Model::DateStruct _date;
		bool _showAll;
		bool _showMonthly;
		bool _showFromDate;
		double _debtValue;
	};
}


#endif /* SETTINGSMANAGER_H_ */
