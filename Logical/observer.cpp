/*
 * observer.cpp
 *
 *  Created on: Jun 21, 2012
 *      Author: Cipri
 */

#include "../Screens/homeScreen.h"
#include "../Screens/listScreen.h"
#include "../Screens/settingsScreen.h"
#include "observer.h"
#include "../Repositories/DBManager.h"
#include "settingsManager.h"

namespace Logical
{
	Observer::Observer()
	{
		_DBManager = new Repositories::DBManager();
		_settingsManager = new Logical::SettingsManager();

		_DBManager->readExpenses();
		_DBManager->readIncomes();
		_totalBudget = _DBManager->getTotalBudget();
		_consumedBudget = _DBManager->getConsumedBudget();
	}

	Observer::~Observer()
	{
	}

	void Observer::setHomeScreenRef(GUI::HomeScreen* homeScreen)
	{
		_homeScreenReference = homeScreen;
		_homeScreenReference->setObserver(this);
	}

	void Observer::requestExpenseAddition(const double& amount, const MAUtil::String& category, const MAUtil::String& description,
										 const MAUtil::String& imgPath, const Model::DateStruct& date, const Model::TimeStruct& time)
	{
		Model::ExpenseObject* obj = new Model::ExpenseObject();
		obj->setAmount(amount);
		obj->setCategory(category);
		obj->setDescription(description);
		obj->setImagePath(imgPath);
		obj->setDate(date);
		obj->setTime(time);

		updateScreenNotification(obj->getAmount(), true, obj->getCategory());
		if(NULL != _listScreenReference)
		{
			_listScreenReference->addExpenseNotification(*obj);
		}
		_DBManager->addExpense(obj);
		_consumedBudget = _DBManager->getConsumedBudget();
	}

	void Observer::requestIncomeAddition(const double& amount, const MAUtil::String& type, const MAUtil::String& description,
									    const MAUtil::String& transactionInfo, const Model::DateStruct& date, const Model::TimeStruct& time)
	{
		Model::IncomeObject* obj = new Model::IncomeObject();
		obj->setAmount(amount);
		obj->setType(type);
		obj->setDescription(description);
		obj->setTransactionInformation(transactionInfo);
		obj->setDate(date);
		obj->setTime(time);

		updateScreenNotification(obj->getAmount(), false, "");
		if(NULL != _listScreenReference)
		{
			_listScreenReference->addIncomeNotification(*obj);
		}
		_DBManager->addIncome(obj);
		_totalBudget = _DBManager->getTotalBudget();
	}

	void Observer::updateScreenNotification(const double& value, bool isExpense, const MAUtil::String& category)
	{
		_homeScreenReference->updateBudgetValues(value, isExpense, category);
	}

	void Observer::updateExpensesListNotification(const Model::ExpenseObject& obj)
	{

	}

	void Observer::updateIncomeListNotification(const Model::IncomeObject& obj)
	{

	}

	MAUtil::Vector<Model::IncomeObject*>* Observer::incomesListRequest()
	{
		return _DBManager->getIncomes();
	}

	MAUtil::Vector<Model::ExpenseObject*>* Observer::expensesListRequest()
	{
		return _DBManager->getExpenses();
	}

	double Observer::requestTotalBudget() const
	{
		return _totalBudget;
	}
	double Observer::requestConsumedBudget() const
	{
		return _consumedBudget;
	}
	double Observer::debtBudgetReqeust() const
	{
		return 500;
	}

	void Observer::updateTotalBudgetNotification()
	{
		_homeScreenReference->updateTotalBudget(_totalBudget);
	}
	void Observer::updateConsumedBudgetNotification()
	{
		_homeScreenReference->updateConsumedBudget(_consumedBudget);
	}

	void Observer::updateDebtBudgetNotification()
	{
		_homeScreenReference->updateDebtBudget(_settingsManager->getDebtValue());
	}

	double Observer::requestCategoryAmount(const MAUtil::String& category)
	{
		return _DBManager->getCategoryAmount(category);
	}

	void Observer::setListScreenRef(GUI::ListScreen* listScreen)
	{
		_listScreenReference = listScreen;
		_listScreenReference->setObserver(this);
	}

	void Observer::setSettingsScreenRef(GUI::SettingsScreen* settingsScreen)
	{
		_settingsScreenReference = settingsScreen;
		_settingsScreenReference->setObserver(this);

		applySettings();
	}

	MAUtil::String& Observer::requestCoin() const
	{
		return _settingsManager->getCoin();
	}

	bool Observer::requestIsShowAll() const
	{
		return _settingsManager->isShowAll();
	}

	bool Observer::requestIsMonthly() const
	{
		return _settingsManager->isShowMonthly();
	}

	bool Observer::requestIsShowFromDate() const
	{
		return _settingsManager->isShowFromDate();
	}

	Model::DateStruct& Observer::requestFromDate()
	{
		return _settingsManager->getDate();
	}

	double& Observer::requestDebtValue()
	{
		return _settingsManager->getDebtValue();
	}

	void Observer::applySettings()
	{
		_DBManager->setDate(requestFromDate());

		if(NULL != _homeScreenReference)
		{
			updateDebtBudgetNotification();
			_homeScreenReference->setCoin(requestCoin());

			MAUtil::Vector<Model::IncomeObject*>* incomes =  _DBManager->getIncomes();
			MAUtil::Vector<Model::ExpenseObject*>* expenses =  _DBManager->getExpenses();

			_totalBudget = 0.0;
			_consumedBudget = 0.0;

			for(int i = 0; i < incomes->size(); i++)
			{
				if(Model::CompareDateObjects(requestFromDate(), (*incomes)[i]->getDate()) == -1 || Model::CompareDateObjects(requestFromDate(), (*incomes)[i]->getDate()) == 0)
				{
					_totalBudget += (*incomes)[i]->getAmount();
				}
			}

			for(int i = 0; i < expenses->size(); i++)
			{
				if(Model::CompareDateObjects(requestFromDate(), (*expenses)[i]->getDate()) == -1 || Model::CompareDateObjects(requestFromDate(), (*expenses)[i]->getDate()) == 0)
				{
					_consumedBudget += (*expenses)[i]->getAmount();
				}
			}

			_homeScreenReference->updateValues();
		}

		if(NULL != _listScreenReference)
		{
			_listScreenReference->setDateFrom(requestFromDate());
			_listScreenReference->updateDebtValue();
			_listScreenReference->setCoin(requestCoin());
			_listScreenReference->clearList();
			_listScreenReference->populateIncomesList();
			_listScreenReference->populateExpensesList();
		}
	}

	void Observer::requestSaveSettings(bool isShowAll, bool isMonthly, bool isFromDate, const double& debtValue, const Model::DateStruct& date, const MAUtil::String& coin)
	{
		_settingsManager->setCoin(coin);
		_settingsManager->setDateFrom(date);
		_settingsManager->setDebtValue(debtValue);
		_settingsManager->setIsShowAll(isShowAll);
		_settingsManager->setIsShowFromDate(isFromDate);
		_settingsManager->setIsShowMontly(isMonthly);

		_settingsManager->ApplySettings();

		applySettings();
	}

	void Observer::requestClearTransactionList()
	{
		_listScreenReference->clearList();
		_DBManager->clearFiles();

		_totalBudget = 0.0;
		_consumedBudget = 0.0;
		_homeScreenReference->updateValues();
	}
}


