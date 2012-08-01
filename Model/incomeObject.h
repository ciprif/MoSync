/*
 * incomeObject.h
 *
 *  Created on: Jun 13, 2012
 *      Author: Cipri
 */

#ifndef INCOMEOBJECT_H_
#define INCOMEOBJECT_H_

#include <MAUtil/String.h>
#include "util.h"

namespace Model
{
	class IncomeObject
	{
	public:
		IncomeObject();
		IncomeObject(const double& amount, const MAUtil::String& type, const MAUtil::String& description,
					 const MAUtil::String& transactionInformation, const int& year, const int& month, const int& day, const int& hour, const int& minute);
		IncomeObject(const IncomeObject&);
		~IncomeObject();
		const MAUtil::String& getType() const;
		const double& getAmount() const;
		const MAUtil::String& getDescription() const;
		const MAUtil::String& getTransactionInformation() const;
		const DateStruct& getDate() const;
		const TimeStruct& getTime() const;
		void setType(const MAUtil::String& type);
		void setAmount(const double& amount);
		void setDescription(const MAUtil::String& description);
		void setTransactionInformation(const MAUtil::String& path);
		void setDate(const DateStruct& date);
		void setTime(const TimeStruct& time);
		IncomeObject& operator=(const IncomeObject& obj);
	private:
		MAUtil::String* _description;
		MAUtil::String* _incomeType;
		MAUtil::String* _transactionInformation;
		double _amount;
		DateStruct _date;
		TimeStruct _time;
	};
}


#endif /* INCOMEOBJECT_H_ */
