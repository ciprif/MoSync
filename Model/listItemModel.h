/*
 * listItemModel.h
 *
 *  Created on: Jul 4, 2012
 *      Author: Cipri
 */

#ifndef LISTITEMMODEL_H_
#define LISTITEMMODEL_H_

#include "expenseObject.h"
#include "incomeObject.h"
#include <MAUtil/util.h>

namespace Model
{
	class ListItemModel
	{
	public:
		ListItemModel();
		ListItemModel(const ListItemModel&);
		~ListItemModel();
		void setExpense(const ExpenseObject& obj);

		void setIncome(const IncomeObject& obj);

		bool IsExpense() const;
		bool IsIncome() const;

		const ExpenseObject& getExpenseObject() const;
		const IncomeObject& getIncomeObject() const;

		ListItemModel& operator=(const ListItemModel& obj);
	private:
		ExpenseObject* expense;
		IncomeObject* income;
		bool isExpense;
		bool isIncome;
	};
}


#endif /* LISTITEMMODEL_H_ */
