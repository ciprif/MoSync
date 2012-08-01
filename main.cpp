#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/Widgets.h>
#include <NativeUI/WidgetUtil.h>
#include "Screens/homeScreen.h"
#include "Screens/addExpenseDialog.h"
#include "Screens/addIncomeDialog.h"
#include "Screens/listScreen.h"
#include "Screens/settingsScreen.h"
#include "Logical/observer.h"
#include "Logical/settingsManager.h"

using namespace MAUtil;
using namespace NativeUI;

/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet : public Moblet
{
public:
	/**
	 * The constructor creates the user interface.
	 */
	NativeUIMoblet()
	{
		_observer = new Logical::Observer();

		createUI();
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		delete _homeScreen;
		delete _parentScreen;
		delete _observer;
	}

	/**
	 * Create the user interface.
	 */
	void createUI()
	{
		_parentScreen = new PanoramaView();
		_parentScreen->setTitle("MyBudget");


		// Create a NativeUI screen that will hold layout and widgets.
		_listScreen = new GUI::ListScreen();
		_listScreen->setTitle("Transactions");
		_homeScreen = new GUI::HomeScreen();
		_homeScreen->setTitle("Home");
		_settingsScreen = new GUI::SettingsScreen();
		_settingsScreen->setTitle("Settings");

		_observer->setHomeScreenRef(_homeScreen);
		_observer->setListScreenRef(_listScreen);
		_observer->setSettingsScreenRef(_settingsScreen);

		_parentScreen->addScreen(_homeScreen);
		_parentScreen->addScreen(_listScreen);
		_parentScreen->addScreen(_settingsScreen);
		//Show the screen
		_parentScreen->show();

		_incomesDialog = new GUI::AddIncomeDialog();
		_expensesDialog = new GUI::AddExpenseDialog();

		_homeScreen->setAddExpensesDialogReference(_expensesDialog);
		_homeScreen->setAddIncomesDialogReference(_incomesDialog);
		_listScreen->setAddExpensesDialogReference(_expensesDialog);
		_listScreen->setAddIncomeDialogReference(_incomesDialog);

		_listScreen->populateIncomesList();
		_listScreen->populateExpensesList();
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

private:
	PanoramaView* _parentScreen;
    GUI::HomeScreen* _homeScreen;
    GUI::ListScreen* _listScreen;
    GUI::SettingsScreen* _settingsScreen;
    GUI::AddExpenseDialog* _expensesDialog;
    GUI::AddIncomeDialog* _incomesDialog;

    Logical::Observer* _observer;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new NativeUIMoblet());
	return 0;
}
