/*
 * expenseObject.h
 *
 *  Created on: Jun 21, 2012
 *      Author: Cipri
 */

#ifndef EXPENSEOBJECT_H_
#define EXPENSEOBJECT_H_

#include <MAUtil/String.h>
#include "util.h"

namespace Model
{
	class ExpenseObject
	{
	public:
		ExpenseObject();
		ExpenseObject(const double& amount, const MAUtil::String& category, const MAUtil::String& description, const MAUtil::String& imgPath,
					  const int& year, const int& month, const int& day, const int& hour, const int& minute);
		ExpenseObject(const ExpenseObject&);
		~ExpenseObject();
		const MAUtil::String& getCategory() const;
		const double& getAmount() const;
		const MAUtil::String& getDescription() const;
		const MAUtil::String& getImagePath() const; //to be discussed
		const DateStruct& getDate() const;
		const TimeStruct& getTime() const;
		void setCategory(const MAUtil::String& category);
		void setAmount(const double& amount);
		void setDescription(const MAUtil::String& description);
		void setImagePath(const MAUtil::String& path);
		void setDate(const DateStruct& date);
		void setTime(const TimeStruct& time);
		ExpenseObject& operator=(const ExpenseObject& obj);
	private:
		MAUtil::String* _category;
		double _amount;
		MAUtil::String* _description;
		MAUtil::String* _imagePath;
		DateStruct _date;
		TimeStruct _time;
	};
}
#endif /* EXPENSEOBJECT_H_ */
