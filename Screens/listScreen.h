/*
 * listScreen.h
 *
 *  Created on: Jun 18, 2012
 *      Author: Cipri
 */

#ifndef LISTSCREEN_H_
#define LISTSCREEN_H_

#include <NativeUI/Screen.h>
#include <NativeUI/ScreenListener.h>
#include <NativeUI/ListViewListener.h>
#include <NativeUI/Label.h>
#include <MAUtil/String.h>
#include <MAUtil/Map.h>
#include "../Logical/observer.h"
#include "../Model/util.h"

namespace Model
{
	class ExpenseObject;
	class IncomeObject;
	class ListItemModel;
}

namespace NativeUI
{
	class HorizontalLayout;
	class VerticalLayout;
	class ListView;
	class ListViewItem;
}

namespace GUI
{
	class AddExpenseDialog;
	class AddIncomeDialog;

	class ListScreen : public NativeUI::Screen, public NativeUI::ListViewListener, public NativeUI::ScreenListener
	{
	public:
		ListScreen();

		void populateExpensesList();
		void populateIncomesList();
		void sortListByDate(bool descending);
		void sortListByType(bool incomesFirst);
		void sortListByAmount(bool descending);

		void setCoin(const MAUtil::String& coin);

		void setObserver(Logical::Observer* obs);

		/**
		 * This method is called when a list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param listViewItem The ListViewItem object that was clicked.
		 */
		void listViewItemClicked(
			NativeUI::ListView* listView,
			NativeUI::ListViewItem* listViewItem){}

		/**
		 * This method is called when a list view item is clicked.
		 * @param listView The list view object that generated the event.
		 * @param index The index on which the list view item is positioned.
		 */
		void listViewItemClicked(
			NativeUI::ListView* listView,
			int index);

		void addExpenseNotification(const Model::ExpenseObject& obj);

		void addIncomeNotification(const Model::IncomeObject& obj);

		void createOptionsMenu();

		void optionsMenuItemSelected(NativeUI::Screen* screen, int index);
		void optionsMenuClosed(NativeUI::Screen* screen);

		void setAddExpensesDialogReference(AddExpenseDialog* expensesDialog);
		void setAddIncomeDialogReference(AddIncomeDialog* incomesDialog);

		void setDateFrom(const Model::DateStruct& dateFrom);

		void updateDebtValue();
	private:
		void _createUI();
		/**
		 *
		 * @param criteria a pointer to a function used for sorting elements
		 * @param left
		 * @param right
		 * @param value the value returned by the criteria function when the first param is "<" then the second one
		 */
		void _sortList(int (*criteria)(const Model::ListItemModel&, const Model::ListItemModel&), int left, int right, int value);
		void _clearAndRepopulateList();

		NativeUI::VerticalLayout* _createListItem(const Model::ListItemModel& obj, int index);

		MAUtil::Vector<Model::ListItemModel>* _itemsMap;
		MAUtil::Vector<NativeUI::Label*>* _detailsMap;
		NativeUI::HorizontalLayout* _mainLayout;
		NativeUI::ListView* _listView;

		MAUtil::String _coin;

		Logical::Observer* _observerReference;
		int _itemWidth;

		int _addIncomeIndex;
		int _addExpenseIndex;
		int _sortByDateIndex;
		int _sortByCategoryIndex;
		int _sortByAmountIndex;

		AddExpenseDialog* _addExpensesDialog;
		AddIncomeDialog* _addIncomeDialog;

		double _budgetTotalValue;
		double _budgetConsumedValue;
		double _debtBudget;

		Model::DateStruct* _startFromDate;

		int _countClicksDates;
		int _countClicksAmount;
		int _countClicksCategory;

		//int (*criteria)(const Model::ListItemModel&, const Model::ListItemModel&);
		MAUtil::Vector<int> _characteristicArrayForSorting;
	};
}


#endif /* LISTSCREEN_H_ */
