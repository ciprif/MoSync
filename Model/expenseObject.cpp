/*
 * expenseObject.cpp
 *
 *  Created on: Jun 21, 2012
 *      Author: Cipri
 */

#include "expenseObject.h"

namespace Model
{
	ExpenseObject::ExpenseObject()
	{
		_amount = 0.0;
		_description = new MAUtil::String("");
		_category = new MAUtil::String("Other");
		_imagePath = new MAUtil::String("");
	}

	ExpenseObject::ExpenseObject(const double& amount, const MAUtil::String& category, const MAUtil::String& description, const MAUtil::String& imgPath,
			  const int& year, const int& month, const int& day, const int& hour, const int& minute)
	{
		_amount = amount;
		_description = new MAUtil::String(description);
		_category = new MAUtil::String(category);
		_imagePath = new MAUtil::String(imgPath);
		_date._year = year;
		_date._mounth = month;
		_date._day = day;
		_time._hour = hour;
		_time._minutes = minute;
	}
	ExpenseObject::ExpenseObject(const ExpenseObject& obj)
	{
		_amount = obj._amount;
		_description = new MAUtil::String(*(obj._description));
		_category = new MAUtil::String(*(obj._category));
		_imagePath =new MAUtil::String(*(obj._imagePath));
		_date._day = obj._date._day;
		_date._year = obj._date._year;
		_date._mounth = obj._date._mounth;
		_time._hour = obj._time._hour;
		_time._minutes = obj._time._minutes;
	}
	ExpenseObject::~ExpenseObject()
	{
		delete _description;
		delete _imagePath;
		delete _category;
	}

	const MAUtil::String& ExpenseObject::getCategory() const
	{
		return *_category;
	}

	const double& ExpenseObject::getAmount() const
	{
		return _amount;
	}

	const MAUtil::String& ExpenseObject::getDescription() const
	{
		return *_description;
	}

	const MAUtil::String& ExpenseObject::getImagePath() const //to be discussed
	{
		return *_imagePath;
	}

	void ExpenseObject::setCategory(const MAUtil::String& category)
	{
		delete _category;
		_category = new MAUtil::String(category);
	}

	void ExpenseObject::setAmount(const double& amount)
	{
		_amount = amount;
	}

	void ExpenseObject::setDescription(const MAUtil::String& description)
	{
		delete _description;
		_description = new MAUtil::String(description);
	}

	void ExpenseObject::setImagePath(const MAUtil::String& path)
	{
		delete _imagePath;
		_imagePath = new MAUtil::String(path);
	}

	void ExpenseObject::setDate(const DateStruct& date)
	{
		_date = date;
	}

	const DateStruct& ExpenseObject::getDate() const
	{
		return _date;
	}
	void ExpenseObject::setTime(const TimeStruct& time)
	{
		_time = time;
	}
	const TimeStruct& ExpenseObject::getTime() const
	{
		return _time;
	}

	ExpenseObject& ExpenseObject::operator=(const ExpenseObject& obj)
	{
		_amount = obj._amount;
		delete _description;
		delete _category;
		delete _imagePath;

		_description = new MAUtil::String(*(obj._description));
		_category = new MAUtil::String(*(obj._category));
		_imagePath =new MAUtil::String(*(obj._imagePath));

		_date._day = obj._date._day;
		_date._year = obj._date._year;
		_date._mounth = obj._date._mounth;
		_time._hour = obj._time._hour;
		_time._minutes = obj._time._minutes;

		return *this;
	}
}
