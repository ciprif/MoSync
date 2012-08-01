/*
 * observer.h
 *
 *  Created on: Jun 21, 2012
 *      Author: Cipri
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

#include <MAUtil/String.h>
#include <MAUtil/Vector.h>


namespace Model
{
	class ExpenseObject;
	class IncomeObject;
	struct TimeStruct;
	struct DateStruct;
}

namespace Repositories
{
	class DBManager;
}

namespace GUI
{
	class HomeScreen;
	class ListScreen;
	class SettingsScreen;
}
namespace Logical
{
	class SettingsManager;

	class Observer
	{
	public:
		Observer();
		~Observer();
		void requestExpenseAddition(const double& amount, const MAUtil::String& category, const MAUtil::String& description,
								   const MAUtil::String& imgPath, const Model::DateStruct& date, const Model::TimeStruct& time);
		void requestIncomeAddition(const double& amount, const MAUtil::String& type, const MAUtil::String& description,
								  const MAUtil::String& transactionInfo, const Model::DateStruct& date, const Model::TimeStruct& time);
		void updateScreenNotification(const double& value, bool isExpense, const MAUtil::String& category);
		void updateExpensesListNotification(const Model::ExpenseObject& obj);
		void updateIncomeListNotification(const Model::IncomeObject& obj);
		void updateTotalBudgetNotification();
		void updateConsumedBudgetNotification();
		void updateDebtBudgetNotification();
		void setHomeScreenRef(GUI::HomeScreen* homeScreen);
		void setListScreenRef(GUI::ListScreen* listScreen);
		void setSettingsScreenRef(GUI::SettingsScreen* settingsScreen);

		MAUtil::Vector<Model::IncomeObject*>* incomesListRequest();
		MAUtil::Vector<Model::ExpenseObject*>* expensesListRequest();

		double requestTotalBudget() const;
		double requestConsumedBudget() const;
		double debtBudgetReqeust() const;
		MAUtil::String& requestCoin() const;
		bool requestIsShowAll() const;
		bool requestIsMonthly() const;
		bool requestIsShowFromDate() const;
		Model::DateStruct& requestFromDate();
		double& requestDebtValue();

		double requestCategoryAmount(const MAUtil::String& category);
		void requestSaveSettings(bool isShowAll, bool isMonthly, bool isFromDate, const double& debtValue, const Model::DateStruct& date, const MAUtil::String& coin);
	private:
		void applySettings();
		GUI::HomeScreen* _homeScreenRefference;
		GUI::ListScreen* _listScreebRefference;
		GUI::SettingsScreen* _settingsScreenReference;

		Repositories::DBManager* _DBManager;
		SettingsManager* _settingsManager;

		double _totalBudget;
		double _consumedBudget;
		double _debtBudget;
	};
}

#endif /* OBSERVER_H_ */
