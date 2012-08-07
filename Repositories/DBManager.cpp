/*
 * DBManager.cpp
 *
 *  Created on: Jun 26, 2012
 *      Author: Cipri
 */

#include <maapi.h>
#include <MAUtil/String.h>
#include <MAUtil/util.h>
#include <MAFS/File.h>
#include <conprint.h>
#include "DBManager.h"

namespace Repositories
{
	DBManager::DBManager()
	{
		_DBExpenses = maFileOpen(EXPENSES_FILE, MA_ACCESS_READ_WRITE);
		_DBIncomes = maFileOpen(INCOMES_FILE, MA_ACCESS_READ_WRITE);
		if(!maFileExists(_DBExpenses))
		{
			maFileCreate(_DBExpenses);
		}
		if(!maFileExists(_DBIncomes))
		{
			maFileCreate(_DBIncomes);
		}
		maFileClose(_DBExpenses);
		maFileClose(_DBIncomes);
		_expenses = new MAUtil::Vector<Model::ExpenseObject*>();
		_incomes = new MAUtil::Vector<Model::IncomeObject*>();

		_totalBudget = 0.0;
		_consumedBudget = 0.0;
	}

	DBManager::~DBManager()
	{
		for(int i = 0; i <= _expenses->size(); i++)
			delete (*_expenses)[i];
		for(int i = 0; i <= _incomes->size(); i++)
					delete (*_incomes)[i];
		delete _expenses;
		delete _incomes;
		delete _expensesFileContent;
		delete _incomesFileContent;
	}

	void DBManager::readExpenses()
	{
		_DBExpenses = maFileOpen(EXPENSES_FILE, MA_ACCESS_READ);

		if(NULL != _expensesFileContent)
			delete _expensesFileContent;
		_expensesFileContent = new MAUtil::String("\0", 5*BUFF_SIZE);

		char buffer[BUFF_SIZE];
		bool reading = true;
		while(reading)
		{
			if(0 <= maFileRead(_DBExpenses, &buffer, BUFF_SIZE) && strlen(buffer) > 0)
			{
				_expensesFileContent->append(buffer, strlen(buffer));
				memset(&buffer, 0, BUFF_SIZE);
			}
			else reading = false;
		}
		maFileClose(_DBExpenses);

		int offset = 0;
		int position = _expensesFileContent->find("|", offset);
		while(-1 != position)
		{
			MAUtil::String expenseString = _expensesFileContent->substr(offset, position - offset);

			int inner_offset = 0;
			int inner_position = expenseString.find("~", inner_offset);

			MAUtil::Vector<MAUtil::String> tokens;

			while(-1 != inner_position)
			{
				//getting each token of an expense string
				tokens.add(expenseString.substr(inner_offset, inner_position - inner_offset));
				inner_offset = inner_position + 1;
				inner_position = expenseString.findFirstOf('~', inner_offset);
			}
			tokens.add(expenseString.substr(inner_offset, expenseString.length() - inner_offset - 1));

			inner_offset = 0;
			inner_position = tokens[4].find("-", inner_offset);
			//extracting date and time from the last 2 tokens
			int count = 0;
			int year, month, day;
			while(-1 != inner_position)
			{
				int val = 0;
				val = MAUtil::stringToInteger(tokens[4].substr(inner_offset, inner_position - inner_offset), 10);
				inner_offset = inner_position + 1;
				inner_position = tokens[4].findFirstOf('-', inner_offset);
				switch(count)
				{
				case 0:
					year = val;
					break;
				case 1:
					month = val;
					break;
				}
				count++;
			}
			day = MAUtil::stringToInteger(tokens[4].substr(inner_offset, tokens[4].length() - inner_offset), 10);

			inner_offset = 0;
			inner_position = tokens[5].find("-", inner_offset);

			int hour = MAUtil::stringToInteger(tokens[5].substr(inner_offset, inner_position), 10);
			inner_offset = inner_position + 1;
			inner_position = tokens[5].find("-", inner_offset);
			int minute = MAUtil::stringToInteger(tokens[5].substr(inner_offset, tokens[5].length() - inner_offset), 10);
			addExpenseToList(MAUtil::stringToDouble(tokens[0]), tokens[1], tokens[2], tokens[3], year, month, day, hour, minute);

			offset = position + 1;
			position = _expensesFileContent->find("|", offset);
		}

		delete _expensesFileContent;
		_expensesFileContent = NULL;
	}

	void DBManager::readIncomes()
	{
		_DBIncomes = maFileOpen(INCOMES_FILE, MA_ACCESS_READ);

		if(NULL != _incomesFileContent)
			delete _incomesFileContent;
		_incomesFileContent = new MAUtil::String("\0", 5*BUFF_SIZE);

		char buffer[BUFF_SIZE];
		bool reading = true;
		while(reading)
		{
			if(0 <= maFileRead(_DBIncomes, &buffer, BUFF_SIZE) && strlen(buffer) > 0)
			{
				_incomesFileContent->append(buffer, strlen(buffer));
				memset(&buffer, 0, BUFF_SIZE);
			}
			else reading = false;
		}

		maFileClose(_DBIncomes);

		int offset = 0;
		int position = _incomesFileContent->find("|", offset);
		while(-1 != position)
		{
			MAUtil::String incomeString = _incomesFileContent->substr(offset, position - offset);

			int inner_offset = 0;
			int inner_position = incomeString.find("~", inner_offset);

			MAUtil::Vector<MAUtil::String> tokens;

			while(-1 != inner_position)
			{
				//getting each token of an expense string
				tokens.add(incomeString.substr(inner_offset, inner_position - inner_offset));
				inner_offset = inner_position + 1;
				inner_position = incomeString.findFirstOf('~', inner_offset);
			}
			tokens.add(incomeString.substr(inner_offset, incomeString.length() - inner_offset));

			inner_offset = 0;
			inner_position = tokens[4].find("-", inner_offset);

			//extracting date and time from the last 2 tokens
			int count = 0;
			int year, month, day;
			while(-1 != inner_position)
			{
				int val = 0;
				val = MAUtil::stringToInteger(tokens[4].substr(inner_offset, inner_position - inner_offset), 10);
				inner_offset = inner_position + 1;
				inner_position = tokens[4].findFirstOf('-', inner_offset);
				switch(count)
				{
				case 0:
					year = val;
					break;
				case 1:
					month = val;
					break;
				}
				count++;
			}
			day = MAUtil::stringToInteger(tokens[4].substr(inner_offset, tokens[4].length() - inner_offset), 10);

			inner_offset = 0;
			inner_position = tokens[5].find("-", inner_offset);

			int hour = MAUtil::stringToInteger(tokens[5].substr(inner_offset, inner_position), 10);
			inner_offset = inner_position + 1;
			inner_position = tokens[5].find("-", inner_offset);
			int minute = MAUtil::stringToInteger(tokens[5].substr(inner_offset, tokens[5].length() - inner_offset), 10);

			addIncomeToList(MAUtil::stringToDouble(tokens[0]), tokens[1], tokens[2], tokens[3], year, month, day, hour, minute);

			offset = position + 1;
			position = _incomesFileContent->find("|", offset);
		}

		delete _incomesFileContent;
		_incomesFileContent = NULL;
	}

	int DBManager::getNumberOfExpenses() const
	{
		return _expenses->size();
	}

	Model::ExpenseObject& DBManager::getExpenseObject(const int& index) const
	{
		if(index < _expenses->size())
		{
			return *((*_expenses)[index]);
		}
	}

	void DBManager::addExpense(Model::ExpenseObject* obj)
	{
		MAUtil::String expenseContent;
		expenseContent.append(MAUtil::doubleToString(obj->getAmount()).c_str(), MAUtil::doubleToString(obj->getAmount()).length());
		expenseContent.append("~", 1);
		expenseContent.append(obj->getCategory().c_str(), obj->getCategory().length());
		expenseContent.append("~", 1);
		expenseContent.append(obj->getDescription().c_str(), obj->getDescription().length());
		expenseContent.append("~", 1);
		expenseContent.append(obj->getImagePath().c_str(), obj->getImagePath().length());
		expenseContent.append("~", 1);
		MAUtil::String date = Model::DateStructToString(obj->getDate());
		expenseContent.append(date.c_str(), date.length());
		expenseContent.append("~", 1);
		MAUtil::String time = Model::TimeStructToString(obj->getTime());
		expenseContent.append(time.c_str(), time.length());
		expenseContent.append("|", 1);

		_DBExpenses = maFileOpen(EXPENSES_FILE, MA_ACCESS_READ_WRITE);
		maFileSeek(_DBExpenses, 0, SEEK_END);
		maFileWrite(_DBExpenses, expenseContent.c_str(), expenseContent.length());
		maFileClose(_DBExpenses);

		_expenses->add(obj);
		_consumedBudget += (obj->getAmount());
	}

	void DBManager::addIncome(Model::IncomeObject* obj)
	{
		MAUtil::String incomeContent;
		incomeContent.append(MAUtil::doubleToString(obj->getAmount()).c_str(), MAUtil::doubleToString(obj->getAmount()).length());
		incomeContent.append("~", 1);
		incomeContent.append(obj->getType().c_str(), obj->getType().length());
		incomeContent.append("~", 1);
		incomeContent.append(obj->getDescription().c_str(), obj->getDescription().length());
		incomeContent.append("~", 1);
		incomeContent.append(obj->getTransactionInformation().c_str(), obj->getTransactionInformation().length());
		incomeContent.append("~", 1);
		MAUtil::String date = Model::DateStructToString(obj->getDate());
		incomeContent.append(date.c_str(), date.length());
		incomeContent.append("~", 1);
		MAUtil::String time = Model::TimeStructToString(obj->getTime());
		incomeContent.append(time.c_str(), time.length());
		incomeContent.append("|", 1);

		_DBIncomes = maFileOpen(INCOMES_FILE, MA_ACCESS_READ_WRITE);
		maFileSeek(_DBIncomes, 0, SEEK_END);
		maFileWrite(_DBIncomes, incomeContent.c_str(), incomeContent.length());
		maFileClose(_DBIncomes);

		_incomes->add(obj);
		_totalBudget += (obj->getAmount());
	}

	int DBManager::getNumberOfIncomes() const
	{
		return _incomes->size();
	}

	Model::IncomeObject& DBManager::getIncomeObject(const int& index) const
	{
		if(index < _incomes->size())
		{
			return (*(*_incomes)[index]);
		}
	}

	void DBManager::addExpenseToList(const double& amount, const MAUtil::String& category, const MAUtil::String& description, const MAUtil::String& imgPath,
								  const int& year, const int& month, const int& day, const int& hour, const int& minute)
	{
		Model::ExpenseObject* obj = new Model::ExpenseObject(amount, category, description, imgPath, year, month, day, hour, minute);
		_expenses->add(obj);
	}
	void DBManager::addIncomeToList(const double& amount, const MAUtil::String& type, const MAUtil::String& description, const MAUtil::String& transactionInfo,
								  const int& year, const int& month, const int& day, const int& hour, const int& minute)
	{
		Model::IncomeObject* obj = new Model::IncomeObject(amount, type, description, transactionInfo, year, month, day, hour, minute);
		_incomes->add(obj);
	}

	double DBManager::getTotalBudget()
	{
		_totalBudget = 0.0;
		for(int i = 0; i < _incomes->size(); i++)
		{
			if(Model::CompareDateObjects(_date, (*_incomes)[i]->getDate()) == -1)
			{
				_totalBudget += (*_incomes)[i]->getAmount();
			}
		}
		return _totalBudget;
	}

	double DBManager::getConsumedBudget()
	{
		_consumedBudget = 0.0;
		for(int i = 0; i < _expenses->size(); i++)
		{
			if(Model::CompareDateObjects(_date, (*_expenses)[i]->getDate()) == -1 ||
			   Model::CompareDateObjects(_date, (*_expenses)[i]->getDate()) == 0)
			{
				_consumedBudget += (*_expenses)[i]->getAmount();
			}
		}
		return _consumedBudget;
	}

	double DBManager::getCategoryAmount(const MAUtil::String& category)
	{
		double value = 0.0;
		for(int i = 0; i < _expenses->size(); i++)
		{
			if(strcmp((*_expenses)[i]->getCategory().c_str(), category.c_str()) == 0 &&
			   (Model::CompareDateObjects(_date, (*_expenses)[i]->getDate()) == -1 ||
			    Model::CompareDateObjects(_date, (*_expenses)[i]->getDate()) == 0))
			{
				value += (*_expenses)[i]->getAmount();
			}
		}
		return value;
	}

	void DBManager::setDate(const Model::DateStruct& d)
	{
		_date._day = d._day;
		_date._mounth = d._mounth;
		_date._year = d._year;
	}

	MAUtil::Vector<Model::ExpenseObject*>* DBManager::getExpenses()
	{
		return _expenses;
	}
	MAUtil::Vector<Model::IncomeObject*>* DBManager::getIncomes()
	{
		return _incomes;
	}

	void DBManager::clearFiles()
	{
		_DBIncomes = maFileOpen(INCOMES_FILE, MA_ACCESS_READ_WRITE);
		maFileTruncate(_DBIncomes, 0);
		maFileClose(_DBIncomes);

		_DBExpenses = maFileOpen(EXPENSES_FILE, MA_ACCESS_READ_WRITE);
		maFileTruncate(_DBExpenses, 0);
		maFileClose(_DBExpenses);

		_incomes->clear();
		_expenses->clear();
	}
}
