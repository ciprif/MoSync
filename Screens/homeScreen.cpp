/*
 * homeScreen.cpp
 *
 *  Created on: Jun 12, 2012
 *      Author: Cipri
 */

#include <NativeUI/Label.h>
#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/Dialog.h>
#include <NativeUI/PanoramaView.h>
#include <NativeUI/ActivityIndicator.h>

#include "addExpenseDialog.h"
#include "addIncomeDialog.h"
#include "../Logical/observer.h"
#include "homeScreen.h"

namespace GUI
{
	HomeScreen::HomeScreen()
	{
		_budgetConsumedValue = 0;
		_budgetTotalValue = 0;
		_coin = "EUR";
		_debtBudget = 0;

		_categoryGraphicsMap = new MAUtil::Map<const MAUtil::String, NativeUI::HorizontalLayout*>();
		_valueMap = new MAUtil::Map<const MAUtil::String, double>();
		for(int i = 0; i < Model::NO_OF_CATEGORIES; i++)
		{
			_valueMap->insert(Model::CATEGORY_LIST[i], 0);
		}

		addScreenListener(this);

		MAExtent size = maGetScrSize();
		int screenWidth = EXTENT_X(size);
		int screenHeight = EXTENT_Y(size);
		_createUI(screenHeight, screenWidth);
	}

	HomeScreen::~HomeScreen()
	{
		delete _valueMap;
		delete _categoryGraphicsMap;
	}

	void HomeScreen::_createUI(const int& screenHeight, const int& screenWidth)
	{
		lprintfln("HomeScreen::_createUI");
		_parentLayoutWidth = 8 * (screenWidth / 10);

		_categoryGraphicWidth = (int)(_parentLayoutWidth - 2.5 * ( screenWidth / 10 ));
		_categoryGraphicHeight = screenHeight / 20;

		_mainLayout = new NativeUI::VerticalLayout();

		_activityIndicator = new NativeUI::ActivityIndicator();
		_activityIndicator->fillSpaceHorizontally();
		_mainLayout->addChild(_activityIndicator);
		_activityIndicator->show();

		_createBudgetLabel(screenHeight / 20);
		_createSimpleExpensesGraphic((int)_parentLayoutWidth, screenHeight / 20);
		_createDetailedGraphic((int)_parentLayoutWidth, screenHeight / 20);
		createOptionsMenu();

		this->setMainWidget(_mainLayout);
	}

	void HomeScreen::_createBudgetLabel(int heigth)
	{
		_budgetLabel = new NativeUI::Label();

		char budgetString[BUFFER_SIZE];
		sprintf(budgetString, "Consumed %.2f / %.2f %s", _budgetConsumedValue, _budgetTotalValue, _coin.c_str());

		_budgetLabel->fillSpaceHorizontally();
		_budgetLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		_budgetLabel->setText(budgetString);
		_budgetLabel->setFontSize(FONT_SIZE);
		_budgetLabel->setHeight(heigth);
		_budgetLabel->setTextVerticalAlignment(MAW_ALIGNMENT_TOP);
	}

	void HomeScreen::_createSimpleExpensesGraphic(const int& width, const int& height)
	{
		_budgetSimpleGraphicParentLayout = new NativeUI::VerticalLayout();

		_budgetSimpleGraphicParentLayout->addChild(_budgetLabel);

		_budgetSimpleGraphicTotalBudgetLayout = new NativeUI::HorizontalLayout();
		_budgetSimpleGraphicTotalBudgetLayout->setBackgroundColor(GREEN);
		_budgetSimpleGraphicTotalBudgetLayout->setSize( width, height);

		_budgetSimpleGraphicConsumedBudgetLayout = new NativeUI::HorizontalLayout();
		_budgetSimpleGraphicConsumedBudgetLayout->setBackgroundColor(YELLOW);

		double valueWidth = (_budgetConsumedValue / _budgetTotalValue) * width;

		_budgetSimpleGraphicConsumedBudgetLayout->setHeight(height);
		_budgetSimpleGraphicConsumedBudgetLayout->setWidth(valueWidth);

		_spacerSimpleGraphic = new NativeUI::HorizontalLayout();
		_spacerSimpleGraphic->setBackgroundColor(RED);
		_spacerSimpleGraphic->setSize(SPACER_WIDTH, height);

		_budgetSimpleGraphicTotalBudgetLayout->addChild(_budgetSimpleGraphicConsumedBudgetLayout);
		_budgetSimpleGraphicTotalBudgetLayout->addChild(_spacerSimpleGraphic);

		_budgetSimpleGraphicParentLayout->addChild(_budgetSimpleGraphicTotalBudgetLayout);

		_mainLayout->addChild(_budgetSimpleGraphicParentLayout);
	}

	void HomeScreen::_createDetailedGraphic(const int& width, const int& height)
	{
		NativeUI::HorizontalLayout* customSpacer = new NativeUI::HorizontalLayout();
		customSpacer->setHeight(SPACER_HEIGHT);
		_mainLayout->addChild(customSpacer);

		for(int i = 0; i < _valueMap->size(); i++)
		{
			_mainLayout->addChild(_createCategoryGraphic(Model::CATEGORY_LIST[i], _budgetTotalValue,
								  (*_valueMap)[Model::CATEGORY_LIST[i]],
								  width, height, i));
		}
	}

	NativeUI::VerticalLayout* HomeScreen::_createCategoryGraphic(const MAUtil::String& category, const double& total, const double& consumed, const int& width, const int& height, const int& index)
	{
		NativeUI::VerticalLayout* categoryGraphicParentLayout = new NativeUI::VerticalLayout();
		NativeUI::HorizontalLayout* categoryGraphicLayout = new NativeUI::HorizontalLayout();
		NativeUI::HorizontalLayout* consumeBar = new NativeUI::HorizontalLayout();
		NativeUI::Label* categoryLabel = new NativeUI::Label();
		NativeUI::HorizontalLayout* spacer = new NativeUI::HorizontalLayout();

		spacer->setSize(SPACER_WIDTH, height);

		categoryLabel->setText(category);
		categoryLabel->setWidth((int)(width / 3.5));
		categoryLabel->setFontSize(FONT_SIZE);

		consumeBar->setBackgroundColor(YELLOW);
		double valueWidth = (consumed / total) * width;
		consumeBar->setSize((int)valueWidth, (int)height);

		if((int)valueWidth == 0)
			consumeBar->setWidth(1);

		categoryGraphicLayout->addChild(spacer);
		categoryGraphicLayout->addChild(categoryLabel);
		categoryGraphicLayout->addChild(consumeBar);

		categoryGraphicLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);

		categoryGraphicParentLayout->addChild(categoryGraphicLayout);
		categoryGraphicParentLayout->fillSpaceHorizontally();

		_categoryGraphicsMap->insert(Model::CATEGORY_LIST[index], consumeBar);

		return categoryGraphicParentLayout;
	}

	void HomeScreen::createOptionsMenu()
	{
		lprintfln("createOptionsMenu");
		_addExpenseIndex = addOptionsMenuItem("Expense", "addIncome.png", true);
		_addIncomeIndex = addOptionsMenuItem("Income", MAW_OPTIONS_MENU_ICON_CONSTANT_ADD, false);
	}

	void HomeScreen::_removeActivityIndicator()
	{
		_activityIndicator->hide();
	}

	void HomeScreen::optionsMenuItemSelected(NativeUI::Screen* screen, int index)
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
					_addExpensesDialog->updateAmountSliderValue();
					_addExpensesDialog->setLaunchedFromHomeScreen(true);
					_addExpensesDialog->show();
				}
			}
			else if(_addIncomeIndex == index) //settings screen
			{
				if(NULL != _addIncomeDialog)
				{
					_addIncomeDialog->setCoin(_coin);
					_addIncomeDialog->setObserver(_observerReference);
					_addIncomeDialog->setLaunchedFromHomeScreen(true);
					_addIncomeDialog->show();
				}
			}
		}
	}

	void HomeScreen::optionsMenuClosed(NativeUI::Screen* screen)
	{
		//Dance for me baby
	}

	void HomeScreen::setObserver(Logical::Observer* observer)
	{
		_observerReference = observer;
		_updateValues();
	}

	void HomeScreen::updateBudgetValues(const double& value, bool isExpense, const MAUtil::String& category)
	{
		if(isExpense)
		{
			(*_valueMap)[category] += value;
			_budgetConsumedValue += value;
			for(int i = 0; i < Model::NO_OF_CATEGORIES; i++)
				_updateConsumeBar((*_categoryGraphicsMap)[Model::CATEGORY_LIST[i]], (*_valueMap)[Model::CATEGORY_LIST[i]]);
			updateSimpleGraphic();
		}
		else
		{
			_budgetTotalValue += value;
			updateSimpleGraphic();
		}
	}

	void HomeScreen::_updateConsumeBar(NativeUI::HorizontalLayout* consumeBar, const double& value)
	{
		if(0 == value)
			consumeBar->setWidth(1);
		else
		{
			double valueWidth = (value / _budgetTotalValue) * _categoryGraphicWidth;
			if(valueWidth > _categoryGraphicWidth)
			{
				NativeUI::HorizontalLayout* debtPart = new NativeUI::HorizontalLayout();
				debtPart->setSize((int)(valueWidth - _categoryGraphicWidth), _categoryGraphicHeight);
				debtPart->setBackgroundColor(RED);
				consumeBar->addChild(debtPart);
				consumeBar->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
				consumeBar->setWidth(_categoryGraphicWidth);
			}
			else
			{
				consumeBar->setWidth((int)valueWidth);
				consumeBar->setBackgroundColor(YELLOW);
			}
		}
	}

	void HomeScreen::updateSimpleGraphic()
	{
		if((_budgetTotalValue - _budgetConsumedValue) >= 0)
		{
			double valueWidth = (_budgetConsumedValue / _budgetTotalValue) * _parentLayoutWidth;
			if((int)(_budgetTotalValue - _budgetConsumedValue) == 0)
			{
				_budgetSimpleGraphicConsumedBudgetLayout->setWidth(1);
			}
			else
			{
				_budgetSimpleGraphicConsumedBudgetLayout->setWidth((int)valueWidth);
			}

			char budgetString[BUFFER_SIZE];
			sprintf(budgetString, "Consumed %.2f / %.2f %s", _budgetConsumedValue, _budgetTotalValue, _coin.c_str());

			_budgetLabel->setText(budgetString);
		}
		else
		{
			double valueWidth = ((_budgetConsumedValue / _budgetTotalValue) * _parentLayoutWidth) - _parentLayoutWidth;
			_budgetSimpleGraphicConsumedBudgetLayout->setWidth((int)valueWidth);
			_budgetSimpleGraphicConsumedBudgetLayout->setBackgroundColor(RED);
			_budgetSimpleGraphicTotalBudgetLayout->setBackgroundColor(YELLOW);

			char budgetString[BUFFER_SIZE];
			sprintf(budgetString, "Consumed %.2f / %.2f %s", _budgetConsumedValue, _budgetTotalValue, _coin.c_str());

			_budgetLabel->setText(budgetString);
		}
	}

	void HomeScreen::updateTotalBudget(const double& value)
	{
		_budgetTotalValue = value;
	}

	void HomeScreen::updateConsumedBudget(const double& value)
	{
		_budgetConsumedValue = value;
	}

	void HomeScreen::updateDebtBudget(const double& value)
	{
		_debtBudget = value;
	}

	void HomeScreen::_updateValues()
	{
		_budgetTotalValue =  _observerReference->requestTotalBudget();
		_budgetConsumedValue = _observerReference->requestConsumedBudget();
		_debtBudget = _observerReference->requestDebtValue();

		_addExpensesDialog->setAcceptedDebtValue(_debtBudget);

		for(int i = 0; i < Model::NO_OF_CATEGORIES; i++)
		{
			(*_valueMap)[Model::CATEGORY_LIST[i]] = _observerReference->requestCategoryAmount(Model::CATEGORY_LIST[i]);
		}
		updateBudgetValues(0, true, Model::CATEGORY_LIST[0]);
	}

	void HomeScreen::setAddExpensesDialogReference(NativeUI::Dialog* obj)
	{
		_addExpensesDialog = (AddExpenseDialog*)obj;
		_addExpensesDialog->setHomeScreenRef(this);
	}

	void HomeScreen::setAddIncomesDialogReference(NativeUI::Dialog* obj)
	{
		_addIncomeDialog = (AddIncomeDialog*)obj;
		_addIncomeDialog->setHomeScreenRef(this);
	}

	void HomeScreen::addExpensesDialogLoaded()
	{
		_expensesDialogLoaded = true;
		if(_incomesDialogLoaded)
			_removeActivityIndicator();
	}
	void HomeScreen::addIncomesDialogLoaded()
	{
		_incomesDialogLoaded = true;
		if(_expensesDialogLoaded)
			_removeActivityIndicator();
	}

	void HomeScreen::setCoin(const MAUtil::String& coin)
	{
		_coin = coin;

		char budgetString[BUFFER_SIZE];
		sprintf(budgetString, "Consumed %.2f / %.2f %s", _budgetConsumedValue, _budgetTotalValue, _coin.c_str());

		_budgetLabel->setText(budgetString);
	}
}
