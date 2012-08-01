/*
 *	incomeObject.cpp
 *
 *  Created on: Jun 13, 2012
 *      Author: Cipri
 */

#include "incomeObject.h"
#include "util.h"

namespace Model
{
	IncomeObject::IncomeObject()
	{
		_amount = 0.0;
		_description = new MAUtil::String("");
		_incomeType = new MAUtil::String("Other");
		_transactionInformation = new MAUtil::String("");
	}

	IncomeObject::IncomeObject(const double& amount, const MAUtil::String& type, const MAUtil::String& description,
			 	 	 	 	  const MAUtil::String& transactionInformation, const int& year, const int& month, const int& day, const int& hour, const int& minute)
	{
		_amount = amount;
		_description = new MAUtil::String(description);
		_incomeType = new MAUtil::String(type);
		_transactionInformation = new MAUtil::String(transactionInformation);
		_date._year = year;
		_date._mounth = month;
		_date._day = day;
		_time._hour = hour;
		_time._minutes = minute;
	}

	IncomeObject::IncomeObject(const IncomeObject& obj)
	{
		_amount = obj._amount;
		_incomeType = new MAUtil::String(*(obj._incomeType));
		_transactionInformation = new MAUtil::String(*(obj._transactionInformation));
		_description =new MAUtil::String(*(obj._description));
		_date._day = obj._date._day;
		_date._year = obj._date._year;
		_date._mounth = obj._date._mounth;
		_time._hour = obj._time._hour;
		_time._minutes = obj._time._minutes;
	}

	IncomeObject::~IncomeObject()
	{
		delete _description;
		delete _transactionInformation;
		delete _incomeType;
	}

	const MAUtil::String& IncomeObject::getType() const
	{
		return *_incomeType;
	}

	const double& IncomeObject::getAmount() const
	{
		return _amount;
	}

	const MAUtil::String& IncomeObject::getDescription() const
	{
		return *_description;
	}

	const MAUtil::String& IncomeObject::getTransactionInformation() const
	{
		return *_transactionInformation;
	}

	void IncomeObject::setType(const MAUtil::String& type)
	{
		*_incomeType = type;
	}

	void IncomeObject::setAmount(const double& amount)
	{
		_amount = amount;
	}

	void IncomeObject::setDescription(const MAUtil::String& description)
	{
		*_description = description;
	}

	void IncomeObject::setTransactionInformation(const MAUtil::String& transactionInfo)
	{
		delete _transactionInformation;
		_transactionInformation = new MAUtil::String(transactionInfo);
	}

	void IncomeObject::setDate(const DateStruct& date)
	{
		_date = date;
	}

	const DateStruct& IncomeObject::getDate() const
	{
		return _date;
	}
	void IncomeObject::setTime(const TimeStruct& time)
	{
		_time = time;
	}
	const TimeStruct& IncomeObject::getTime() const
	{
		return _time;
	}

	IncomeObject& IncomeObject::operator=(const IncomeObject& obj)
	{
		_amount = obj._amount;

		delete _incomeType;
		delete _transactionInformation;
		delete _description;

		_incomeType = new MAUtil::String(*(obj._incomeType));
		_transactionInformation = new MAUtil::String(*(obj._transactionInformation));
		_description =new MAUtil::String(*(obj._description));

		_date._day = obj._date._day;
		_date._year = obj._date._year;
		_date._mounth = obj._date._mounth;
		_time._hour = obj._time._hour;
		_time._minutes = obj._time._minutes;

		return *this;
	}
}



