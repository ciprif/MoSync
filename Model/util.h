/*
 * util.h
 *
 *  Created on: Jun 28, 2012
 *      Author: Cipri
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <MAUtil/String.h>
#include <MAUtil/util.h>
#include <matime.h>

namespace GUI
{

	const int GREEN = 0x00FF00;
	const int YELLOW = 0xFFFF00;
	const int RED = 0xFF0000;
	const int DIALOG_FONT_SIZE = 25;
	const int DIALOG_FONT_SIZE_SMALL = 18;
	const int DESCRIPTION_EDIT_BOX_HEIGHT = 200;
	const int DESCRIPTION_EDIT_BOX_LINES = 10;
	const int DIALOG_HEIGHT = 750;
	const int DIALOG_BUTTON_WIDTH = 140;
	const int DIALOG_BOTTOM_LINE_HEIGHT = 20;
	const int DIALOG_COLOR_WHITE = 0xFFFFFF;
	const int MAX_VALUE = 9999;
	const char DEFAULT_COIN[] = "EUR";
}
namespace Model
{
	const int NO_OF_CATEGORIES = 5;
	const int NO_OF_INCOMES = 4;
	const MAUtil::String INCOME_TYPES_LIST[] = { "Bank Transfer", "Salary", "Debt", "Other" };
	const MAUtil::String CATEGORY_LIST[] = { "Food", "Clothes", "Bills", "Fun", "Other" };

	struct DateStruct
	{
		int _year;
		int _mounth;
		int _day;

		DateStruct() : _day(1), _mounth(1), _year(1601) {}
	};

	struct TimeStruct
	{
		int _hour;
		int _minutes;
	};

	static MAUtil::String DateStructToString(const DateStruct& date)
	{
		MAUtil::String s;
		s.append(MAUtil::integerToString(date._year, 10).c_str(), MAUtil::integerToString(date._year, 10).length());
		s.append("-", 1);
		s.append(MAUtil::integerToString(date._mounth, 10).c_str(), MAUtil::integerToString(date._mounth, 10).length());
		s.append("-", 1);
		s.append(MAUtil::integerToString(date._day, 10).c_str(), MAUtil::integerToString(date._day, 10).length());

		return s;
	}

	static MAUtil::String TimeStructToString(const TimeStruct& time)
	{
		MAUtil::String s;
		s.append(MAUtil::integerToString(time._hour, 10).c_str(), MAUtil::integerToString(time._hour, 10).length());
		s.append("-", 1);
		s.append(MAUtil::integerToString(time._minutes, 10).c_str(), MAUtil::integerToString(time._minutes, 10).length());

		return s;
	}

	static int CompareDateObjects(const DateStruct& date1, const DateStruct& date2)
	{
		if(date1._year > date2._year) return 1;
		else
		{
			if(date1._year < date2._year) return -1;
			else
			{
				if(date1._mounth > date2._mounth) return 1;
				else
				{
					if(date1._mounth < date2._mounth) return -1;
					else
					{
						if(date1._day > date2._day) return 1;
						else if(date1._day == date2._day) return 0;
						return -1;
					}
				}
			}
		}
	}
}

#endif /* UTIL_H_ */
