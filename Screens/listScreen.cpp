/*
 * expensesListScreen.cpp
 *
 *  Created on: Jun 18, 2012
 *      Author: Cipri
 */

#include "listScreen.h"
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/VerticalLayout.h>
#include <MAUtil/Vector.h>

#include <NativeUI/ListView.h>
#include "../Model/expenseObject.h"
#include "../Model/incomeObject.h"
#include "../Model/listItemModel.h"
#include "addExpenseDialog.h"
#include "addIncomeDialog.h"

namespace GUI
{
	bool isFromRemove;

	int _sortByDateCriteria(const Model::ListItemModel&, const Model::ListItemModel&);
	int _sortByTypeCriteria(const Model::ListItemModel&, const Model::ListItemModel&);
	int _sortByAmountCriteria(const Model::ListItemModel&, const Model::ListItemModel&);

	ListScreen::ListScreen() : _listView(NULL)
	{
		isFromRemove = false;

		 MAUtil::Environment::getEnvironment().addCustomEventListener(this);

		_detailsMap = new MAUtil::Vector<NativeUI::Label*>();
		_itemsMap = new MAUtil::Vector<Model::ListItemModel>();
		_coin = "EUR";

		addScreenListener(this);

		MAExtent size = maGetScrSize();
		int screenWidth = EXTENT_X(size);
		int screenHeight = EXTENT_Y(size);
		_itemWidth = 9 * (screenWidth / 10) - screenWidth / 30;
		_startFromDate = new Model::DateStruct();

		//default values
		_startFromDate->_day = 1;
		_startFromDate->_mounth = 1;
		_startFromDate->_year = 1900;

		_countClicksAmount = 0;
		_countClicksCategory = 0;
		_countClicksDates = 0;

		_createUI();
	}

	void ListScreen::populateExpensesList()
	{
		MAUtil::Vector<Model::ExpenseObject*>* expList = _observerReference->expensesListRequest();

		Model::ListItemModel* obj;

		for(int i = 0; i < expList->size(); i++)
		{
			if(Model::CompareDateObjects(*_startFromDate, (*expList)[i]->getDate()) == -1 || Model::CompareDateObjects(*_startFromDate, (*expList)[i]->getDate()) == 0)
			{
				obj = new Model::ListItemModel();
				obj->setExpense(*(*expList)[i]);
				_listView->addChild(_createListItem(*obj, _listView->countChildWidgets()));
				_itemsMap->add(*obj);
				delete obj;
			}
		}
	}

	void ListScreen::populateIncomesList()
	{
		MAUtil::Vector<Model::IncomeObject*>* incList = _observerReference->incomesListRequest();

		Model::ListItemModel* obj;
		for(int i = 0; i < incList->size(); i++)
		{
			if(Model::CompareDateObjects(*_startFromDate, (*incList)[i]->getDate()) == -1 || Model::CompareDateObjects(*_startFromDate, (*incList)[i]->getDate()) == 0)
			{
				obj = new Model::ListItemModel();
				obj->setIncome(*(*incList)[i]);
				_listView->addChild(_createListItem(*obj, _listView->countChildWidgets()));
				_itemsMap->add(*obj);
				delete obj;
			}
		}
	}

	int _sortByDateCriteria(const Model::ListItemModel& a, const Model::ListItemModel& b)
	{
		Model::DateStruct d1, d2;
		if(a.IsExpense())
			d1 = a.getExpenseObject().getDate();
		else if(a.IsIncome())
			d1 = a.getIncomeObject().getDate();

		if(b.IsExpense())
			d2 = b.getExpenseObject().getDate();
		else if(b.IsIncome())
			d2 = b.getIncomeObject().getDate();

		return Model::CompareDateObjects(d1, d2);
	}

	int _sortByTypeCriteria(const Model::ListItemModel& a, const Model::ListItemModel& b)
	{
		if(a.IsExpense() && b.IsIncome()) return 1;
		else if(a.IsIncome() && b.IsExpense()) return -1;

		return 0;
	}

	int _sortByAmountCriteria(const Model::ListItemModel& a, const Model::ListItemModel& b)
	{
		double a_amount, b_amount;

		if(a.IsExpense()) a_amount = a.getExpenseObject().getAmount();
		else a_amount = a.getIncomeObject().getAmount();

		if(b.IsExpense()) b_amount = b.getExpenseObject().getAmount();
		else b_amount = b.getIncomeObject().getAmount();

		if((a_amount - b_amount) > 0) return 1;

		return -1;
	}

	void ListScreen::sortListByDate(bool descending)
	{
		if(descending) _sortList(_sortByDateCriteria, 0, _itemsMap->size() - 1, 1);
		else _sortList(_sortByDateCriteria, 0, _itemsMap->size() - 1, -1);

		for(int k = 0; k < _itemsMap->size(); k++)
		{
			Model::DateStruct d;
			if((*_itemsMap)[k].IsExpense())
			{
				d = (*_itemsMap)[k].getExpenseObject().getDate();
			}
			else d = (*_itemsMap)[k].getIncomeObject().getDate();
		}

		_clearAndRepopulateList();
	}

	void ListScreen::sortListByType(bool incomesFirst)
	{
		if(incomesFirst) _sortList(&_sortByTypeCriteria, 0, _itemsMap->size() - 1, 1);
		else _sortList(&_sortByTypeCriteria, 0, _itemsMap->size() - 1, -1);

		_clearAndRepopulateList();
	}

	void ListScreen::sortListByAmount(bool descending)
	{
		if(descending) _sortList(&_sortByAmountCriteria, 0, _itemsMap->size() - 1, 1);
		else _sortList(&_sortByAmountCriteria, 0, _itemsMap->size() - 1, -1);

		_clearAndRepopulateList();
	}

	void ListScreen::_clearAndRepopulateList()
	{
		_listView->removeListViewListener(this);

		delete _listView;

		_listView = new NativeUI::ListView();
		_listView->fillSpaceHorizontally();
		_listView->fillSpaceVertically();
		_listView->addListViewListener(this);

		_mainLayout->addChild(_listView);

		for(int i = 0; i < _detailsMap->size(); i++)
			delete (*_detailsMap)[i];

		delete _detailsMap;
		_detailsMap = new MAUtil::Vector<NativeUI::Label*>();

		for(int i = 0; i < _itemsMap->size(); i++)
		{
			_listView->addChild(_createListItem((*_itemsMap)[i], i));
		}
	}

	void ListScreen::clearList()
	{
		_listView->removeListViewListener(this);
		delete _listView;

		_listView = new NativeUI::ListView();
		_listView->fillSpaceHorizontally();
		_listView->fillSpaceVertically();
		_listView->addListViewListener(this);

		_mainLayout->addChild(_listView);
	}

	void ListScreen::_createUI()
	{
		_mainLayout = new NativeUI::HorizontalLayout();
		_listView = new NativeUI::ListView();
		_listView->fillSpaceHorizontally();
		_listView->fillSpaceVertically();
		_listView->addListViewListener(this);

		_mainLayout->addChild(_listView);

		setMainWidget(_mainLayout);

		createOptionsMenu();

		//populateList();
	}

	void ListScreen::addExpenseNotification(const Model::ExpenseObject& obj)
	{
		Model::ListItemModel list_obj;
		list_obj.setExpense(obj);

		_listView->addChild(_createListItem(list_obj, _listView->countChildWidgets()));
		_itemsMap->add(list_obj);
	}

	void ListScreen::addIncomeNotification(const Model::IncomeObject& obj)
	{
		Model::ListItemModel list_obj;
		list_obj.setIncome(obj);

		_listView->addChild(_createListItem(list_obj, _listView->countChildWidgets()));
		_itemsMap->add(list_obj);
	}

	NativeUI::VerticalLayout* ListScreen::_createListItem(const Model::ListItemModel& obj, int index)
	{
		NativeUI::VerticalLayout* itemParent = new NativeUI::VerticalLayout();
		itemParent->setWidth(_itemWidth);

		NativeUI::HorizontalLayout* visiblePart = new NativeUI::HorizontalLayout();
		NativeUI::Label* typeLabel = new NativeUI::Label();
		NativeUI::Label* sumLabel = new NativeUI::Label();

		NativeUI::Label* details = new NativeUI::Label();

		typeLabel->setFontSize(DIALOG_FONT_SIZE);
		sumLabel->setFontSize(DIALOG_FONT_SIZE);
		typeLabel->fillSpaceHorizontally();
		sumLabel->fillSpaceHorizontally();
		if(obj.IsExpense())
		{
			typeLabel->setText(obj.getExpenseObject().getCategory());
			typeLabel->setFontColor(RED);

			char amountString[10];
			sprintf(amountString, "%.2f", obj.getExpenseObject().getAmount());
			MAUtil::String aux(amountString);
			aux += " ";
			aux += _coin;
			sumLabel->setText(aux);
			aux.clear();
			aux += "Description:\n";
			aux += obj.getExpenseObject().getDescription();
			aux += "\nDate:\n";
			aux += Model::DateStructToString(obj.getExpenseObject().getDate());
			aux += "\nTime:\n";
			aux += Model::TimeStructToString(obj.getExpenseObject().getTime());
			details->setText(aux);
			details->setMaxNumberOfLines(10);
			details->setWidth(_itemWidth);
			details->setTextHorizontalAlignment(MAW_ALIGNMENT_LEFT);
		}
		else
		{
			typeLabel->setText(obj.getIncomeObject().getType());
			typeLabel->setFontColor(GREEN);

			char amountString[10];
			sprintf(amountString, "%.2f", obj.getIncomeObject().getAmount());

			MAUtil::String aux(amountString);
			aux += " ";
			aux += _coin;

			sumLabel->setText(aux);

			aux.clear();
			aux += "Description:\n";
			aux += obj.getIncomeObject().getDescription();
			aux += "\nTransaction info:\n";
			aux += obj.getIncomeObject().getTransactionInformation();
			aux += "\nDate:\n";
			aux += Model::DateStructToString(obj.getIncomeObject().getDate());
			aux += "\nTime:\n";
			aux += Model::TimeStructToString(obj.getIncomeObject().getTime());

			details->setText(aux);
			details->setMaxNumberOfLines(10);
			details->setWidth(_itemWidth);
			details->setTextHorizontalAlignment(MAW_ALIGNMENT_LEFT);
		}

		visiblePart->addChild(typeLabel);
		visiblePart->addChild(sumLabel);

		itemParent->addChild(visiblePart);

		_detailsMap->add(details);

		return itemParent;
	}

	void ListScreen::setCoin(const MAUtil::String& coin)
	{
		_coin = coin;
	}

	void ListScreen::setObserver(Logical::Observer* obs)
	{
		_observerReference = obs;
	}

	void ListScreen::createOptionsMenu()
	{
		_addExpenseIndex = addOptionsMenuItem("Expense", "addIncome.png", true);
		_addIncomeIndex = addOptionsMenuItem("Income", MAW_OPTIONS_MENU_ICON_CONSTANT_ADD, false);
		_clearListIndex = addOptionsMenuItem("Clear list", MAW_OPTIONS_MENU_ICON_CONSTANT_DELETE, false);

		_sortByDateIndex = addOptionsMenuItem("Sort by date");
		_sortByCategoryIndex = addOptionsMenuItem("Sort by category");
		_sortByAmountIndex = addOptionsMenuItem("Sort by amount");
	}

	void ListScreen::listViewItemClicked(NativeUI::ListView* listView, int index)
	{
		if(listView == _listView)
		{
			if(((NativeUI::VerticalLayout*)listView->getChild(index))->getChild(1) == NULL)
			{
				((NativeUI::VerticalLayout*)listView->getChild(index))->addChild((*_detailsMap)[index]);
				((NativeUI::VerticalLayout*)listView->getChild(index))->getChild(1)->setBackgroundColor(0x736F6E);
				((NativeUI::VerticalLayout*)listView->getChild(index))->getChild(0)->setBackgroundColor(0x413D3C);
			}
			else
			{
				((NativeUI::VerticalLayout*)listView->getChild(index))->removeChild((*_detailsMap)[index]);
				((NativeUI::VerticalLayout*)listView->getChild(index))->getChild(1)->setBackgroundColor(0);
				((NativeUI::VerticalLayout*)listView->getChild(index))->getChild(0)->setBackgroundColor(0);
			}
		}
	}

	void ListScreen::optionsMenuItemSelected(NativeUI::Screen* screen, int index)
	{
		if(screen == this)
		{
			if(_addExpenseIndex == index) //add expense screen
			{
				if(NULL != _addExpensesDialog)
				{
					_addExpensesDialog->setObserver(_observerReference);

					_budgetTotalValue =  _observerReference->requestTotalBudget();
					_budgetConsumedValue = _observerReference->requestConsumedBudget();

					_addExpensesDialog->setAvailableBudget(_budgetTotalValue - _budgetConsumedValue);
					/** @todo get this value from settings */
					_addExpensesDialog->setAcceptedDebtValue(_debtBudget);
					_addExpensesDialog->setCoin(_coin);
					_addExpensesDialog->setLaunchedFromHomeScreen(false);
					_addExpensesDialog->updateAmountSliderValue();
					_addExpensesDialog->show();
				}
			}
			else if(_addIncomeIndex == index)
			{
				if(NULL != _addIncomeDialog)
				{
					_addIncomeDialog->setCoin(_coin);
					_addIncomeDialog->setObserver(_observerReference);
					_addIncomeDialog->setLaunchedFromHomeScreen(false);
					_addIncomeDialog->show();
				}
			}
			else if(_sortByCategoryIndex == index)
			{
				if(_countClicksCategory % 2 == 0)
					sortListByType(true);
				else
					sortListByType(false);

				_countClicksCategory = (_countClicksCategory + 1) % 2;
				_countClicksAmount = 0;
				_countClicksDates = 0;
			}
			else if(_sortByDateIndex == index)
			{
				if(_countClicksDates % 2 == 0)
					sortListByDate(true);
				else
					sortListByDate(false);

				_countClicksDates = (_countClicksDates + 1) % 2;
				_countClicksAmount = 0;
				_countClicksCategory = 0;
			}
			else if(_sortByAmountIndex == index)
			{
				if(_countClicksAmount % 2 == 0)
					sortListByAmount(true);
				else
					sortListByAmount(false);

				_countClicksAmount = (_countClicksAmount + 1) % 2;
				_countClicksCategory = 0;
				_countClicksDates = 0;
			}
			else if(_clearListIndex == index)
			{
				maAlert("Alert!", "Are you sure you want to clear the list? Note that this action has a permanent effect.", "OK", "Cancel", NULL);
				isFromRemove = true;
			}
		}
	}

	void ListScreen::optionsMenuClosed(NativeUI::Screen* screen)
	{
		//Dance for me baby
	}

	void ListScreen::setAddExpensesDialogReference(AddExpenseDialog* expensesDialog)
	{
		_addExpensesDialog = expensesDialog;
		_addExpensesDialog->setListScreenRef(this);
	}
	void ListScreen::setAddIncomeDialogReference(AddIncomeDialog* incomesDialog)
	{
		_addIncomeDialog = incomesDialog;
		_addIncomeDialog->setListScreenRef(this);
	}

	void ListScreen::setDateFrom(const Model::DateStruct& dateFrom)
	{
		*_startFromDate = dateFrom;
	}

	void ListScreen::_sortList(int (*criteria)(const Model::ListItemModel&, const Model::ListItemModel&), int left, int right, int value)
	{
		for(int k = 0; k < _itemsMap->size(); k++)
		{
			Model::DateStruct d;
			if((*_itemsMap)[k].IsExpense())
			{
				d = (*_itemsMap)[k].getExpenseObject().getDate();
			}
			else d = (*_itemsMap)[k].getIncomeObject().getDate();
		}
		int i = left, j = right;
		Model::ListItemModel* pivot = new Model::ListItemModel((*_itemsMap)[(left + right) / 2]);
		Model::ListItemModel* aux;

		while(i <= j)
		{
			while( (*criteria)((*_itemsMap)[i], *pivot) == value)
			{
				i++;
			}
			while( (*criteria)(*pivot, (*_itemsMap)[j]) == value)
			{
				j--;
			}

			if(i <= j)
			{
				if(i != j)
				{
					aux = new Model::ListItemModel((*_itemsMap)[i]);
					(*_itemsMap)[i] = (*_itemsMap)[j];
					(*_itemsMap)[j] = *aux;

					delete aux;
				}
				i++;
				j--;
			}
		}
		delete pivot;
		if (left < j)
			_sortList(criteria, left, j, value);
		if (i < right)
			_sortList(criteria, i, right, value);
	}

	void ListScreen::updateDebtValue()
	{
		_debtBudget = _observerReference->requestDebtValue();
	}

	void ListScreen::customEvent(const MAEvent& event)
	{
		if(event.type == EVENT_TYPE_ALERT)
		{
			if(1 == event.alertButtonIndex && isFromRemove)
			{
				_observerReference->requestClearTransactionList();
				isFromRemove = false;
			}
		}
	}
}


