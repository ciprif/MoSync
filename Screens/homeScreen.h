/*
 * homeScreen.h
 *
 *  Created on: Jun 12, 2012
 *      Author: Cipri
 */

#ifndef HOMESCREEN_H_
#define HOMESCREEN_H_

#include <NativeUI/Screen.h>
#include <NativeUI/ScreenListener.h>
#include <MAUtil/String.h>

#define BUFFER_SIZE 256

#define SPACER_WIDTH 10
#define SPACER_HEIGHT 40
#define FONT_SIZE 25


//Forward declarations
class AddExpenseDialog;
class AddIncomeDialog;

namespace NativeUI
{
	class Label;
	class HorizontalLayout;
	class VerticalLayout;
	class PanoramaView;
	class Dialog;
	class ActivityIndicator;
}

namespace Logical
{
	class Observer;
}

namespace GUI
{
	class HomeScreen : public NativeUI::Screen, public NativeUI::ScreenListener
	{
	public:
		HomeScreen();
		~HomeScreen();

		/**
		 * This method is called when the OptionsMenu is being closed
		 * (either by the user canceling the menu with the back/menu
		 * button, or when an item is selected.
		 * @param screen The screen that generated the event.
		 */
		void optionsMenuClosed(NativeUI::Screen* screen);

		/**
		 * This method is called when an options menu item is selected.
		 * @param index The index on which the item is placed into the
		 * OptionsMenu.
		 * @param screen The screen that generated the event.
		 */
		void optionsMenuItemSelected(NativeUI::Screen* screen, int index);

		/**
		 * \brief creates the options menu
		 */
		void createOptionsMenu();

		/**
		 * \brief sets the observer for this screen
		 * @param observer the observer for this screen
		 */
		void setObserver(Logical::Observer* observer);

		/**
		 * \brief update the values for the graphics
		 * @param value the value
		 * @param isExpense true if the transaction is an expense -> the value will be withdrawn
		 * @param category != "" if the transaction is an expense, will keep the category
		 */
		void updateBudgetValues(const double& value, bool isExpense, const MAUtil::String& category);

		void updateTotalBudget(const double&);

		void updateConsumedBudget(const double&);

		void updateDebtBudget(const double&);

		void updateSimpleGraphic();

		void setAddExpensesDialogReference(NativeUI::Dialog* obj);
		void setAddIncomesDialogReference(NativeUI::Dialog* obj);

		void addExpensesDialogLoaded();
		void addIncomesDialogLoaded();
		void setCoin(const MAUtil::String&);

		void updateValues();
	private:
		/**
		 * \brief creates the main layout and sets some enviroment variables. This is the point where
		 * 		  all the UI is initialized;
		 * @param screenHeight the screen height.
		 * @param screenWidth the screen width.
		 */
		void _createUI(const int& screenHeight, const int& screenWidth);

		/**
		 * \brief creates the budget information label and the related parent controls.
		 * @param height the height of the screen.
		 */
		void _createBudgetLabel(int height);

		/**
		 * \brief creates the simple graphic bar the illustrates the amount consumed.
		 * @param width the screen width
		 * @param height the screen height
		 */
		void _createSimpleExpensesGraphic(const int& width, const int& height);

		/**
		 * \brief creates the detailed graphic, bar for each category of expense
		 * @param width the width of the screen
		 * @param height the height of the screen
		 */
		void _createDetailedGraphic(const int& width, const int& height);

		/**
		 * \brief creates a category graphic
		 * @param category the category as a string
		 * @param total the total budget
		 * @param consumed the budget consumed of this category of expenses
		 * @param width the width of the screen
		 * @param height the height of the screen
		 * @return NativeUI::VerticalLayout* the vertical layout containing the desired information
		 */
		NativeUI::VerticalLayout* _createCategoryGraphic(const MAUtil::String& category, const double& total, const double& consumed, const int& width, const int& height, const int& index);

		void _updateConsumeBar(NativeUI::HorizontalLayout* consumeBar, const double& value);
		void _removeActivityIndicator();

		// Member data
		double _budgetTotalValue;
		double _budgetConsumedValue;
		double _debtBudget;
		MAUtil::String _coin;

		NativeUI::VerticalLayout* _budgetSimpleGraphicParentLayout;
		NativeUI::VerticalLayout* _mainLayout;
		NativeUI::HorizontalLayout* _budgetSimpleGraphicTotalBudgetLayout;
		NativeUI::HorizontalLayout* _budgetSimpleGraphicConsumedBudgetLayout;
		NativeUI::HorizontalLayout* _spacerSimpleGraphic;

		NativeUI::Label* _budgetLabel;

		int _addExpenseIndex;
		int _settingsButtonIndex;
		int _addIncomeIndex;

		AddExpenseDialog* _addExpensesDialog;
		AddIncomeDialog* _addIncomeDialog;

		Logical::Observer* _observerReference;
		MAUtil::Map<const MAUtil::String, double>* _valueMap;
		MAUtil::Map<const MAUtil::String, NativeUI::HorizontalLayout*>* _categoryGraphicsMap;

		NativeUI::ActivityIndicator* _activityIndicator;

		int _categoryGraphicHeight;
		int _categoryGraphicWidth;

		bool _incomesDialogLoaded;
		bool _expensesDialogLoaded;
		bool _progressBarActive;

		double _parentLayoutWidth;
	};
}
#endif /* HOMESCREEN_H_ */
