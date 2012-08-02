/*
 * DBManager.h
 *
 *  Created on: Jun 26, 2012
 *      Author: Cipri
 */

#ifndef DBMANAGER_H_
#define DBMANAGER_H_

#include <MAUtil/Vector.h>
#include <ma.h>
#include <MAUtil/String.h>

#include "../Model/expenseObject.h"
#include "../Model/incomeObject.h"

#define EXPENSES_FILE "expensesDB.txt"
#define INCOMES_FILE "incomesDB.txt"

#define BUFF_SIZE 256

namespace Repositories
{
	class DBManager
	{
	public:
		DBManager();
		~DBManager();

		void readExpenses();
		void readIncomes();
		int getNumberOfExpenses() const;
		int getNumberOfIncomes() const;
		Model::ExpenseObject& getExpenseObject(const int& index) const;
		Model::IncomeObject& getIncomeObject(const int& index) const;
		void addExpense(Model::ExpenseObject* obj);
		void addIncome(Model::IncomeObject* obj);
		double getTotalBudget();
		double getConsumedBudget();
		double getCategoryAmount(const MAUtil::String& category);
		MAUtil::Vector<Model::ExpenseObject*>* getExpenses();
		MAUtil::Vector<Model::IncomeObject*>* getIncomes();

		void setDate(const Model::DateStruct& d);
	private:
		void addExpenseToList(const double& amount, const MAUtil::String& category, const MAUtil::String& description, const MAUtil::String& imgPath,
							  const int& year, const int& month, const int& day, const int& hour, const int& minute);
		void addIncomeToList(const double& amount, const MAUtil::String& type, const MAUtil::String& description, const MAUtil::String& transactionInfo,
									  const int& year, const int& month, const int& day, const int& hour, const int& minute);
		MAHandle _DBExpenses;
		MAHandle _DBIncomes;
		MAUtil::Vector<Model::ExpenseObject*>* _expenses;
		MAUtil::Vector<Model::IncomeObject*>* _incomes;
		MAUtil::String* _expensesFileContent;
		MAUtil::String* _incomesFileContent;
		double _totalBudget;
		double _consumedBudget;

		Model::DateStruct _date;
	};
}

#endif /* DBMANAGER_H_ */
