/*
 * listItemModel.cpp
 *
 *  Created on: Jul 4, 2012
 *      Author: Cipri
 */

#include "listItemModel.h"
#include "expenseObject.h"
#include "incomeObject.h"
#include <conprint.h>

namespace Model
{
	ListItemModel::ListItemModel() : income(NULL), expense(NULL)
	{
		isExpense = false;
		isIncome = false;
	}

	ListItemModel::ListItemModel(const ListItemModel& obj)
	{
		isExpense = obj.isExpense;
		isIncome = obj.isIncome;
		if(isExpense)
			expense = new ExpenseObject(*obj.expense);
		if(isIncome)
			income = new IncomeObject(*obj.income);
	}

	ListItemModel::~ListItemModel()
	{
		if(NULL != expense) delete expense;
		if(NULL != income) delete income;
	}

	void ListItemModel::setExpense(const ExpenseObject& obj)
	{
		if(!isIncome)
		{
			if(NULL == expense) expense = new ExpenseObject();

			*expense = obj;
			isExpense = true;
		}
	}

	void ListItemModel::setIncome(const IncomeObject& obj)
	{
		if(!isExpense)
		{
			if(NULL == income) income = new IncomeObject();

			*income = obj;
			isIncome = true;
		}
	}

	ListItemModel& ListItemModel::operator=(const ListItemModel& obj)
	{
		delete expense;
		delete income;

		if(obj.isExpense)
		{
			expense = new ExpenseObject(obj.getExpenseObject());
			isExpense = true;
			isIncome = false;
			income = NULL;
		}
		else if(obj.isIncome)
		{
			income = new IncomeObject(obj.getIncomeObject());
			isExpense = false;
			isIncome = true;
			expense = NULL;
		}

		return *this;
	}

	bool ListItemModel::IsExpense() const { return isExpense; }
	bool ListItemModel::IsIncome() const { return isIncome; }

	const ExpenseObject& ListItemModel::getExpenseObject() const { return *expense; }
	const IncomeObject& ListItemModel::getIncomeObject() const { return *income; }
}
